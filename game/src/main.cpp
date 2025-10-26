#include <chrono>
#include "sol/sol.hpp"
#include <iostream>
#include <csignal>
#include "common/RingBuffer.hpp"
#include "common/XLog.h"
#include "network/Dispatcher.h"
#include "MsgHandler.h"
#include "GameRole.h"
#include "ProtoInner.pb.h"

#include "thread/AthenaThreadPool.h"
#include "common/ObjectPool.hpp"
#include "db/Dal.hpp"
#if defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#endif
#include "transport/AthenaTcpServer.h"
#include "transport/Channel.h"


static std::atomic<bool> g_running(true);
static std::condition_variable g_cv;
static std::mutex g_mutex;

void handleSignal(int signum) {
    INFO_LOG("Received signal {} exiting...", signum);
    g_running = false;
    g_cv.notify_all(); // 唤醒主线程
}

int main(int argc, char **argv) {
    std::signal(SIGTERM, handleSignal);
    std::signal(SIGINT, handleSignal);
    xLogInitLog(LogLevel::LL_INFO, "../logs/game.log");

    AthenaTcpServer  tcp_server;
    tcp_server.onNewConnection=[](Channel* channel){
        INFO_LOG("on new connection ={}",channel->getAddr());

    };
    tcp_server.onClosed=[](Channel* channel){
        INFO_LOG("connection ={}  closed ",channel->getAddr());
    };
    tcp_server.bind(9999).start(3);

    std::string ip = "172.18.2.101";
    Dal::Cache::init(ip, 6379, "", "", "");
    //  Dal::Cache::execute()

    //     RingBuffer<int *> *pRingBuf = new RingBuffer<int *>(3);
    //     int i1 = 1;
    //     int i2 = 2;
    //     int i3 = 3;
    //     int i4 = 4;     int **arrya = new int *[4];
    //     pRingBuf->push(arrya, 4);
    //     int* op = pRingBuf->pop();
    //     if (op == nullptr) {
    //         INFO_LOG("not found element");
    //     }
    //     pRingBuf->push(&i1);
    //     pRingBuf->push(&i2);
    //     pRingBuf->push(&i3);
    //     for (int i = 0; i < 1000; ++i)
    //     {
    //         int *point = pRingBuf->pop();
    //         pRingBuf->push(&i4);
    //     }
    //
    //    // pRingBuf->push(&i4);
    //
    std::shared_ptr<InnerHead> pInnherHead = std::make_shared<InnerHead>();
    pInnherHead->set_id(14441);
    std::string pServer = pInnherHead->SerializeAsString();
    // std::shared_ptr<InnerHead> pInnherHead2 = std::make_shared<InnerHead>();
    // bool ret = pInnherHead2->ParseFromString(pServer);
    // // std::cout<<"------ "<<pInnherHead2->id() <<std::endl;
    // INFO_LOG("------{}", pInnherHead2->id());

    // REGISTER_MSG_ID_FUN(100, MsgHandler::onSomeMsg);
    // Dispatcher::Instance()->processMsg(100, 8889, pServer.c_str(), pServer.length());

    REGISTER_MSG_ID_FUN(100, MsgHandler::onSomeMsg);
    Thread::ThreadPool *threadPool = new AthenaThreadPool();
    threadPool->create(2);

    int msgId = 100;
    int playerId = 999;
    threadPool->execute([msgId, playerId, pServer]() {
        Dispatcher::Instance()->processMsg(msgId, playerId, pServer.c_str(), pServer.length());
    }, 2);

    int i = 1;
    i++;
    threadPool->execute([i]() {
        INFO_LOG("thread test 2={}", i);
    }, 2);

    threadPool->execute([i]() {
        INFO_LOG("thread test 2={}", i);
    }, 2);

    {
        ObjectPool<GameRole>::PoolObjRef ref = ObjPool::acquire<GameRole>(100);
        INFO_LOG("pool obj GameRole test  pid ={}", ref->getPid());
    }
    INFO_LOG("==========================  wait release");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    threadPool->execute([]() {
        INFO_LOG("thread test   obj=3");
    }, 2);


    INFO_LOG("========================== should be hit");
    int x = 100;
    threadPool->execute([x]() {
        INFO_LOG("thread test x=2={}", x);
    }, 2);

    threadPool->execute([] {
        INFO_LOG(" thread test 1={}", 1);
    }, 1);

    {
        ObjectPool<GameRole>::PoolObjRef poolRef = GameRole::claim(888);
        INFO_LOG("pool obj GameRole test  pid ={}", poolRef->getPid());
    }
    // std::this_thread::sleep_for(std::chrono::seconds(1));

    // 💡 主线程阻塞等待，无限期休眠（CPU 占用≈0）
    {
        std::unique_lock<std::mutex> lock(g_mutex);
        g_cv.wait(lock, [] { return !g_running.load(); });
    }

    INFO_LOG("service exited");

    return 0;
}

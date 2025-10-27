//
// Created by zhongweiqi on 2025/10/20.
//

#include <chrono>
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
#include "transport/AthenaTcpServer.h"

#if defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#endif
#include "transport/TcpClient.h"

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

    xLogInitLog(LogLevel::LL_INFO, "../logs/gateway.log");
    // AthenaTcpServer athenaTcpServer;
    // athenaTcpServer.bind(6666).start(4);
    //
    // std::string ip = "172.18.2.101";
    // Dal::Cache::init(ip, 6379, "", "", "");

    TcpClient tcp_client;
    tcp_client.onConnected = [](Channel *channel) {
        INFO_LOG(" on   Connected...  {}", channel->getAddr());
    };
    tcp_client.start();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    tcp_client.connect("127.0.0.1",9999);


   // AthenaTcpClient athena_tcp_client;
    // for (int i = 0; i < 2; ++i) {
    //     int connRet = athena_tcp_client.connect("localhost", 38881);
    //     if (connRet) {
    //         ERR_LOG(" errror  connect rest ={}", connRet);
    //     }
    // }
    // 💡 主线程阻塞等待，无限期休眠（CPU 占用≈0）
    {
        std::unique_lock<std::mutex> lock(g_mutex);
        g_cv.wait(lock, [] { return !g_running.load(); });
    }

    INFO_LOG("service exited");

    return 0;
}

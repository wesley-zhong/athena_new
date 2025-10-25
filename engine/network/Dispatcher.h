#ifndef DISPATCHER_H_
#define DISPATCHER_H_

#include <functional>
#include <map>
#include <iostream>
#include <google/protobuf/message.h>
#include "Singleton.h"


#define REGISTER_MSG_ID_FUN(MSGID, FUNCTION) \
Dispatcher::Instance()->registerMsgHandler(MSGID, std::function(FUNCTION))

struct MsgFunction {
    std::function<void(int64_t, void *)> function;
    std::function<void *()> newParam;    //this may be use obj pool
};

class Dispatcher : public Singleton<Dispatcher> {

public:
    Dispatcher() = default;

    ~Dispatcher() = default;

    template<typename T>
    void registerMsgHandler(int msgId, std::function<void(int64_t, T *)> msgFuc);

//  template <typename T>
//  void  registerMsgHandlers(int msgId, std::function<void(int, std::shared_ptr<T>)> msgFuc);

    void processMsg(int msgId, int64_t playerId, const void *body, int len);
    // template <typename T>
    // void callFunction(int msgId, std::shared_ptr<T> param_);

private:
    std::map<int, MsgFunction *> msgMap;
    // std::map<int, std::function<void(int, std::shared_ptr<void> )>> msgSMap;
};

template<typename T>
void Dispatcher::registerMsgHandler(int msgId, std::function<void(int64_t, T *)> msgFuc) {
    auto *msgFunction = new MsgFunction();
    msgFunction->function = [msgFuc](int64_t p1, void *p2) {
        msgFuc(p1, (T *) p2);
    };
    msgFunction->newParam = []() {
        return new T();
    };
    msgMap[msgId] = msgFunction;
}
// template <typename T>
// void Dispatcher::registerMsgHandlers(int msgId, std::function<void(int, std::shared_ptr<T>)> msgFuc)
// {
//     msgMap[msgId] = [msgId, msgFuc](int p1, std::shared_ptr<void> p2)
//   {
//     msgFuc(msgId, (std::shared_ptr<T>)p2);
//   };
// }


void Dispatcher::processMsg(int msgId, int64_t playerId, const void *body, int len) {
    auto func = msgMap[msgId];
    if (func == nullptr) {
        INFO_LOG("msg id ={} not found", msgId);
        return;
    }
    auto *param = (google::protobuf::Message *) func->newParam();
    param->ParseFromArray(body, len);
    func->function(playerId, param);
}

#endif
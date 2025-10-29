#ifndef DISPATCHER_H_
#define DISPATCHER_H_

#include <functional>
#include <map>
#include "google/protobuf/message.h"
#include "Singleton.h"
#include "ObjectPool.hpp"


#define REGISTER_MSG_ID_FUN(MSGID, FUNCTION) \
Dispatcher::Instance()->registerMsgHandler(MSGID, std::function(FUNCTION))

struct MsgFunction {
    std::function<void(int64_t, void *)> function;
    std::function<void *()> newParam; //this may be use obj pool
};

class Dispatcher : public Singleton<Dispatcher> {
public:
    Dispatcher() = default;

    ~Dispatcher() = default;

    template<typename T>
    void registerMsgHandler(int msgId, std::function<void(int64_t, T *)> msgFuc);

    void processMsg(int msgId, int64_t playerId, const void *body, int len);

    MsgFunction *findMsgFuncion(int msgId) {
        auto it = msgMap.find(msgId);
        if (it != msgMap.end()) {
            return it->second;
        }
        return nullptr;
    }

private:
    std::map<int, MsgFunction *> msgMap;
    // std::map<int, std::function<void(int, std::shared_ptr<void> )>> msgSMap;
};

template<typename T>
void Dispatcher::registerMsgHandler(int msgId, std::function<void(int64_t, T *)> msgFuc) {
    auto *msgFunction = new MsgFunction();
    msgFunction->function = [msgFuc](int64_t p1, void *p2) {
        msgFuc(p1, (T *) p2);
        ObjPool::release<T>((T *) p2, true);
    };
    msgFunction->newParam = []() {
        // return new T();
        return ObjPool::acquirePtr<T>();
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


#endif

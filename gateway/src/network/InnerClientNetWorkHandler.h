//
// Created by zhongweiqi on 2025/10/28.
//

#ifndef ATHENA_NETWORKHANDLER_H
#define ATHENA_NETWORKHANDLER_H
#include "thread/AthenaThreadPool.h"
#include "transport/EventDefs.h"
struct MsgFunction;
class Channel;


class InnerClientNetWorkHandler {
public:
    static void initAllMsgRegister();

    static void startThread(int threadNum);

    static void onNewConnect(Channel *channel, int status);

    static void onMsg(Channel *channel, void *buff, int len);

    static void onClosed(Channel *channel);

    static void onEventTrigger(Channel *channel, TriggerEventEnum reason);

    static Thread::ThreadPool *threadPool;

private:
    static void processInnerLogin(MsgFunction *msg_function, Channel *channel, void *body, int len);
};


#endif //ATHENA_NETWORKHANDLER_H

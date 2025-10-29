//
// Created by zhongweiqi on 2025/10/28.
//

#ifndef ATHENA_NETWORKHANDLER_H
#define ATHENA_NETWORKHANDLER_H
#include "thread/AthenaThreadPool.h"
struct MsgFunction;
class Channel;


class InnerNetWorkHandler {
public:
    static void initAllMsgRegister();

    static void startThread(int threadNum);

    static void onConnect(Channel *channel);

    static void onMsg(Channel *channel, void *buff, int len);

    static void onClosed(Channel *channel);

    static Thread::ThreadPool *threadPool;

private:
    template<class  T>
    static void processMsgWithChannel(MsgFunction *msg_function, Channel *channel, void *body, int len);
};
#endif //ATHENA_NETWORKHANDLER_H

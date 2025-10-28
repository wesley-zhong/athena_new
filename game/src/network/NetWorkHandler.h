//
// Created by zhongweiqi on 2025/10/28.
//

#ifndef ATHENA_NETWORKHANDLER_H
#define ATHENA_NETWORKHANDLER_H
#include "AthenaThreadPool.h"
class Channel;


class NetWorkHandler {
public:
    static void initAllMsgRegister();

    static void startThread(int threadNum);

    static void onConnect(Channel *channel);

    static void onMsg(Channel *channel, void *buff, int len);

    static void onClosed(Channel *channel);

    static Thread::ThreadPool *threadPool;
};


#endif //ATHENA_NETWORKHANDLER_H

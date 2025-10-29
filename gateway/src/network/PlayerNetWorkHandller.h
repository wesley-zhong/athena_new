//
// Created by zhongweiqi on 2025/10/28.
//

#ifndef ATHENA_PLAYERNETWORKHANDLLER_H
#define ATHENA_PLAYERNETWORKHANDLLER_H
#include "thread/AthenaThreadPool.h"
struct MsgFunction;
class Channel;


class PlayerNetWorkHandller {
public:
    static void initAllMsgRegister();

    static void startThread(int threadNum);

    static void onConnect(Channel *channel);

    static void onMsg(Channel *channel, void *buff, int len);

    static void onClosed(Channel *channel);

    static Thread::ThreadPool *threadPool;

    static void processPlayerLogin(MsgFunction *msg_function, Channel *channel, void *body, int len);
};


#endif //ATHENA_PLAYERNETWORKHANDLLER_H

//
// Created by zhongweiqi on 2025/10/28.
//

#pragma once
#include "thread/AthenaThreadPool.h"
struct MsgFunction;
class Channel;


class ClientNetWorkHandler {
public:
    static void initAllMsgRegister();

    static void startThread(int threadNum);

    static void onConnect(Channel *channel);

    static void onMsg(Channel *channel, void *buff, int len);

    static void onClosed(Channel *channel);

    static Thread::ThreadPool *threadPool;

private:
    static void processInnerLogin(MsgFunction *msg_function, Channel *channel, void *body, int len);
};



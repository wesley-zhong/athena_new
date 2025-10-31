//
// Created by zhongweiqi on 2025/10/28.
//

#include "InnerServerNetWorkHandler.h"
#include "network/Dispatcher.h"
#include "MsgHandler.h"
#include "transport/Channel.h"
#include "XLog.h"
#include "transport/ByteUtils.h"


void InnerServerNetWorkHandler::initAllMsgRegister() {
    REGISTER_MSG_ID_FUN(INNER_SERVER_HAND_SHAKE_REQ, InnerServerHandShakeReq, MsgHandler::onShakHandReq);
    REGISTER_MSG_ID_FUN(INNER_HEART_BEAT_REQ, InnerHeartBeatRequest, MsgHandler::onHeartBeat);

    REGISTER_MSG_ID_FUN(INNER_TO_GAME_LOGIN_REQ, InnerLoginRequest, MsgHandler::onInnerLogin);
}

void InnerServerNetWorkHandler::startThread(int threadNum) {
    threadPool = new AthenaThreadPool();
    threadPool->create(2);
}

void InnerServerNetWorkHandler::onNewConnect(Channel *channel) {
    INFO_LOG("on new connection ={}", channel->getAddr());
}

void InnerServerNetWorkHandler::onMsg(Channel *channel, void *buff, int len) {
    INFO_LOG("  === on read   channel ={} len ={}", channel->getAddr(), len);
    uint8 *data = static_cast<uint8 *>(buff);
    data = data + 4;
    int msgId = ByteUtils::readInt32(data);
    int playerId = 999;
    data += 4;
    len -= 8;

    MsgFunction *msg_function = Dispatcher::Instance()->findMsgFuncion(msgId);
    if (msg_function == nullptr) {
        ERR_LOG(" msgId ={} not found process function", msgId);
        return;
    }

    void *msg = msg_function->newParam((char *) data + 4, len);
    threadPool->execute([playerId, msg_function, channel, msg]() {
        msg_function->msgFunction(playerId, channel, msg);
    }, 2);
}


void InnerServerNetWorkHandler::onClosed(Channel *channel) {
    INFO_LOG("connection ={}  closed ", channel->getAddr());
}

void InnerServerNetWorkHandler::onEventTrigger(Channel *channel, TriggerEventEnum reason) {
    INFO_LOG("====onEventTrigger ={}   reason ={} ", channel->getAddr(), (int)reason);
}
Thread::ThreadPool *InnerServerNetWorkHandler::threadPool = nullptr;

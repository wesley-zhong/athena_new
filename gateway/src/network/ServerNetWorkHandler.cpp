//
// Created by zhongweiqi on 2025/10/28.
//

#include "ServerNetWorkHandler.h"
#include "network/Dispatcher.h"
#include "MsgHandler.h"
#include "transport/Channel.h"
#include "XLog.h"
#include "transport/ByteUtils.h"
#include "ProtoInner.pb.h"


void ServerNetWorkHandler::initAllMsgRegister() {
    REGISTER_MSG_ID_FUN(INNER_TO_GAME_LOGIN_RES,InnerLoginResponse, MsgHandler::onLoginRes);
    REGISTER_MSG_ID_FUN(INNER_SERVER_HAND_SHAKE_RES,InnerServerHandShakeRes, MsgHandler::onShakeHandRes);
}

void ServerNetWorkHandler::startThread(int threadNum) {
    threadPool = new AthenaThreadPool();
    threadPool->create(threadNum);
}

void ServerNetWorkHandler::onConnect(Channel *channel, int status) {
    INFO_LOG(" PlayerNetWorkHandller on new connection ={}", channel->getAddr());
}

void ServerNetWorkHandler::onMsg(Channel *channel, void *buff, int len) {
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


void ServerNetWorkHandler::onClosed(Channel *channel) {
    INFO_LOG("connection ={}  closed ", channel->getAddr());
}

void ServerNetWorkHandler::onEventTrigger(Channel *channel, TriggerEventEnum reason) {
}


Thread::ThreadPool *ServerNetWorkHandler::threadPool = nullptr;

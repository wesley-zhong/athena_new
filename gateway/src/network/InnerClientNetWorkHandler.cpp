//
// Created by zhongweiqi on 2025/10/28.
//

#include "InnerClientNetWorkHandler.h"
#include "network/Dispatcher.h"
#include "MsgHandler.h"
#include "transport/Channel.h"
#include "XLog.h"
#include "transport/ByteUtils.h"

#include "ProtoInner.pb.h"

void InnerClientNetWorkHandler::initAllMsgRegister() {
    REGISTER_MSG_ID_FUN(INNER_TO_GAME_LOGIN_REQ, InnerLoginResponse, MsgHandler::onLoginRes);
    REGISTER_MSG_ID_FUN(INNER_SERVER_HAND_SHAKE_RES, InnerServerHandShakeRes, MsgHandler::onShakeHandRes);
}

void InnerClientNetWorkHandler::startThread(int threadNum) {
    threadPool = new AthenaThreadPool();
    threadPool->create(threadNum);
}

void InnerClientNetWorkHandler::onNewConnect(Channel *channel, int status) {
    INFO_LOG("on new connection ={}", channel->getAddr());
    auto req = std::make_shared<InnerServerHandShakeReq>();
    req->set_service_id("JJJJJJJJJ");
    channel->sendMsg(INNER_SERVER_HAND_SHAKE_REQ, req);
}

void InnerClientNetWorkHandler::onMsg(Channel *channel, void *buff, int len) {
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

void InnerClientNetWorkHandler::onEventTrigger(Channel *channel, TriggerEventEnum reason) {
    INFO_LOG("========== onEventTrigger ={}   reason ={} ", channel->getAddr(), (int)reason);
    if (reason == WRITE_IDLE) {
        auto msg = std::make_shared<InnerHeartBeatRequest>();
        msg->set_time(8888);
        channel->sendMsg(INNER_HEART_BEAT_REQ, msg);
        return;
    }
    // this should be closed
    if (reason == READ_IDLE) {
    }
}


void InnerClientNetWorkHandler::onClosed(Channel *channel) {
    INFO_LOG("connection ={}  closed ", channel->getAddr());
}


Thread::ThreadPool *InnerClientNetWorkHandler::threadPool = nullptr;

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
    REGISTER_MSG_ID_FUN(INNER_TO_GAME_LOGIN_REQ, MsgHandler::onInnerLogin);
    REGISTER_MSG_ID_FUN(INNER_SERVER_HAND_SHAKE_REQ, MsgHandler::onShakHandReq);
}

void InnerServerNetWorkHandler::startThread(int threadNum) {
    threadPool = new AthenaThreadPool();
    threadPool->create(2);
}

void InnerServerNetWorkHandler::onNewConnect(Channel *channel) {
    INFO_LOG("on new connection ={}", channel->getAddr());

}

void InnerServerNetWorkHandler::onMsg(Channel *channel, void *buff, int len) {
    INFO_LOG("  === on read ");
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
    if (msgId == INNER_TO_GAME_LOGIN_REQ) {
        processMsgWithChannel<InnerLoginRequest>(msg_function, channel, data, len);
        return;
    }
    if (msgId == INNER_SERVER_HAND_SHAKE_REQ) {
        processMsgWithChannel<InnerServerHandShakeReq>(msg_function, channel, data, len);
        return;
    }
    auto *param = static_cast<google::protobuf::Message *>(msg_function->newParam());
    param->ParseFromArray(data, len);

    threadPool->execute([playerId, msg_function, param]() {
        msg_function->function(playerId, param);
    }, 2);
}


void InnerServerNetWorkHandler::onClosed(Channel *channel) {
    INFO_LOG("connection ={}  closed ", channel->getAddr());
}

void InnerServerNetWorkHandler::onEventTrigger(Channel *channel, TriggerEventEnum reason) {
    INFO_LOG("====onEventTrigger ={}   reason ={} ", channel->getAddr(),(int)reason);
}

template<class T>
void InnerServerNetWorkHandler::processMsgWithChannel(MsgFunction *msg_function, Channel *channel, void *body, int len) {
    auto msg = static_cast<ReqChannel<T> *>(msg_function->newParam());
    msg->req.ParseFromArray(body, len);
    msg->channel = channel;
    threadPool->execute([msg_function, msg]() {
        msg_function->function(0, msg);
    }, 0);
}

Thread::ThreadPool *InnerServerNetWorkHandler::threadPool = nullptr;

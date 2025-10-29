//
// Created by zhongweiqi on 2025/10/28.
//

#include "InnerNetWorkHandler.h"
#include "network/Dispatcher.h"
#include "MsgHandler.h"
#include "transport/Channel.h"
#include "XLog.h"
#include "transport/ByteUtils.h"

#include "ProtoInner.pb.h"

void InnerNetWorkHandler::initAllMsgRegister() {
    REGISTER_MSG_ID_FUN(INNER_LOGIN_REQ, MsgHandler::onLoginRes);
    REGISTER_MSG_ID_FUN(INNER_SERVER_HAND_SHAKE, MsgHandler::onShakeHandRes);
}

void InnerNetWorkHandler::startThread(int threadNum) {
    threadPool = new AthenaThreadPool();
    threadPool->create(threadNum);
}

void InnerNetWorkHandler::onConnect(Channel *channel) {
    INFO_LOG("on new connection ={}", channel->getAddr());
}

void InnerNetWorkHandler::onMsg(Channel *channel, void *buff, int len) {
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
    if (msgId == INNER_LOGIN_REQ) {
        processInnerLogin(msg_function, channel, data, len);
        return;
    }
    auto *param = static_cast<google::protobuf::Message *>(msg_function->newParam());
    param->ParseFromArray(data, len);

    threadPool->execute([playerId, msg_function, param]() {
        msg_function->function(playerId, param);
    }, 2);
}


void InnerNetWorkHandler::onClosed(Channel *channel) {
    INFO_LOG("connection ={}  closed ", channel->getAddr());
}

void InnerNetWorkHandler::processInnerLogin(MsgFunction *msg_function, Channel *channel, void *body, int len) {
    InnerLogin *inner_login = static_cast<InnerLogin *>(msg_function->newParam());
    inner_login->req.ParseFromArray(body, len);
    inner_login->channel = channel;
    threadPool->execute([msg_function, inner_login]() {
        msg_function->function(0, inner_login);
    }, 0);
}

Thread::ThreadPool *InnerNetWorkHandler::threadPool = nullptr;

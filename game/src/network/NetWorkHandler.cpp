//
// Created by zhongweiqi on 2025/10/28.
//

#include "NetWorkHandler.h"
#include "network/Dispatcher.h"
#include "MsgHandler.h"
#include "transport/Channel.h"
#include "XLog.h"
#include "transport/ByteUtils.h"

void NetWorkHandler::initAllMsgRegister() {
    REGISTER_MSG_ID_FUN(INNER_LOGIN_REQ, MsgHandler::onInnerLogin);
}

void NetWorkHandler::startThread(int threadNum) {
    threadPool = new AthenaThreadPool();
    threadPool->create(2);
}

void NetWorkHandler::onConnect(Channel *channel) {
    INFO_LOG("on new connection ={}", channel->getAddr());
}

void NetWorkHandler::onMsg(Channel *channel, void *buff, int len) {
    INFO_LOG("  === on read ");
    uint8 *data = (uint8 *) buff;
    data = data + 4;
    int msgId = ByteUtils::readInt32(data);
    int playerId = 999;
    data += 4;
    len -= 8;

    MsgFunction *msg_function = Dispatcher::Instance()->findMsgFuncion(msgId);
    if (msg_function == nullptr) {
        ERR_LOG(" msgId ={} not found prcess function", msgId);
        return;
    }
    auto *param = (google::protobuf::Message *) msg_function->newParam();
    param->ParseFromArray(data, len);
    threadPool->execute([playerId, msg_function, param]() {
        msg_function->function(playerId, param);
    }, 2);
}

void NetWorkHandler::onClosed(Channel *channel) {
    INFO_LOG("connection ={}  closed ", channel->getAddr());
}

Thread::ThreadPool *NetWorkHandler::threadPool = nullptr;

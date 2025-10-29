//
// Created by zhongweiqi on 2025/10/29.
//
#include "Dispatcher.h"
#include "XLog.h"

void Dispatcher::processMsg(int msgId, int64_t playerId, const void *body, int len) {
    auto func = msgMap[msgId];
    if (func == nullptr) {
        INFO_LOG("msg id ={} not found", msgId);
        return;
    }
    auto *param = (google::protobuf::Message *) func->newParam();
    param->ParseFromArray(body, len);
    func->function(playerId, param);
}

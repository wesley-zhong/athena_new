#include "MsgHandler.h"
#include "common/XLog.h"
#include "objs/Player.h"
#include "ProtoInner.pb.h"


void MsgHandler::onShakHand(int64_t playerId, InnerServerHandShakeReq *req) {
    INFO_LOG("HELLO playerId ={} innherHeaderId = {}", playerId, req->service_id());
}

void MsgHandler::onInnerLogin(int64_t playerId, InnerLogin *request) {
    INFO_LOG(" ON INNER LOGIN sid = {} roleId ={} channel ={}", request->req.sid(), request->req.roleid(),
             request->channel->getAddr());
}

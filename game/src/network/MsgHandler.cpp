#include "MsgHandler.h"
#include "common/XLog.h"
#include "objs/Player.h"
#include "ProtoInner.pb.h"


void MsgHandler::onShakHandReq(int64_t playerId, ReqChannel<InnerServerHandShakeReq> *req) {
    INFO_LOG("HELLO channle ={} innherHeaderId = {}", req->channel->getAddr(), playerId, req->req.service_id());
    auto res = std::make_shared<InnerServerHandShakeRes>();
    res->set_service_id("9999ttt");
    req->channel->sendMsg(INNER_SERVER_HAND_SHAKE_RES, res);
}

void MsgHandler::onInnerLogin(int64_t playerId, ReqChannel<InnerLoginRequest> *request) {
    INFO_LOG(" ON INNER LOGIN sid = {} roleId ={} channel ={}", request->req.sid(), request->req.roleid(),
             request->channel->getAddr());
    auto res = std::make_shared<InnerLoginResponse>();
    request->channel->sendMsg(INNER_TO_GAME_LOGIN_RES, res);
}

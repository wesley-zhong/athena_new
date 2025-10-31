#include "MsgHandler.h"
#include "common/XLog.h"
#include "objs/Player.h"
#include "ProtoInner.pb.h"


void MsgHandler::onShakHandReq(Channel *channel, InnerServerHandShakeReq *req) {
    INFO_LOG("HELLO channle ={} innherHeaderId = {}", channel->getAddr(), req->service_id());
    auto res = std::make_shared<InnerServerHandShakeRes>();
    res->set_service_id("9999ttt");
    channel->sendMsg(INNER_SERVER_HAND_SHAKE_RES, res);
}

void MsgHandler::onHeartBeat(Channel *channel, InnerHeartBeatRequest *req) {
    auto res = std::make_shared<InnerHeartBeatRequest>();
    channel->sendMsg(INNER_HEART_BEAT_RES, res);
}


void MsgHandler::onInnerLogin(Channel *channel, InnerLoginRequest *request) {
    INFO_LOG(" ON INNER LOGIN sid = {} roleId ={} channel ={}", request->sid(), request->roleid(),
             channel->getAddr());
    auto res = std::make_shared<InnerLoginResponse>();
    channel->sendMsg(INNER_TO_GAME_LOGIN_RES, res);
}

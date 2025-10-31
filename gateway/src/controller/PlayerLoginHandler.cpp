#include "PlayerLoginHandler.h"

#include "common/XLog.h"

#include "ProtoInner.pb.h"
#include "network/Dispatcher.h"

void PlayerLoginHandler::onLoginRes(Channel *channel, InnerLoginResponse *res) {
    INFO_LOG("----- on login res ={}", res->roleid(), res->sid());
}


void PlayerLoginHandler::registMsgHandler() {
    REGISTER_MSG_ID_FUN(INNER_TO_GAME_LOGIN_REQ, InnerLoginResponse, PlayerLoginHandler::onLoginRes);

}
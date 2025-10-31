#include "PlayerLoginHandler.h"

#include "BaseType.h"
#include "common/XLog.h"

#include "ProtoInner.pb.h"


void PlayerLoginHandler::onLoginRes(Channel* channel, InnerLoginResponse *res) {
    INFO_LOG("----- on login res ={}", res->roleid(), res->sid());
}

void PlayerLoginHandler::onShakeHandRes(Channel* channel, InnerServerHandShakeRes *res) {
    INFO_LOG("----- on onShakeHandRes res ={} serviceId ={}", res->service_name(), res->service_id());
}

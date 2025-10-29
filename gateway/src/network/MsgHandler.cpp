#include "MsgHandler.h"

#include "BaseType.h"
#include "common/XLog.h"

#include "ProtoInner.pb.h"


void MsgHandler::onLoginRes(int64_t playerId, InnerLoginResponse *res) {
    INFO_LOG("----- on login res ={}", res->roleid(), res->sid());
}

void MsgHandler::onShakeHandRes(int64_t playerId, InnerServerHandShakeRes *res) {
    INFO_LOG("----- on onShakeHandRes res ={}", res->service_name(), res->service_id());
}

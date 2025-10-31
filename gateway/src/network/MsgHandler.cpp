#include "MsgHandler.h"

#include "BaseType.h"
#include "common/XLog.h"

#include "ProtoInner.pb.h"


void MsgHandler::onLoginRes(Channel *channel, InnerLoginResponse *res) {
    INFO_LOG("----- on login res ={}", res->roleid(), res->sid());
}

void MsgHandler::onShakeHandRes(Channel *channel, InnerServerHandShakeRes *res) {
    INFO_LOG("----- on onShakeHandRes res ={} serviceId ={}", res->service_name(), res->service_id());
}

void MsgHandler::onHeartBeatRes(Channel *channel, InnerHeartBeatResponse *res) {
    INFO_LOG("----- on onHeartBeatRes res ={} time ", res->time());
}

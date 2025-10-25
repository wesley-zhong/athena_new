#include "MsgHandler.h"
#include "common/XLog.h"
#include "GameRole.h"
#include "ProtoInner.pb.h"


void MsgHandler::onLogin(int64_t playerId, GameRole* gameRole){
    INFO_LOG("HELLO playerId ={} roleId = {}",playerId, gameRole->getPid());
}

void MsgHandler::onSomeMsg(int64_t playerId, InnerHead* InnerHead ){
    INFO_LOG("HELLO playerId ={} innherHeaderId = {}",playerId, InnerHead->id());
}
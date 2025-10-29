//
// Created by zhongweiqi on 2025/10/28.
//

#include "Player.h"

void Player::sendMsg(int msgId, std::shared_ptr<google::protobuf::Message> msg) {
    channel->sendMsg(msgId, msg);
}

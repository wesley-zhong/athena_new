//
// Created by zhongweiqi on 2025/10/27.
//

#include "TcpClient.h"


TcpClient::TcpClient() {
    loop = new EventLoop(this);
}



void TcpClient::connect(const std::string &ip, int port) {
    loop->asyncConnect(ip, port);
}

void TcpClient::start() {
    loop->start();
}

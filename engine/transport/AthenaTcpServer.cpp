//
// Created by zhongweiqi on 2025/10/23.
//

#include "AthenaTcpServer.h"

#include "Channel.h"
#include "XLog.h"
static std::mutex g_conns_mtx;

void AthenaTcpServer::uv_on_new_connection(uv_stream_t *server, int status) {
    INFO_LOG("uv_on_new_connection accept  new socket");
    AthenaTcpServer *tcpserver = (AthenaTcpServer *) server->data;
    tcpserver->onAccept(server, status);
}


void AthenaTcpServer::onAccept(uv_stream_t *server, int status) {
    if (status < 0) {
        ERR_LOG("Accept error:{}", uv_strerror(status));
        return;
    }

    INFO_LOG("event loop  accept  new socket");
    // choose reactor
    const int idx = next_reactor++ % event_loops_.size();
    std::unique_ptr<EventLoop> &loopUptr = event_loops_[idx];

    // accept into temporary uv_tcp_t to get the fd
    uv_tcp_t *client = new uv_tcp_t;
    uv_tcp_init(loopUptr->uv_loop(), client);
    client->data = loopUptr.get();
    loopUptr->asyncAccept(server, client);
}

void AthenaTcpServer::start(int eventLoopNum) {
    for (int i = 0; i < eventLoopNum; i++) {
        auto it = std::make_unique<EventLoop>();
        event_loops_.push_back(std::move(it));
    }
    for (int i = 0; i < eventLoopNum; i++) {
        event_loops_[i]->start();
    }
    INFO_LOG("server  start io thread count ={}", eventLoopNum);
    uv_run(&main_loop, UV_RUN_DEFAULT);
}

AthenaTcpServer &AthenaTcpServer::bind(int port) {
    // 主Reactor监听
    uv_loop_init(&main_loop);

    uv_tcp_init(&main_loop, &server);
    server.data = this;

    sockaddr_in addr;
    uv_ip4_addr("0.0.0.0", port, &addr);
    uv_tcp_bind(&server, reinterpret_cast<const sockaddr *>(&addr), 0);
    uv_listen((uv_stream_t *) &server, 128, uv_on_new_connection);
    INFO_LOG("server  bind socket port ={}", port);
    return *this;
}

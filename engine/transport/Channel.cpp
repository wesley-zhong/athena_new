//
// Created by zhongweiqi on 2025/10/23.
//

#include "Channel.h"
#include <sstream>
#include "XLog.h"
#include "EventLoop.h"
std::string Channel::getAddr() const {
    std::ostringstream ss;
    struct sockaddr_storage addr;
    int addr_len = sizeof(addr);
    char ip[INET6_ADDRSTRLEN] = {0};
    int port = 0;
    uv_tcp_getpeername(client, (struct sockaddr *) &addr, &addr_len);
    // 提取 IP 和端口
    if (addr.ss_family == AF_INET) {
        struct sockaddr_in *addr4 = (struct sockaddr_in *) &addr;
        uv_ip4_name(addr4, ip, sizeof(ip));
        port = ntohs(addr4->sin_port);
        ss << ip << ":" << port;
    } else if (addr.ss_family == AF_INET6) {
        struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *) &addr;
        uv_ip6_name(addr6, ip, sizeof(ip));
        port = ntohs(addr6->sin6_port);
        ss << ip << ":" << port;
    } else {
        fprintf(stderr, "Unknown address family\n");
    }
    return ss.str();
}


void Channel::send(void *data, size_t size) {
    INFO_LOG("*********  send data size ={} ", size);
    auto *req = new uv_write_t;
    uv_buf_t b = uv_buf_init((char*)data, size);
    req->data = data;
    uv_write(req, (uv_stream_t *) client, &b, 1,
             [](uv_write_t *req1, int status) {
                 INFO_LOG(" ------------write complete call back ={}", status);
                 delete  req1->data;
                 delete req1;
             });
}

void Channel::onRead(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    INFO_LOG("MMMMMMMMMM  READ ={}", nread);
    if (nread > 0) {
        recv_buffer->writeBytes(buf->base, nread);
        // std::string msg(buf->base, nread);
        // delete[] buf->base;
        //
        //  char*   body  =(char *) malloc(6);
        //  body[0]='a';
        //  body[1]='b';
        // send((void*)body, 2);
        return;
    }
    INFO_LOG("socket closed =");
    _eventLoop->onClosed(this);
    uv_close((uv_handle_t *) client, nullptr);
}


//
// Created by zhongweiqi on 2025/10/23.
//

#include "Channel.h"
#include <sstream>
#include "XLog.h"

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
        ss << ip << ":" << port;
        port = ntohs(addr4->sin_port);
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

}

void Channel::onRead(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    INFO_LOG("MMMMMMMMMM  READ ={}",nread);
    if (nread > 0) {
        std::string msg(buf->base, nread);
        delete[] buf->base;
        return;
    }
    INFO_LOG("socket closed =");
    uv_close((uv_handle_t *) client, nullptr);
}


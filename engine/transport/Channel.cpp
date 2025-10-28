//
// Created by zhongweiqi on 2025/10/23.
//

#include "Channel.h"
#include <sstream>
#include "XLog.h"
#include "EventLoop.h"

void Channel::onRead(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    if (nread > 0) {
        recv_buffer->writeBytes(buf->base, nread);
        return;
    }
    _eventLoop->onClosed(this);
    uv_close((uv_handle_t *) client, nullptr);
}

void Channel::sendMsg(int msgId, std::shared_ptr<google::protobuf::Message> msg) {
    _eventLoop->push([this, msgId, msg]() {
        std::string body = msg->SerializeAsString();
        this->eventLoopDoSend(msgId, (char *) body.c_str(), body.size());
    });
    _eventLoop->async_write_task();
}


void Channel::sendMsg(int msgId, char *body, size_t size) {
    _eventLoop->push([this, msgId, body, size]() {
        this->eventLoopDoSend(msgId, body, size);
    });
    _eventLoop->async_write_task();
}


void Channel::eventLoopUvSend(void *data, size_t size) {
    INFO_LOG("*********  send data size ={} ", size);
    auto *req = new uv_write_t;
    uv_buf_t b = uv_buf_init((char *) data, size);
    req->data = data;
    uv_write(req, (uv_stream_t *) client, &b, 1,
             [](uv_write_t *req1, int status) {
                 INFO_LOG(" ------------write complete call back ={}", status);
                 delete req1->data;
                 delete req1;
             });
}


void Channel::eventLoopDoSend(int msgId, char *body, int32 bodyLen) {
    int msgLen = 4 + bodyLen;
    send_buff->writeInt32(msgLen);
    send_buff->writeInt32(msgId);
    send_buff->writeBytes(body, msgLen);

    while (true) {
        size_t outLen = 0;
        const uint8_t *readTail = send_buff->storage().linearReadablePtr(&outLen);
        if (outLen == 0) {
            return;
        }
        this->eventLoopUvSend((void *) readTail, outLen);
        send_buff->storage().consume(outLen);
    }
}


std::string Channel::getAddr() {
    struct sockaddr_storage addr;
    int addr_len = sizeof(addr);
    uv_tcp_getpeername(client, (struct sockaddr *) &addr, &addr_len);
    std::string right = getAddrString(addr);
    uv_tcp_getsockname(client, (struct sockaddr *) &addr, &addr_len);
    std::string left = getAddrString(addr);
    return "[L:/" + left + " - R:/" + right + "]";
}

std::string Channel::getAddrString(const struct sockaddr_storage &addr) {
    std::ostringstream ss;

    char ip[INET6_ADDRSTRLEN] = {0};
    int port = 0;

    // 提取 IP 和端口
    if (addr.ss_family == AF_INET) {
        struct sockaddr_in *addr4 = (struct sockaddr_in *) &addr;
        uv_ip4_name(addr4, ip, sizeof(ip));
        port = Endian::fromNetwork16(addr4->sin_port);
        ss << ip << ":" << port;
    } else if (addr.ss_family == AF_INET6) {
        struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *) &addr;
        uv_ip6_name(addr6, ip, sizeof(ip));
        port = Endian::fromNetwork16(addr6->sin6_port);
        ss << ip << ":" << port;
    } else {
        fprintf(stderr, "Unknown address family\n");
    }
    return ss.str();
}

//
// Created by zhongweiqi on 2025/10/23.
//

#ifndef ATHENA_EVENTLOOP_H
#define ATHENA_EVENTLOOP_H

#include <map>
#include <mutex>

#include "Channel.h"
#include "uv.h"
#include "ThreadPool.h"
#include "XLog.h"

class EventLoop {
public:
    EventLoop() {
    }

    ~EventLoop() {
    }

    void add(std::unique_ptr<Channel> &channel) {
        channels.insert({channel->getFd(), std::move(channel)});
    }

    void asyncAccept(uv_os_sock_t client);

    void execute();

    void push(std::function<void()> func) {
        auto it = Thread::RunTask::create(func);
        _waitTasks.push(it);
        INFO_LOG("--------  push task");
    }

    Thread::TaskPtr pop() {
        Thread::TaskPtr run_task;
        bool ret = _waitTasks.tryPop(run_task);
        if (ret) {
            return run_task;
        }
        return nullptr;
    }

    void run();

    void start();

    void async_write_task() {
        uv_async_send(&uv_async_write);
    }

    void async_accept_task() {
        uv_async_send(&uv_async_accept);
    }


    uv_loop_t *uv_loop() {
        return _loop;
    }

    static void uv_alloc_cb(uv_handle_t *h, size_t s, uv_buf_t *buf);

    static void uv_read_cb(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf);

private:
    uv_loop_t *_loop;
    uv_async_t uv_async_accept; // not used in this sample (accept in main thread)
    uv_async_t uv_async_write; // used by biz threads to notify reactor for pending writes
    std::mutex write_mtx;
    uv_async_t async;
    std::unordered_map<uint64, std::unique_ptr<Channel> > channels;
    TQueue<Thread::TaskPtr> _waitTasks;
    std::thread t;
};

#endif //ATHENA_EVENTLOOP_H

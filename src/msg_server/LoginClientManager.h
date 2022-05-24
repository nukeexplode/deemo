#pragma once
#include <atomic>

#include <muduo/net/EventLoopThread.h>
#include <muduo/net/EventLoopThreadPool.h>
#include <muduo/net/EventLoop.h>

#include "ConfigFileReader.h"

using muduo::net::EventLoopThread;
using muduo::net::EventLoopThreadPool;
using muduo::net::EventLoop;

class ProxyClient;

class LoginClientManager {
public:
    LoginClientManager();
    virtual ~LoginClientManager();

    int init();

    ProxyClient* getClientPtr();

private:
    std::string m_name;
    std::vector<std::unique_ptr<ProxyClient>> m_clients;
    std::unique_ptr<EventLoopThreadPool> m_pool;
    uint32_t m_conn_cnt;
    uint32_t m_thread_cnt;
    EventLoop* m_loop;
    std::atomic_int rank;
};
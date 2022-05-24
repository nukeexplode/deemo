// #pragma once
#ifndef DEEMO_MSGCLIENTPOOL__H_
#define DEEMO_MSGCLIENTPOOL__H_
#include <random>
#include <atomic>

#include <muduo/net/EventLoopThread.h>
#include <muduo/net/EventLoopThreadPool.h>
#include <muduo/net/EventLoop.h>

#include "ConfigFileReader.h"

using muduo::net::EventLoopThread;
using muduo::net::EventLoopThreadPool;
using muduo::net::EventLoop;

class ProxyClient;

class MsgClientPool {
public:
    MsgClientPool();
    virtual ~MsgClientPool();

    int init(std::string name, std::string ip, uint16_t port, int conncnt, int threadcnt);

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
#endif
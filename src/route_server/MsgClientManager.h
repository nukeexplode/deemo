#pragma once
#include <random>
#include <atomic>
#include <string>

#include <muduo/net/EventLoopThread.h>
#include <muduo/net/EventLoopThreadPool.h>
#include <muduo/net/EventLoop.h>

#include "ConfigFileReader.h"
#include "MsgClientPool.h"

using muduo::net::EventLoopThread;
using muduo::net::EventLoopThreadPool;
using muduo::net::EventLoop;

class ProxyClient;

class MsgClientManager {
public:
    MsgClientManager();
    virtual ~MsgClientManager();

    int init();

    ProxyClient* getClientPtr(const char* msg_server);

private:
    std::map<std::string, std::unique_ptr<MsgClientPool>> m_pool_map;
};
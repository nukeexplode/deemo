#pragma once
#include <memory>
#include <functional>
#include <mutex>

#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <muduo/base/Logging.h>

#include "ProxySession.h"
#include "MysqlManager.h"
#include "RedisManager.h"

using namespace muduo;
using namespace muduo::net;

class ProxyServer final{
public:
    ProxyServer();
    ~ProxyServer() = default;

    ProxyServer(const ProxyServer& ) = delete;
    ProxyServer& operator=(const ProxyServer& ) = delete;

    bool init(const char* ip, uint16_t port, EventLoop* loop);

private:
    void onConnected(const std::shared_ptr<TcpConnection>& conn);
    void onDisconnected(const std::shared_ptr<TcpConnection>& conn);

private:
    std::unique_ptr<TcpServer> m_tcp_server;
    std::mutex m_mutex;
    std::list<std::shared_ptr<ProxySession>> m_sessions;
    std::atomic_int m_session_id;
};
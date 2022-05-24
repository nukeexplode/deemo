#pragma once
#include <memory>
#include <functional>
#include <mutex>
#include <unordered_map>
#include <atomic>
#include <list>

#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <muduo/base/Logging.h>

#include "RouteSession.h"
#include "codec.h"
#include "dispatcher.h"
#include "ConcurrentHashmap.h"

using muduo::net::TcpConnection;
using muduo::net::TcpServer;
using muduo::net::EventLoop;
using muduo::net::InetAddress;
using muduo::net::TcpConnectionPtr;

class RouteServer final {
public:
    RouteServer();
    ~RouteServer() = default;

    RouteServer(const RouteServer& ) = delete;
    RouteServer& operator=(const RouteServer& ) = delete;

    bool init(const char* ip, uint16_t port, EventLoop* loop);


private:
    void onConnected(const std::shared_ptr<TcpConnection>& conn);
    void onDisconnected(const std::shared_ptr<TcpConnection>& conn);
    void onUnknownMessage(const TcpConnectionPtr& conn, const MessagePtr& message, Timestamp)
    {
        LOG_INFO << "onUnknownMessage: " << message->GetTypeName();
        conn->shutdown();
    }

private:
    std::unique_ptr<TcpServer> m_tcp_server;
    ConcurrentHashmap<uint32_t, std::shared_ptr<RouteSession>> m_sessions;
    std::atomic_int m_sessionId;

    std::mutex m_mutex;
};
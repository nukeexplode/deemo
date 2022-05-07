// #pragma once
#ifndef DEEMO_REGSERVER__H_
#define DEEMO_REGSERVER__H_
#include <memory>
#include <functional>
#include <mutex>
#include <unordered_map>
#include <atomic>
#include <list>

#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <muduo/base/Logging.h>

#include "RegSession.h"
#include "codec.h"
#include "dispatcher.h"

using muduo::net::TcpConnection;
using muduo::net::TcpServer;
using muduo::net::EventLoop;
using muduo::net::InetAddress;

class RegServer final {
public:
    RegServer();
    ~RegServer() = default;

    RegServer(const RegServer& ) = delete;
    RegServer& operator=(const RegServer& ) = delete;

    bool init(const char* ip, uint16_t port, EventLoop* loop);

    bool getSessionsByUsername(std::shared_ptr<RegSession>& sessions, std::string& name);

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
    std::list<std::shared_ptr<RegSession>> m_sessions;
    std::atomic_int m_sessionId;

    std::mutex m_mutex;
};

#endif
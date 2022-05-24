// #pragma once
#ifndef DEEMO_MsgServer__H_
#define DEEMO_MsgServer__H_
#include <memory>
#include <functional>
#include <mutex>
#include <unordered_map>
#include <atomic>
#include <list>

#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <muduo/base/Logging.h>

#include "MsgSession.h"
#include "codec.h"
#include "dispatcher.h"
#include "ConcurrentHashmap.h"
#include "ConcurrentList.h"

using muduo::net::TcpConnection;
using muduo::net::TcpServer;
using muduo::net::EventLoop;
using muduo::net::InetAddress;

class MsgServer final {
public:
    MsgServer();
    ~MsgServer() = default;

    MsgServer(const MsgServer& ) = delete;
    MsgServer& operator=(const MsgServer& ) = delete;

    bool init(const char* ip, uint16_t port, EventLoop* loop);

    std::string getServerName() {
        return m_server_name;
    }

    bool getSessionsByUserId(std::list<std::shared_ptr<MsgSession>>& sessions, uint32_t user_id);
    bool getSessionBySessionId(std::shared_ptr<MsgSession>& sessions, uint32_t session_id);
    bool getSessionByUserIdAndClientType(std::shared_ptr<MsgSession>& sessions, uint32_t user_id, BaseDefine::ClientType client_type);
    bool ValidSession(uint32_t user_id, std::shared_ptr<MsgSession>& session);
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
    ConcurrentHashmap<uint32_t, ConcurrentList<std::weak_ptr<MsgSession>>> m_userid2session; // useid 2 msgsession
    ConcurrentHashmap<uint32_t, std::weak_ptr<MsgSession>> m_sessionid2session; // sessionid 2 msgsession
    std::atomic_int m_sessionId;
    std::string m_server_name;
};

#endif
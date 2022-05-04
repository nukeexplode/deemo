#pragma once
#include <memory>

#include "User.pb.h"
#include "BaseDefine.pb.h"

#include <muduo/base/Timestamp.h>
#include <muduo/base/Logging.h>
#include <muduo/net/TimerId.h>
#include <muduo/net/EventLoop.h>

#include "TcpSession.h"
#include "Singleton.h"
#include "codec.h"
#include "dispatcher.h"

using Timestamp = muduo::Timestamp;
using TimerId = muduo::net::TimerId;
using EventLoop = muduo::net::EventLoop;

class ProxySession : public TcpSession
{
public:
    ProxySession(const std::shared_ptr<TcpConnection>& conn, const std::string& sessionname, int session_id);
    virtual ~ProxySession();

    ProxySession(const ProxySession& rhs) = delete;
    ProxySession& operator =(const ProxySession& rhs) = delete;

    //无效命令
    void onUnknownMessage(const TcpConnectionPtr& conn, const MessagePtr& message, Timestamp)
    {
        LOG_INFO << "onUnknownMessage: " << message->GetTypeName();
        conn->shutdown();
    }

    void enableHeartbeat();
    void disableHeartbeat();

    //检测心跳包，如果指定时间内（现在是30秒）未收到数据包，则主动断开于客户端的连接
    void checkHeartbeat(const std::shared_ptr<TcpConnection>& conn);

private:
    void handleRegister(const std::shared_ptr<TcpConnection>& conn, const std::shared_ptr<User::RegReq>& req, Timestamp t);

private:
    std::string m_sessionname;
    int32_t m_id;                 //session id

    //heartbeat
    Timestamp m_lastPackageTime;    //上一次收发包的时间
    TimerId m_TimerId;
    int m_interval;
};
// #pragma once
#ifndef DEEMO_MsgSession__H_
#define DEEMO_MsgSession__H_
#include <memory>

#include "User.pb.h"
#include "BaseDefine.pb.h"
#include "Msg2Proxy.pb.h"
#include "Msg2Route.pb.h"

#include <muduo/base/Timestamp.h>
#include <muduo/base/Logging.h>
#include <muduo/net/TimerId.h>
#include <muduo/net/EventLoop.h>

#include "TcpSession.h"
#include "Singleton.h"
#include "ProxyClientManager.h"
#include "LoginClientManager.h"
#include "RouteClientManager.h"
#include "codec.h"
#include "dispatcher.h"

using Timestamp = muduo::Timestamp;
using TimerId = muduo::net::TimerId;
using EventLoop = muduo::net::EventLoop;

class MsgSession : public TcpSession
{
public:
    MsgSession(const std::shared_ptr<TcpConnection>& conn,  const std::string& sessionname, int32_t m_id);
    virtual ~MsgSession();

    MsgSession(const MsgSession& rhs) = delete;
    MsgSession& operator =(const MsgSession& rhs) = delete;

    //有数据可读, 会被多个工作loop调用
    void onRead(const std::shared_ptr<TcpConnection>& conn, Buffer* pBuffer, Timestamp receivTime);
    
    //无效命令
    void onUnknownMessage(const TcpConnectionPtr& conn, const MessagePtr& message, Timestamp)
    {
        LOG_INFO << "onUnknownMessage: " << message->GetTypeName();
        conn->shutdown();
    }

    //让Session失效，用于被踢下线的用户的session
    void makeSessionInvalid();
    bool isSessionValid();

    void enableHeartbeat();
    void disableHeartbeat();

    //检测心跳包，如果指定时间内（现在是30秒）未收到数据包，则主动断开于客户端的连接
    void checkHeartbeat(const std::shared_ptr<TcpConnection>& conn);

    std::string getUsername() {
        return m_userinfo.user_name();
    }

    uint32_t getUserid() {
        return m_userinfo.user_id();
    }

    uint32_t getSessionid() {
        return m_id;
    }

    BaseDefine::ClientType getClientType() {
        std::lock_guard<std::mutex> lock(m_userinfo_mutex);
        return m_userinfo.client_type();
    }

    void setUserinfo(const BaseDefine::UserInfo& user_info) {
        std::lock_guard<std::mutex> lock(m_userinfo_mutex);
        m_userinfo = user_info;
    }

    void ValidateSession() {
        m_login = true;
    }

private:
    void MsgSession::handleLogin(const std::shared_ptr<TcpConnection>& conn, const std::shared_ptr<User::LoginReq>& req, Timestamp t);

private:
    void onHeartbeatResponse(const std::shared_ptr<TcpConnection>& conn);
    void onRegisterResponse(const std::string& data, const std::shared_ptr<TcpConnection>& conn);

private:
    int32_t m_id;                 //session id
    std::string m_sessionname;
    Timestamp m_lastPackageTime;    //上一次收发包的时间
    TimerId m_TimerId;
    const int m_interval;
    std::atomic_bool m_login;
    std::atomic_int m_client_type;
};

#endif
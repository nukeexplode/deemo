// #pragma once
#ifndef DEEMO_REGSESSION__H_
#define DEEMO_REGSESSION__H_
#include <memory>

#include "User.pb.h"
#include "BaseDefine.pb.h"

#include <muduo/base/Timestamp.h>
#include <muduo/base/Logging.h>
#include <muduo/net/TimerId.h>
#include <muduo/net/EventLoop.h>

#include "TcpSession.h"
#include "Singleton.h"
#include "ProxyClientManager.h"
#include "codec.h"
#include "dispatcher.h"

using Timestamp = muduo::Timestamp;
using TimerId = muduo::net::TimerId;
using EventLoop = muduo::net::EventLoop;

class RegSession : public TcpSession
{
public:
    RegSession(const std::shared_ptr<TcpConnection>& conn,  const std::string& sessionname, int32_t m_id);
    virtual ~RegSession();

    RegSession(const RegSession& rhs) = delete;
    RegSession& operator =(const RegSession& rhs) = delete;

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
        return m_username;
    }

private:
    void handleRegister(const std::shared_ptr<TcpConnection>& conn, const std::shared_ptr<User::RegReq>& req, Timestamp t);

private:
    void onHeartbeatResponse(const std::shared_ptr<TcpConnection>& conn);
    void onRegisterResponse(const std::string& data, const std::shared_ptr<TcpConnection>& conn);

private:
    std::string m_username;
    std::string m_sessionname;

    int32_t m_id;                 //session id
    Timestamp m_lastPackageTime;    //上一次收发包的时间
    TimerId m_TimerId;
    int m_interval;
};

#endif
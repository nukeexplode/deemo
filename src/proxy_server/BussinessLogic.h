#pragma once
#include <list>
#include <memory>

#include "Msg2Proxy.pb.h"

#include <muduo/net/TcpConnection.h>
#include <muduo/base/Timestamp.h>

#include "Singleton.h"
#include "MysqlManager.h"
#include "RedisManager.h"
#include "ProxySession.h"
#include "UserManager.h"

class BussinessLogic final
{
public:
    BussinessLogic();
    ~BussinessLogic();

    BussinessLogic(const BussinessLogic& rhs) = delete;
    BussinessLogic& operator =(const BussinessLogic& rhs) = delete;

public:
    Msg2Proxy::LoginRsp handleLogin(const std::shared_ptr<muduo::net::TcpConnection>& conn, const std::shared_ptr<Msg2Proxy::LoginReq>& req, muduo::Timestamp time);

private:
    std::list<muduo::Timestamp> lsErrorTime;
};
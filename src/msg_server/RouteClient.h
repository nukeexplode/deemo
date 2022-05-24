#pragma once
#include <mutex>
#include <functional>
#include <thread>
#include <string>

#include "BaseDefine.pb.h"
#include "User.pb.h"

#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpClient.h>
#include <muduo/base/Logging.h>
#include <muduo/base/Timestamp.h>
#include <muduo/net/Buffer.h>
#include <muduo/net/TcpConnection.h>

#include "RegServer.h"
#include "Singleton.h"
#include "dispatcher.h"
#include "codec.h"

using muduo::net::EventLoop;
using muduo::net::TcpClient;
using muduo::net::InetAddress;
using muduo::net::TcpConnection;   
using muduo::net::TcpConnectionPtr;
using muduo::net::ConnectionCallback;
using muduo::net::MessageCallback;
using muduo::net::CloseCallback;
using muduo::net::Buffer;
using muduo::Timestamp;

class RouteClient
{
public:
    RouteClient(EventLoop* loop, const InetAddress& serverAddr)
    :m_client(loop, serverAddr, "RouteClient")
    {
        m_codec.setMessageCallback(std::bind(&ProtobufDispatcher::onProtobufMessage, &m_dispatcher, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

        m_dispatcher.setDefaultCallback(std::bind(&RouteClient::onUnknownMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        m_dispatcher.registerMessageCallback<User::RegRsp>(std::bind(&RouteClient::handleRegister, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

        m_client.setConnectionCallback(std::bind(&RouteClient::onConnection, this, std::placeholders::_1));
        m_client.setMessageCallback(std::bind(&ProtobufCodec::onMessage, &m_codec, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        m_client.enableRetry();
    }

    void connect()
    {
        m_client.connect();
    }

    void disconnect()
    {
        m_client.disconnect();
    }

    bool isconnect() {
        return m_connected;
    }

    void send(google::protobuf::Message* msg)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_conn)
        {
            m_codec.send(m_conn, *msg);
        }
    }

    // void setConnectionCallback(ConnectionCallback cb) 
    // {
    //     m_connectionCallback = cb;
    // }

    void setMessageCallback(MessageCallback cb) 
    {
        m_messageCallback = cb;
    }

private:
    void onConnection(const std::shared_ptr<TcpConnection>& conn) 
    {
        LOG_INFO << conn->localAddress().toIpPort() << " -> "
                 << conn->peerAddress().toIpPort() << " is "
                 << (conn->connected() ? "UP" : "DOWN");

        std::unique_lock<std::mutex> lock(m_mutex);
        if (conn->connected()) {
            m_conn = conn;
            m_connected = true;
        } else {
            m_conn.reset();
            m_connected = false;
        }
    }

    void onUnknownMessage(const TcpConnectionPtr&, const MessagePtr& message, Timestamp)
    {
        LOG_INFO << "onUnknownMessage: " << message->GetTypeName();
    }
private:
    void handleRegister(const TcpConnectionPtr& conn, const std::shared_ptr<User::RegRsp>& rsp, Timestamp);

private:
    TcpClient m_client;
    TcpConnectionPtr m_conn;
    std::mutex m_mutex;
    ProtobufCodec m_codec;
    ProtobufDispatcher m_dispatcher;

    bool m_connected;
    ConnectionCallback m_connectionCallback;
    MessageCallback m_messageCallback;
};
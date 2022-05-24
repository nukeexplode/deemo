#include "RouteServer.h"

RouteServer::RouteServer() {
    
}

bool RouteServer::init(const char* ip, uint16_t port, EventLoop* loop) {
    InetAddress addr(ip, port);
    m_tcp_server.reset(new TcpServer(loop, addr, "deemo route server", TcpServer::kReusePort));

    m_tcp_server->setConnectionCallback(std::bind(&RouteServer::onConnected, this, std::placeholders::_1));
    // m_tcp_server->setMessageCallback(std::bind(&ProtobufCodec::onMessage, &m_codec, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    // m_codec.setMessageCallback(std::bind(&ProtobufDispatcher::onProtobufMessage, &m_dispatcher, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    // m_dispatcher.setDefaultCallback(std::bind(&RouteServer::onUnknownMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    m_tcp_server->setThreadNum(1);
    m_tcp_server->start();
    return true;
}

void RouteServer::onConnected(const std::shared_ptr<TcpConnection>& conn) {
    if (conn->connected()) {
        char log[255];
        sprintf(log, "reg server connected : %s", conn->peerAddress().toIpPort().c_str());
        LOG_DEBUG << log;

        std::string session_name = "msgserver:" + conn->peerAddress().toIpPort();
        std::shared_ptr<RouteSession> sp_proxysession(std::make_shared<RouteSession>(conn, session_name, m_sessionId++));
        conn->setMessageCallback(std::bind(&ProtobufCodec::onMessage, sp_proxysession->getProtoobufCodec(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

        m_sessions[sp_pro]
    } else {
        onDisconnected(conn);
    }
}

void RouteServer::onDisconnected(const std::shared_ptr<TcpConnection>& conn) {
    
}

bool RouteServer::getSessionsByUsername(std::shared_ptr<RegSession>& session, std::string& name) {
    std::unique_lock<std::mutex> lock(m_mutex);
    for (auto& sp_session : m_sessions) {
        if (sp_session->getUsername() == name) {
            session = sp_session;
            return true;
        }
    }
    
    LOG_ERROR << "can't find user";
    return false;
}
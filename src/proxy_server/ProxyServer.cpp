#include "ProxyServer.h"

ProxyServer::ProxyServer() {

}

bool ProxyServer::init(const char* ip, uint16_t port, EventLoop* loop) {
    InetAddress addr(ip, port);
    m_tcp_server.reset(new TcpServer(loop, addr, "deemo proxy server", TcpServer::kReusePort));
    m_tcp_server->setConnectionCallback(std::bind(&ProxyServer::onConnected, this, std::placeholders::_1));
    m_tcp_server->setThreadNum(1);
    m_tcp_server->start();

    return true;
}

void ProxyServer::onConnected(const std::shared_ptr<TcpConnection>& conn) {
    if (conn->connected()) {
        char log[255];
        sprintf(log, "msgserver connected : %s", conn->peerAddress().toIpPort().c_str());
        LOG_DEBUG << log;

        std::string session_name = "msgserver:" + conn->peerAddress().toIpPort();
        std::shared_ptr<ProxySession> sp_proxysession(std::make_shared<ProxySession>(conn, session_name, m_session_id++));
        conn->setMessageCallback(std::bind(&ProtobufCodec::onMessage, sp_proxysession->getProtoobufCodec(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

        std::lock_guard<std::mutex> lock(m_mutex);
        m_sessions.push_back(sp_proxysession);
    } else {
        onDisconnected(conn);
    }
}

void ProxyServer::onDisconnected(const std::shared_ptr<TcpConnection>& conn) {
    
}
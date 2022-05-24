#include "MsgServer.h"

MsgServer::MsgServer() {

}

bool MsgServer::init(const char* ip, uint16_t port, EventLoop* loop) {
    InetAddress addr(ip, port);
    m_tcp_server.reset(new TcpServer(loop, addr, "deemo msg server", TcpServer::kReusePort));

    m_tcp_server->setConnectionCallback(std::bind(&MsgServer::onConnected, this, std::placeholders::_1));
    // m_tcp_server->setMessageCallback(std::bind(&ProtobufCodec::onMessage, &m_codec, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    // m_codec.setMessageCallback(std::bind(&ProtobufDispatcher::onProtobufMessage, &m_dispatcher, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    // m_dispatcher.setDefaultCallback(std::bind(&RegServer::onUnknownMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    m_server_name = "msgserver1";
    m_tcp_server->setThreadNum(1);
    m_tcp_server->start();
    return true;
}

//sessions must be local variable
bool MsgServer::getSessionsByUserId(std::list<std::shared_ptr<MsgSession>>& sessions, uint32_t user_id) {
    if (m_userid2session.count(user_id) == false) {
        return false;
    }
    
    {
        std::shared_lock<std::shared_mutex> lock(m_userid2session[user_id].m_mutex);
        for (auto wp_session : m_userid2session[user_id].vector) {
            std::shared_ptr<MsgSession> sp_session = wp_session.lock();
            if (sp_session != nullptr) {
                sessions.push_back(sp_session);
            }
        }
    }

    return !sessions.empty();
}

bool MsgServer::getSessionBySessionId(std::shared_ptr<MsgSession>& session, uint32_t session_id) {
    if (m_sessionid2session.count(session_id) == false) {
        return false;
    }
    
    std::shared_ptr<MsgSession> sp_msg_session = m_sessionid2session[session_id].lock();
    if (sp_msg_session == nullptr) {
        return false;
    }

    session = sp_msg_session;
    return true;
}

bool MsgServer::getSessionByUserIdAndClientType(std::shared_ptr<MsgSession>& sessions, uint32_t user_id, BaseDefine::ClientType client_type) {
    if (m_userid2session.count(user_id) == false) {
        return false;
    }
    
    {
        std::shared_lock<std::shared_mutex> lock(m_userid2session[user_id].m_mutex);
        for (auto wp_session : m_userid2session[user_id].vector) {
            std::shared_ptr<MsgSession> sp_session = wp_session.lock();
            if (sp_session == nullptr) {
                continue;
            }

            if (client_type == sp_session->getClientType()) {
                sessions = sp_session;
                return true;
            }
        }
    }

    return false;
}

bool MsgServer::ValidSession(uint32_t user_id, std::shared_ptr<MsgSession>& session) {
    std::unique_lock<std::shared_mutex> lock(m_userid2session[user_id].m_mutex);
    
    for (auto wp_session : m_userid2session[user_id].vector) {
        std::shared_ptr<MsgSession> sp_session = wp_session.lock();
        if (sp_session == nullptr) {
            continue;
        }

        if (session->getClientType() == sp_session->getClientType()) {
            LOG_ERROR << "为什么会有两个相同客户端的类型";
            return false;
        }
    }

    m_userid2session[user_id].vector.push_back(session);
    return true;
}

void MsgServer::onConnected(const std::shared_ptr<TcpConnection>& conn) {
    if (conn->connected()) {
        char log[255];
        sprintf(log, "msg server connected : %s", conn->peerAddress().toIpPort().c_str());
        LOG_DEBUG << log;

        std::string session_name = "msgserver:" + conn->peerAddress().toIpPort();
        std::shared_ptr<MsgSession> sp_msgsession(std::make_shared<MsgSession>(conn, session_name, m_sessionId++));
        conn->setMessageCallback(std::bind(&ProtobufCodec::onMessage, sp_msgsession->getProtoobufCodec(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

        m_sessionid2session[sp_msgsession->getSessionid()] = sp_msgsession;
    } else {
        onDisconnected(conn);
    }
}
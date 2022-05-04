#include "RegSession.h"

#include "ProxyClient.h"

RegSession::RegSession(const std::shared_ptr<TcpConnection>& conn, const std::string& sessionname, int sessionid) : TcpSession(conn), m_sessionname(sessionname), m_id(sessionid) {
    m_lastPackageTime = Timestamp(time(NULL));
    m_interval = 15;
    m_dispatcher.setDefaultCallback(std::bind(&RegSession::onUnknownMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    m_dispatcher.registerMessageCallback<User::RegReq>(std::bind(&RegSession::handleRegister, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    m_codec.setMessageCallback(std::bind(&ProtobufDispatcher::onProtobufMessage, &m_dispatcher, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    enableHeartbeat();
}

RegSession::~RegSession() {
    std::shared_ptr<TcpConnection> conn = getConnectionPtr();
    if (conn)
    {
        char log[255];
        sprintf(log, "register session offline : %s", conn->peerAddress().toIpPort().c_str());
        LOG_INFO << log;
    }

    disableHeartbeat();
}

void RegSession::enableHeartbeat() {
    std::shared_ptr<TcpConnection> conn = getConnectionPtr();
    if (conn)
    {
        //每15秒钟检测一下是否有掉线现象
        conn->getLoop()->assertInLoopThread();
        m_TimerId = conn->getLoop()->runEvery(m_interval * 1000 * 1000, std::bind(&RegSession::checkHeartbeat, this, conn));
    }
}

void RegSession::disableHeartbeat() {
    std::shared_ptr<TcpConnection> conn = getConnectionPtr();
    if (conn)
    {
        char log[255];
        sprintf(log, "remove check online timerId, client address: %s\n", conn->peerAddress().toIpPort().c_str());
        LOG_INFO << log;

        conn->getLoop()->cancel(m_TimerId);
    }
}

void RegSession::checkHeartbeat(const std::shared_ptr<TcpConnection>& conn) {
    if (!conn) {
        return;
    }
    
    Timestamp time = muduo::addTime(m_lastPackageTime, m_interval);
    if (Timestamp::now() < time) {
        return;
    }

    conn->forceClose();
}

// void RegSession::onRead(const std::shared_ptr<TcpConnection>& conn, Buffer* pBuffer, Timestamp receivTime) {
//     while (true) {
//         if (pBuffer->readableBytes() < sizeof(chat_msg_header)) {
//             return;
//         }

//         chat_msg_header header;
//         header.length = pBuffer->peekInt32();
//         pBuffer->retrieveInt32();
//         header.reversed = pBuffer->peekInt32();
//         pBuffer->retrieveInt32();

//         if (pBuffer->readableBytes() < header.length) {
//             return;
//         }
        
//         IM::Msg msg;
//         if (!msg.ParseFromArray(pBuffer->peek(), pBuffer->readableBytes())) {
//             LOG_ERROR << "protobuf parse failed";
//             conn->forceClose();
//             return;
//         }

//         if (process(conn, msg) != true) {
//             char log[255];
//             sprintf(log, "Process error, close TcpConnection, client: %s", conn->peerAddress().toIpPort().c_str());
//             LOG_ERROR << log;
//             conn->forceClose();
//             return;
//         }

//         m_lastPackageTime = Timestamp::now();
//     }
// }

// bool RegSession::process(const std::shared_ptr<TcpConnection>& conn, IM::Msg& msg) {
//     int sid = msg.sid();
//     switch (sid) 
//     {
//         case BaseDefine::ServiceID::SID_REGISTER:
//             handleRegister(conn, msg);
//             break;
//         default:
//             return false;
//     }
    
//     return true;
// }

// void RegSession::handleRegister(const std::shared_ptr<TcpConnection>& conn, IM::Msg& msg) {
//     int cmd = msg.cmd();
//     switch (cmd)
//     {
//         case BaseDefine::RegCmdID::CID_REGISTER_REQ_USEREG:
//             _handleRegister(conn, msg);
//             break;
        
//         default:
//             break;
//     }
// }

void RegSession::handleRegister(const std::shared_ptr<TcpConnection>& conn, const std::shared_ptr<User::RegReq>& req, Timestamp t) {
    std::string username = req->username();

    m_username = username;

    ProxyClient* client = Singleton<ProxyClientManager>::Instance().getClientPtr();
    client->send(req.get());
}
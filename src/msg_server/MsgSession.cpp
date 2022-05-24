#include "MsgSession.h"

#include "ProxyClient.h"

MsgSession::MsgSession(const std::shared_ptr<TcpConnection>& conn, const std::string& sessionname, int sessionid) : TcpSession(conn), m_sessionname(sessionname), 
m_id(sessionid), m_interval(15), m_login(false) {
    m_lastPackageTime = Timestamp(time(NULL));
    m_dispatcher.setDefaultCallback(std::bind(&MsgSession::onUnknownMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    m_dispatcher.registerMessageCallback<User::LoginReq>(std::bind(&MsgSession::handleLogin, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    m_codec.setMessageCallback(std::bind(&ProtobufDispatcher::onProtobufMessage, &m_dispatcher, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    enableHeartbeat();
}

MsgSession::~MsgSession() {
    std::shared_ptr<TcpConnection> conn = getConnectionPtr();
    if (conn)
    {
        char log[255];
        sprintf(log, "register session offline : %s", conn->peerAddress().toIpPort().c_str());
        LOG_INFO << log;
    }

    disableHeartbeat();
}

void MsgSession::enableHeartbeat() {
    std::shared_ptr<TcpConnection> conn = getConnectionPtr();
    if (conn)
    {
        //每15秒钟检测一下是否有掉线现象
        conn->getLoop()->assertInLoopThread();
        m_TimerId = conn->getLoop()->runEvery(m_interval * 1000 * 1000, std::bind(&MsgSession::checkHeartbeat, this, conn));
    }
}

void MsgSession::disableHeartbeat() {
    std::shared_ptr<TcpConnection> conn = getConnectionPtr();
    if (conn)
    {
        char log[255];
        sprintf(log, "remove check online timerId, client address: %s\n", conn->peerAddress().toIpPort().c_str());
        LOG_INFO << log;

        conn->getLoop()->cancel(m_TimerId);
    }
}

void MsgSession::checkHeartbeat(const std::shared_ptr<TcpConnection>& conn) {
    if (!conn) {
        return;
    }
    
    Timestamp time = muduo::addTime(m_lastPackageTime, m_interval);
    if (Timestamp::now() < time) {
        return;
    }

    conn->forceClose();
}

// void MsgSession::onRead(const std::shared_ptr<TcpConnection>& conn, Buffer* pBuffer, Timestamp receivTime) {
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

// bool MsgSession::process(const std::shared_ptr<TcpConnection>& conn, IM::Msg& msg) {
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

// void MsgSession::handleRegister(const std::shared_ptr<TcpConnection>& conn, IM::Msg& msg) {
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

void MsgSession::handleLogin(const std::shared_ptr<TcpConnection>& conn, const std::shared_ptr<User::LoginReq>& req, Timestamp t) {
    ProxyClient* client = Singleton<ProxyClientManager>::Instance().getClientPtr();
    if (client == nullptr) {
        User::LoginRsp rsp;
        rsp.set_server_time(time(NULL));
        rsp.set_result_code(BaseDefine::ResultType::SERVER_NO_PROXY_SERVER);
        rsp.set_result_string("没有代理服务器");

        send(&rsp);
        LOG_ERROR << "没有代理服务器";
        return;
    }
}
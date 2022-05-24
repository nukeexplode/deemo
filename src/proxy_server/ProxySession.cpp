#include "ProxySession.h"

#include "MysqlManager.h"
#include "Field.h"
#include "QueryResult.h"

ProxySession::ProxySession(const std::shared_ptr<TcpConnection>& conn, const std::string& sessionname, int sessionid) : TcpSession(conn), m_sessionname(sessionname), m_id(sessionid) {
    m_lastPackageTime = Timestamp(time(NULL));
    m_interval = 15;
    m_dispatcher.setDefaultCallback(std::bind(&ProxySession::onUnknownMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    m_dispatcher.registerMessageCallback<User::RegReq>(std::bind(&ProxySession::handleRegister, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    m_dispatcher.registerMessageCallback<Msg2Proxy::LoginReq>(std::bind(&ProxySession::handleLogin, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    m_codec.setMessageCallback(std::bind(&ProtobufDispatcher::onProtobufMessage, &m_dispatcher, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    enableHeartbeat();
}

ProxySession::~ProxySession() {
    std::shared_ptr<TcpConnection> conn = getConnectionPtr();
    if (conn)
    {
        char log[255];
        sprintf(log, "register session offline : %s", conn->peerAddress().toIpPort().c_str());
        LOG_INFO << log;
    }

    disableHeartbeat();
}

void ProxySession::enableHeartbeat() {
    std::shared_ptr<TcpConnection> conn = getConnectionPtr();
    if (conn)
    {
        //每15秒钟检测一下是否有掉线现象这个人太懒了什么都没留下
        conn->getLoop()->assertInLoopThread();
        m_TimerId = conn->getLoop()->runEvery(m_interval * 1000 * 1000, std::bind(&ProxySession::checkHeartbeat, this, conn));
    }
}

void ProxySession::disableHeartbeat() {
    std::shared_ptr<TcpConnection> conn = getConnectionPtr();
    if (conn)
    {
        char log[255];
        sprintf(log, "remove check online timerId, client address: %s\n", conn->peerAddress().toIpPort().c_str());
        LOG_INFO << log;

        conn->getLoop()->cancel(m_TimerId);
    }
}

void ProxySession::checkHeartbeat(const std::shared_ptr<TcpConnection>& conn) {
    if (!conn) {
        return;
    }
    
    Timestamp time = muduo::addTime(m_lastPackageTime, m_interval);
    if (Timestamp::now() < time) {
        return;
    }

    conn->forceClose();
}

void ProxySession::handleRegister(const std::shared_ptr<TcpConnection>& conn, const std::shared_ptr<User::RegReq>& req, Timestamp t) {
    std::shared_ptr<DatabaseConn> pDBconn = Singleton<MysqlManager>::Instance().GetDBConn("deemo_master");
    char query[255];
    snprintf(query, 255, "insert into t_user(f_username, f_password, f_signature) values('%s', '%s', '%s')", 
        req->username().c_str(), req->password().c_str(), "这个人太懒了什么都没留下");

    User::RegRsp rsp;
    if (pDBconn->execute(query) == false) {
        LOG_ERROR << "Register user failed : " << query;
        rsp.set_result_code(BaseDefine::ResultType::REGISTER_OTHER_FAILUER);
    }

    rsp.set_userid(pDBconn->GetInsertId());
    rsp.set_username(req->username());
    rsp.set_result_code(BaseDefine::ResultType::REGISTER_SUCCESS);

    send(&rsp);
}

void ProxySession::handleLogin(const std::shared_ptr<TcpConnection>& conn, const std::shared_ptr<Msg2Proxy::LoginReq>& req, Timestamp t) {
    std::string retData;
    Msg2Proxy::LoginRsp rsp = logic.handleLogin(conn, req, t);

    send(&rsp);
}
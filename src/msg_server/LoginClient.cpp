#include "LoginClient.h"

#include "RouteClientManager.h"
#include "LoginClientManager.h"
#include "MsgSession.h"
#include "MsgServer.h"
#include "Singleton.h"

void LoginClient::handleRegister(const TcpConnectionPtr& conn, const std::shared_ptr<User::RegRsp>& rsp, Timestamp) {
    int res = rsp->result_code();
    if (res != 0) {
        LOG_ERROR << "register failed";
    }

    LOG_INFO << "register: userid: " << rsp->username() << "password: " << rsp->password();

    std::shared_ptr<MsgSession> sp_regsession;
    std::string username = rsp->username();
    if (Singleton<MsgServer>::Instance().getSessionsByUsername(sp_regsession, username) == false) {
        LOG_ERROR << "no session named " << username;
    }
    sp_regsession->send(rsp.get());
}

void LoginClient::handleLogin(const TcpConnectionPtr& conn, const std::shared_ptr<Msg2Proxy::LoginRsp>& rsp, Timestamp) {
    uint32_t session_id = rsp->session_id();
    std::shared_ptr<MsgSession> sp_msg_session;
    if (Singleton<MsgServer>::Instance().getSessionBySessionIdAndClientType(sp_msg_session, session_id) == false) {
        LOG_DEBUG << "user has been already exit. user id " << rsp->user_info().user_id();
        return;
    }

    User::LoginRsp login_rsp;
    login_rsp.set_server_time(time(NULL));
    login_rsp.set_result_code(rsp->result_code());
    login_rsp.set_result_string(rsp->result_string());
    BaseDefine::UserInfo* user_info = login_rsp.mutable_userinfo();
    *user_info = rsp->user_info();
    sp_msg_session->send(&login_rsp);

    
}
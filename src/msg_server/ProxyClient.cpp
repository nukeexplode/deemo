#include "ProxyClient.h"

#include "RouteClient.h"
#include "RouteClientManager.h"
#include "MsgServer.h"
#include "MsgSession.h"
#include "ConcurrentList.h"

void ProxyClient::handleRegister(const TcpConnectionPtr& conn, const std::shared_ptr<User::RegRsp>& rsp, Timestamp) {
    int res = rsp->result_code();
    if (res != 0) {
        LOG_ERROR << "register failed";
    }

    LOG_INFO << "register: userid: " << rsp->username() << "password: " << rsp->password();

    std::shared_ptr<RegSession> sp_regsession;
    std::string username = rsp->username();
    if (Singleton<RegServer>::Instance().getSessionsByUsername(sp_regsession, username) == false) {
        LOG_ERROR << "no session named " << username;
    }
    sp_regsession->send(rsp.get());
}

void ProxyClient::handleLogin(const TcpConnectionPtr& conn, const std::shared_ptr<Msg2Proxy::LoginRsp>& rsp, Timestamp) {
    uint32_t session_id = rsp->session_id();
    std::shared_ptr<MsgSession> sp_msg_session;
    if (Singleton<MsgServer>::Instance().getSessionBySessionId(sp_msg_session, session_id) == false) {
        LOG_DEBUG << "user has been already exit. user id " << rsp->user_info().user_id();
        return;
    }

    User::LoginRsp login_rsp;
    // FIXME 多个类型的客户端读到的消息的顺序可能会不同
    login_rsp.set_server_time(time(NULL));
    login_rsp.set_result_code(rsp->result_code());
    login_rsp.set_result_string(rsp->result_string());

    if (rsp->result_code() != BaseDefine::ResultType::LOGIN_SUCCESS) {
        LOG_DEBUG << "user id: " << sp_msg_session->getUserid() << " login failed";
        sp_msg_session->send(&login_rsp);
        return;
    }

    BaseDefine::UserInfo* user_info = login_rsp.mutable_userinfo();
    *user_info = rsp->user_info();
    sp_msg_session->send(&login_rsp);

    //multi user client type login
    std::shared_ptr<MsgSession> session;
    assert(Singleton<MsgServer>::Instance().getSessionByUserIdAndClientType(session, user_info->user_id(), user_info->client_type()) == true);
    User::KickUser kick_commmd;
    if (session != nullptr) {
        kick_commmd.set_user_id(user_info->user_id());
        kick_commmd.set_client_type(user_info->client_type());
        kick_commmd.set_result_code(BaseDefine::LOGIN_KICK_USER);
        kick_commmd.set_kick_result_string("有另一个相同类型的客户端登录了");
        session->send(&kick_commmd);
        conn->forceClose();
    }

    //TODO send notify msg

    //TODO send offline message

    Msg2Route::LoginRsp route_login_rsp;
    route_login_rsp.set_user_id(rsp->user_info().user_id());
    route_login_rsp.set_client_type(rsp->user_info().client_type());
    route_login_rsp.set_server_name(Singleton<MsgServer>::Instance().getServerName());
    RouteClient* route_client = Singleton<RouteClientManager>::Instance().getClientPtr();
    route_client->send(&route_login_rsp);
}
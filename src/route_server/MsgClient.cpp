#include "MsgClient.h"

void MsgClient::handleKickUser(const TcpConnectionPtr& conn, const std::shared_ptr<User::KickUser>& rsp, Timestamp) {
    int res = rsp->result_code();
    if (res != 0) {
        LOG_ERROR << "register failed";
    }

    LOG_INFO << "register: userid: " << rsp->user_id();

    std::shared_ptr<RegSession> sp_regsession;
    uint32_t user_id = rsp->user_id();
    if (Singleton<RegServer>::Instance().getSessionsByUserId(sp_regsession, user_id) == false) {
        LOG_ERROR << "no session named " << username;
    }
    sp_regsession->send(rsp.get());
}

void MsgClient::handleLoginRsp(const TcpConnectionPtr& conn, const std::shared_ptr<User::LoginRsp>& rsp, Timestamp) {
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
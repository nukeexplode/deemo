#include "ProxyClient.h"

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
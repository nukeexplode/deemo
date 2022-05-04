#include "RegClient.h"

void RegClient::handleRegister(const TcpConnectionPtr& conn, const std::shared_ptr<User::RegRsp>& rsp, Timestamp) {
    std::string username = rsp->username();
    uint32_t userid = rsp->userid();
    std::string password = rsp->password();
    uint32_t res = rsp->result_code();

    if (res != 0) {
        LOG_INFO << "register failed";
    } else {
        LOG_INFO << "register success: username = " << username << " password = " << password << " userid = " << userid;
    }
}
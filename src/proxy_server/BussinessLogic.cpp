#include "BussinessLogic.h"

using namespace muduo::net;
using namespace muduo;

BussinessLogic::BussinessLogic() {

}

BussinessLogic::~BussinessLogic() {

}

Msg2Proxy::LoginRsp BussinessLogic::handleLogin(const std::shared_ptr<muduo::net::TcpConnection>& conn, const std::shared_ptr<Msg2Proxy::LoginReq>& req, muduo::Timestamp time) {
    Timestamp now = Timestamp::now();
    Msg2Proxy::LoginRsp rsp;

    for (auto iter = lsErrorTime.begin(); iter != lsErrorTime.end(); iter++) {
        if (timeDifference(now, *iter) > 30 * 60) {
            lsErrorTime.pop_front();
        } else {
            break;
        }
    }

    assert(lsErrorTime.size() <= 10);
    if (lsErrorTime.size() == 10) {
        rsp.set_result_code(BaseDefine::ResultType::LOGIN_TOO_MANY_PASSWORD_ERROR);
        rsp.set_result_string("用户名/密码错误次数太多");
        rsp.set_session_id(req->session_id());
        LOG_DEBUG << req->user_id() << "用户名/密码错误次数太多";
        return rsp;
    }
    
    uint32_t user_id = req->user_id();
    std::string password = req->password();
    uint32_t session_id = req->session_id();
    BaseDefine::UserInfo* user_info = rsp.mutable_user_info();
    BaseDefine::ResultType res_type;
    std::string str;
    if ((res_type = UserManager::verifyUser(req->user_id(), req->password(), user_info, str)) != BaseDefine::ResultType::LOGIN_SUCCESS) {
        lsErrorTime.push_back(time);
        LOG_DEBUG << "userid " << user_id << " password " << password << " error: " << str;

        rsp.set_session_id(session_id);
        rsp.set_result_code(res_type);
        rsp.set_result_string(str);
        LOG_DEBUG << str;
        return rsp;
    }

    rsp.set_session_id(session_id);
    rsp.set_result_code(BaseDefine::ResultType::LOGIN_SUCCESS);
    rsp.set_result_string(str);
    //TODO save friend list
    return rsp;
}
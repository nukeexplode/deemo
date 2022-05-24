#include "UserManager.h"

BaseDefine::ResultType UserManager::verifyUser(uint32_t user_id, std::string password, BaseDefine::UserInfo* user_info, std::string& retData) {
    std::shared_ptr<DatabaseConn> pDBconn = Singleton<MysqlManager>::Instance().GetDBConn("deemo");
    char query[255];
    snprintf(query ,255, "select f_user_id, f_password, f_username, f_signature from t_user");
    std::unique_ptr<QueryResult> pRes = pDBconn->query(query);

    Field* pRow = pRes->fetch();
    if (pRow == NULL) {
        retData = "用户未注册";
        return BaseDefine::ResultType::LOGIN_NO_SUCH_USER;
    }

    int user_id_ = pRow[0].getInt32();
    std::string password_ = pRow[1].getCppString();
    std::string username = pRow[2].getCppString();
    std::string signature = pRow[3].getCppString();

    if (password_ != password) {
        retData = "用户密码错误";
        return BaseDefine::ResultType::LOGIN_PASSWORD_ERROR;
    }

    char log[255];
    sprintf(log, "userid: %d, password: %s", user_id, password.c_str());
    std::cout << log << std::endl;

    assert(!pRes->nextRow() && "两个相同的user_id");
    user_info->set_user_id(user_id_);
    user_info->set_user_name(username);
    user_info->set_signature(signature);

    retData = "用户登录成功";
    return BaseDefine::ResultType::LOGIN_SUCCESS;
}
#include "UserManager.h"

UserManager::UserManager() {

}

UserManager::~UserManager() {

}

void UserManager::AddUser(uint32_t user_id, BaseDefine::ClientType client_type, std::string msg_server_name) {
    RouteServer::UserInfo user_info;
    user_info.set_msg_server_name(msg_server_name);
    user_info.set_client_type(client_type);
    m_map.insert(user_id, user_info);
}
#include <string>

#include "BaseDefine.pb.h"
#include "RouteServer.pb.h"

#include "ConcurrentHashmap.h"
#include "ConcurrentList.h"

class UserManager {
public:
    UserManager();
    ~UserManager();

    void AddUser(uint32_t user_id, BaseDefine::ClientType client_type, std::string msg_server_name);
private:
    ConcurrentHashmap<uint32_t, ConcurrentList<RouteServer::UserInfo>> m_map;
};
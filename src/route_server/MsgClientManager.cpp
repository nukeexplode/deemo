#include "MsgClientManager.h"

#include "ProxyClient.h"

extern EventLoop g_main_loop;

MsgClientManager::MsgClientManager()
{

}

MsgClientManager::~MsgClientManager()
{

}

int MsgClientManager::init() {
    CConfigFileReader config("../etc/routeserver.conf");
    int cnt = std::stoi(config.getConfigName("instance_cnt"));

    for (int i = 1; i <= cnt; i++) {
        std::string name = config.getConfigName(("name" + std::to_string(i)).c_str());
        std::string ip = config.getConfigName(("ip" + std::to_string(i)).c_str());
        uint16_t port = static_cast<uint16_t>(std::stoi(config.getConfigName("port")));
        int conncnt = std::stoi(config.getConfigName(("conncnt" + std::to_string(i)).c_str()));
        int threadcnt = std::stoi(config.getConfigName(("threadcnt" + std::to_string(i)).c_str()));
        
        std::unique_ptr<MsgClientPool> up_pool = std::make_unique<MsgClientPool>();
        up_pool->init(name, ip, port, conncnt, threadcnt);
        m_pool_map[name] = std::move(up_pool);
    }

    return 0;
}

ProxyClient* MsgClientManager::getClientPtr(const char* msg_server_name) {
    assert(m_pool_map.count(msg_server_name) == true);
    return m_pool_map[msg_server_name]->getClientPtr();
}
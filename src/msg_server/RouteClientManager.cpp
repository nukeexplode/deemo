#include "RouteClientManager.h"

#include "RouteClient.h"

extern EventLoop g_main_loop;

RouteClientManager::RouteClientManager() : 
rank(0)
{

}

RouteClientManager::~RouteClientManager()
{

}

int RouteClientManager::init() {
    CConfigFileReader config("../etc/registerserver.conf");

    m_name = config.getConfigName("proxy_name");
    std::string ip = config.getConfigName("proxy_ip");
    uint16_t port = static_cast<uint16_t>(std::stoi(config.getConfigName("proxy_port")));
    m_conn_cnt = std::stoi(config.getConfigName("proxy_conncnt"));
    m_thread_cnt = std::stoi(config.getConfigName("proxy_threadcnt"));
    InetAddress addr(ip, port);

    m_loop = &g_main_loop;  
    m_pool.reset(new EventLoopThreadPool(m_loop, "proxy client pool"));
    m_pool->setThreadNum(m_thread_cnt);
    m_pool->start();

    for (uint32_t i = 0; i < m_conn_cnt; i++) {
        std::unique_ptr<RouteClient> up_routeclient(std::make_unique<RouteClient>(m_pool->getNextLoop(), addr));
        up_routeclient->connect();
        m_clients.push_back(std::move(up_routeclient));
    }

    return 0;
}

RouteClient* RouteClientManager::getClientPtr() {
        uint32_t size = static_cast<uint32_t>(m_clients.size());
        for (uint32_t i = 0; i < size; i++) { 
            uint32_t r = ( rank + i) % size;
            assert(m_clients[r]);
            if (m_clients[r]->isconnect()) {
                rank = r + 1;
                return m_clients[r].get();
            }
        }

        return nullptr;
    }
#include "MsgClientPool.h"

#include "MsgClient.h"

extern EventLoop g_main_loop;

MsgClientPool::MsgClientPool() : 
rank(0)
{

}

MsgClientPool::~MsgClientPool()
{

}

int MsgClientPool::init(std::string name, std::string ip, uint16_t port, int conncnt, int threadcnt) {
    CConfigFileReader config("../etc/routeserver.conf");

    m_name = name;
    std::string ip = ip;
    uint16_t port = port;
    m_conn_cnt = conncnt;
    m_thread_cnt = threadcnt;
    InetAddress addr(ip, port);

    m_loop = &g_main_loop;  
    m_pool.reset(new EventLoopThreadPool(m_loop, "proxy client pool"));
    m_pool->setThreadNum(m_thread_cnt);
    m_pool->start();

    for (uint32_t i = 0; i < m_conn_cnt; i++) {
        std::unique_ptr<ProxyClient> up_proxyclient(std::make_unique<ProxyClient>(m_pool->getNextLoop(), addr));
        up_proxyclient->connect();
        m_clients.push_back(std::move(up_proxyclient));
    }

    return 0;
}

ProxyClient* MsgClientPool::getClientPtr() {
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
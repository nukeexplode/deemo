#include "RedisPool.h"

RedisPool::RedisPool(const char* host, int port, int db_num, int max_conn_cnt) {
    m_host = host;
    m_port = port;
    m_db_num = db_num;
    m_max_conn_cnt = max_conn_cnt;
    m_cur_conn_cnt = m_max_conn_cnt / 2;
}

RedisPool::~RedisPool() {
    for (auto p : m_list) {
        delete p;
    }

    m_list.clear();
    m_cur_conn_cnt = 0;
}

int RedisPool::Init() {
    for (int i = 0; i < m_cur_conn_cnt; i++) {
        RedisConn* conn = new RedisConn(m_host, m_port, m_db_num);
        if (conn->Init()) {
            delete conn;
            return -1;
        }

        m_list.push_back(conn);
    }

    return 0;
}

std::shared_ptr<RedisConn> RedisPool::GetRedisConn() {
    std::unique_lock<std::mutex> lock(m_mutex);
    while (m_list.empty()) {
        if (m_cur_conn_cnt >= m_max_conn_cnt) {
            m_cond.wait(lock);
        } else {
            RedisConn* conn = new RedisConn(m_host, m_port, m_db_num);
            if (conn->Init()) {
                delete conn;
                return nullptr;
            }

            m_list.push_back(conn);
            m_cur_conn_cnt++;
        }
    }

    RedisConn* conn = m_list.front();
    m_list.pop_front();

    std::shared_ptr<RedisConn> sp_redis_conn(conn, std::bind(&RedisPool::RelRedisConn, this, std::placeholders::_1));
    return sp_redis_conn;
}

void RedisPool::RelRedisConn(RedisConn* conn) {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_list.push_back(conn);
    m_cond.notify_all();
}
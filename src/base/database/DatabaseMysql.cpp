#include "DatabaseMysql.h"

DatabaseMysql::DatabaseMysql(const char* pool_name, const char* ip, int port, const char* username, const char* password, const char* db_name, int max_conn_cnt) {
    m_strHost = ip;
    m_strDataBase = db_name;
    m_strname = pool_name;
    m_strUser = username;
    m_strPassword = password;
    m_max_conn_cnt = max_conn_cnt;
    m_port = port;
    m_cur_conn_cnt = 0;
}

DatabaseMysql::~DatabaseMysql() {
    for (auto conn : m_conn_list) {
        delete conn;
    }
}

int DatabaseMysql::Init() {
    m_cur_conn_cnt = m_max_conn_cnt / 2;
    for (int i = 0; i < m_cur_conn_cnt; i++) {
		DatabaseConn* pdbconn = new DatabaseConn(m_strHost.c_str(), m_port, m_strUser.c_str(), m_strPassword.c_str(), m_strDataBase.c_str());
		int ret = pdbconn->Init();
		if (ret) {
			return ret;
		}

		m_conn_list.push_back(pdbconn);
	}

	return 0;
}

std::shared_ptr<DatabaseConn> DatabaseMysql::GetDBConn() {
    std::unique_lock<std::mutex> lock(m_mutex);
    assert(m_cur_conn_cnt <= m_max_conn_cnt);
    while (m_conn_list.empty()) {
        if (m_cur_conn_cnt == m_max_conn_cnt) {
            m_cond.wait(lock);
        } else {
            DatabaseConn* conn = new DatabaseConn(m_strHost.c_str(), m_port, m_strUser.c_str(), m_strPassword.c_str(), m_strDataBase.c_str());
            int ret = conn->Init();
            if (ret != 0) {
                LOG_ERROR << "database connection init fail";
                delete conn;
                return nullptr;
            }

            m_conn_list.push_back(conn);
            m_cur_conn_cnt++;
        }
    }

    DatabaseConn* conn = m_conn_list.front();
    m_conn_list.pop_front();

    std::shared_ptr<DatabaseConn> sp_conn(conn, std::bind(&DatabaseMysql::RelDBConn, this, std::placeholders::_1));
    return sp_conn;
}

void DatabaseMysql::RelDBConn(DatabaseConn* conn) {
    std::unique_lock<std::mutex> lock(m_mutex);

    m_conn_list.push_back(conn);
    m_cond.notify_all();
}
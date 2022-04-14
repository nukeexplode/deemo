#pragma once
#include <string>
#include <list>
#include <condition_variable>
#include <mutex>
#include <memory>
#include <assert.h>
#include <functional>

#include <muduo/base/Logging.h>

#include "DatabaseConn.h"

class DatabaseMysql {
public:
	DatabaseMysql(const char* pool_name, const char* ip, int port,
			const char* username, const char* password, const char* db_name, int max_conn_cnt);
	virtual ~DatabaseMysql();

	int Init();
	std::shared_ptr<DatabaseConn> GetDBConn();
	void RelDBConn(DatabaseConn* pConn);

    std::string getHost() { return m_strHost; }
	std::string getUser() { return m_strUser; }
	std::string getPwd() { return m_strPassword; }
	std::string getDBName() { return m_strDataBase; }
    std::string getName() { return m_strname; }
    int getPort() { return m_port; }
	// std::string getCharSet() { return m_strCharactSet; }

private:
    std::string m_strHost;
    std::string m_strUser;
    std::string m_strPassword;
    std::string m_strDataBase;
    std::string m_strname;
    int m_port;

    int m_cur_conn_cnt;
    int m_max_conn_cnt;
    
    std::list<DatabaseConn*> m_conn_list;
    std::condition_variable m_cond;
    std::mutex m_mutex;                     
};
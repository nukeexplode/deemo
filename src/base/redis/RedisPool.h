#pragma once
#include <list>
#include <string>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <functional>

#include "RedisConn.h"

class RedisPool {
public:
	RedisPool(const char* host, int port, int db_num, int max_conn_cnt);
	virtual ~RedisPool();
	int Init();
	std::shared_ptr<RedisConn> GetRedisConn();
	void RelRedisConn(RedisConn*);

	const char* GetHost() { return m_host.c_str(); }
	int GetPort() { return m_port; }
	int GetDBNum() { return m_db_num; }
	// std::string GetPassword() {return m_password; }
private:
	// std::string m_pool_name;
	std::string m_host;
	// std::string m_password;

	int m_port;
	int m_db_num;
	int m_cur_conn_cnt;
	int m_max_conn_cnt;

	std::list<RedisConn*> m_list;
	std::condition_variable m_cond;
    std::mutex m_mutex;
};
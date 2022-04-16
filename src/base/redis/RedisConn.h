#pragma once
#include <string>
#include <vector>
#include <map>
#include <list>
#include <memory>

#include <hiredis/hiredis.h>
#include <muduo/base/Logging.h>

class RedisConn {
public:
	RedisConn(std::string& host, int port, int db_num);
	virtual ~RedisConn();

	int Init();
	const char* GetPoolName();

	std::string get(std::string key);
	std::string setex(std::string key, int timeout, std::string value);
    std::string set(std::string key, std::string& value);
    
    //批量获取
    bool mget(const std::vector<std::string>& keys, std::map<std::string, std::string>& ret_value);
    // 判断一个key是否存在
    bool isExists(std::string &key);

	// Redis hash structure
	bool hExists(std::string key, std::string field);
	long hdel(std::string key, std::string field);
	std::string hget(std::string key, std::string field);
	bool hgetAll(std::string key, std::map<std::string, std::string>& ret_value);
	long hset(std::string key, std::string field, std::string value);

	long hincrBy(std::string key, std::string field, long value);
	long incrBy(std::string key, long value);
	std::string hmset(std::string key, std::map<std::string, std::string>& hash);
	bool hmget(std::string key, std::list<std::string>& fields, std::list<std::string>& ret_value);

	//原子加减1
	long incr(std::string key);
	long decr(std::string key);

	// Redis list structure
	long lpush(std::string key, std::string value);
	long rpush(std::string key, std::string value);
	long llen(std::string key);
	bool lrange(std::string key, long start, long end, std::list<std::string>& ret_value);

private:
    std::string m_host;
	int m_port;
	int m_db_num;

	redisContext* 	m_pContext;
	uint64_t		m_last_connect_time;
};
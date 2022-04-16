#pragma once
#include <memory>
#include <map>
#include <string>

#include <muduo/base/Logging.h>

#include "RedisPool.h"
#include "RedisConn.h"
#include "ConfigFileReader.h"

class RedisManager {
public:
    RedisManager();
	virtual ~RedisManager();

	int Init();
	std::shared_ptr<RedisConn> GetRedisConn(const char* pool_name);
private:
	std::map<std::string, std::unique_ptr<RedisPool>> m_redis_pool_map;
};
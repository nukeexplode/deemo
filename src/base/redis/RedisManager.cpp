#include "RedisManager.h"

RedisManager::RedisManager() {

}

RedisManager::~RedisManager() {

}

int RedisManager::Init() {
    CConfigFileReader config("../../etc/redis.conf");

    std::string redis_instance = config.getConfigName("instances");
    if (redis_instance.empty()) {
        LOG_DEBUG << "not config DBInstance";
        return -1;
    }

    std::string host = config.getConfigName("host");
	std::string str_port= config.getConfigName("port");
    std::string str_db_num = config.getConfigName("db_num");
    std::string str_max_conn_cnt = config.getConfigName("maxconncnt");

    if (redis_instance.empty() || host.empty() || str_port.empty() || str_db_num.empty() || str_max_conn_cnt.empty()) {
        LOG_ERROR << "redis config file error";
        return -1;
    }

    int port = std::stoi(str_port);
    int db_num = std::stoi(str_db_num);
    int max_conn_cnt = std::stoi(str_max_conn_cnt);
    std::unique_ptr<RedisPool> up_redis_pool = std::make_unique<RedisPool>(host.c_str(), port, db_num, max_conn_cnt);
    if (up_redis_pool->Init() != 0) {
        LOG_ERROR << "init db instance failed: " << redis_instance;
        return -1;
    }

    m_redis_pool_map[redis_instance] = std::move(up_redis_pool);
    return 0;
}

std::shared_ptr<RedisConn> RedisManager::GetRedisConn(const char* name) {
    std::string str_name(name);
    auto iter = m_redis_pool_map.find(name);
    if (iter == m_redis_pool_map.end()) {
        return nullptr;
    }

    return iter->second->GetRedisConn();
}
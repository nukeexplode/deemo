#include <memory>

#include <muduo/base/Logging.h>

#include "RedisManager.h"
#include "Singleton.h"

int main() {
    Singleton<RedisManager>::Instance().Init();

    std::shared_ptr<RedisConn> pCacheConn = Singleton<RedisManager>::Instance().GetRedisConn("redis");
    std::string str = "redis";
    LOG_INFO << pCacheConn->set("redis", str);
    LOG_INFO << pCacheConn->get("redis");
}
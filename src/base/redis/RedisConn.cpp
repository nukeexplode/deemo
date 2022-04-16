#include "RedisConn.h"

RedisConn::RedisConn(std::std::string& host, int port, int db_num)
{
    m_host = host;
    m_port = port;
    m_db_num = db_num;

	m_pContext = NULL;
	m_last_connect_time = 0;
}

RedisConn::~RedisConn()
{
	if (m_pContext) {
		redisFree(m_pContext);
		m_pContext = NULL;
	}
}

/*
 * redis初始化连接和重连操作，类似mysql_ping()
 */
int RedisConn::Init()
{
	if (m_pContext) {
		return 0;
	}

	// 4s 尝试重连一次
	uint64_t cur_time = (uint64_t)time(NULL);
	if (cur_time < m_last_connect_time + 4) {
		return 0;
	}

	m_last_connect_time = cur_time;

	// 200ms超时
	struct timeval timeout = {0, 200000};
	m_pContext = redisConnectWithTimeout(m_host.c_str(), m_port, timeout);
	if (!m_pContext || m_pContext->err) {
		if (m_pContext) {
            char log[255];
			sprintf(log, "redisConnect failed: %s", m_pContext->errstr);
            LOG_DEBUG << log;
			redisFree(m_pContext);
			m_pContext = NULL;
		} else {
            char log[255];
			sprintf(log, "redisConnect failed");
            LOG_DEBUG << log;
		}

		return 1;
	}

	redisReply* reply = NULL;
	reply = (redisReply *)redisCommand(m_pContext, "SELECT %d", m_db_num);
	if (reply && (reply->type == REDIS_REPLY_STATUS) && (strncmp(reply->str, "OK", 2) == 0)) {
		freeReplyObject(reply);
		return 0;
	} else {
        char log[255];
		sprintf(log, "select cache db failed");
        LOG_DEBUG << log;
		return 2;
	}
}

std::string RedisConn::get(std::string key)
{
	std::string value;

	if (Init()) {
		return value;
	}

	redisReply* reply = (redisReply *)redisCommand(m_pContext, "GET %s", key.c_str());
	if (!reply) {
		sprintf("redisCommand failed:%s", m_pContext->errstr);
		redisFree(m_pContext);
		m_pContext = NULL;
		return value;
	}

	if (reply->type == REDIS_REPLY_STRING) {
		value.append(reply->str, reply->len);
	}

	freeReplyObject(reply);
	return value;
}

std::string RedisConn::setex(std::string key, int timeout, std::string value)
{
	std::string ret_value;

	if (Init()) {
		return ret_value;
	}

	redisReply* reply = (redisReply *)redisCommand(m_pContext, "SETEX %s %d %s", key.c_str(), timeout, value.c_str());
	if (!reply) {
		sprintf("redisCommand failed:%s", m_pContext->errstr);
		redisFree(m_pContext);
		m_pContext = NULL;
		return ret_value;
	}

	ret_value.append(reply->str, reply->len);
	freeReplyObject(reply);
	return ret_value;
}

std::string RedisConn::set(std::string key, std::string &value)
{
    std::string ret_value;
    
    if (Init()) {
        return ret_value;
    }
    
    redisReply* reply = (redisReply *)redisCommand(m_pContext, "SET %s %s", key.c_str(), value.c_str());
    if (!reply) {
        sprintf("redisCommand failed:%s", m_pContext->errstr);
        redisFree(m_pContext);
        m_pContext = NULL;
        return ret_value;
    }
    
    ret_value.append(reply->str, reply->len);
    freeReplyObject(reply);
    return ret_value;
}

bool RedisConn::mget(const std::vector<std::string>& keys, std::map<std::string, std::string>& ret_value)
{
    if(Init())
    {
        return false;
    }
    if(keys.empty())
    {
        return false;
    }
    
    std::string strKey;
    bool bFirst = true;
    for (std::vector<std::string>::const_iterator it=keys.begin(); it!=keys.end(); ++it) {
        if(bFirst)
        {
            bFirst = false;
            strKey = *it;
        }
        else
        {
            strKey += " " + *it;
        }
    }
    
    if(strKey.empty())
    {
        return false;
    }
    strKey = "MGET " + strKey;
    redisReply* reply = (redisReply*) redisCommand(m_pContext, strKey.c_str());
    if (!reply) {
        sprintf("redisCommand failed:%s", m_pContext->errstr);
        redisFree(m_pContext);
        m_pContext = NULL;
        return false;
    }
    if(reply->type == REDIS_REPLY_ARRAY)
    {
        for(size_t i=0; i<reply->elements; ++i)
        {
            redisReply* child_reply = reply->element[i];
            if (child_reply->type == REDIS_REPLY_STRING) {
                ret_value[keys[i]] = child_reply->str;
            }
        }
    }
    freeReplyObject(reply);
    return true;
}

bool RedisConn::isExists(std::string &key)
{
    if (Init()) {
        return false;
    }
    
    redisReply* reply = (redisReply*) redisCommand(m_pContext, "EXISTS %s", key.c_str());
    if(!reply)
    {
        sprintf("redisCommand failed:%s", m_pContext->errstr);
        redisFree(m_pContext);
        return false; 
    }
    long ret_value = reply->integer;
    freeReplyObject(reply);
    if(0 == ret_value)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool RedisConn::hExists(std::string key, std::string field)
{
    if (Init()) {
        return false;
    }
    
    redisReply* reply = (redisReply*) redisCommand(m_pContext, "HEXISTS %s %s", key.c_str(),field.c_str());
    if(!reply)
    {
        sprintf("redisCommand failed:%s", m_pContext->errstr);
        redisFree(m_pContext);
        return false;
    }
    long ret_value = reply->integer;
    freeReplyObject(reply);
    if(0 == ret_value)
    {
        return false;
    }
    else
    {
        return true;
    }
}

long RedisConn::hdel(std::string key, std::string field)
{
	if (Init()) {
		return 0;
	}

	redisReply* reply = (redisReply *)redisCommand(m_pContext, "HDEL %s %s", key.c_str(), field.c_str());
	if (!reply) {
		sprintf("redisCommand failed:%s", m_pContext->errstr);
		redisFree(m_pContext);
		m_pContext = NULL;
		return 0;
	}

	long ret_value = reply->integer;
	freeReplyObject(reply);
	return ret_value;
}

std::string RedisConn::hget(std::string key, std::string field)
{
	std::string ret_value;
	if (Init()) {
		return ret_value;
	}

	redisReply* reply = (redisReply *)redisCommand(m_pContext, "HGET %s %s", key.c_str(), field.c_str());
	if (!reply) {
		sprintf("redisCommand failed:%s", m_pContext->errstr);
		redisFree(m_pContext);
		m_pContext = NULL;
		return ret_value;
	}

	if (reply->type == REDIS_REPLY_std::string) {
		ret_value.append(reply->str, reply->len);
	}

	freeReplyObject(reply);
	return ret_value;
}

bool RedisConn::hgetAll(std::string key, std::map<std::string, std::string>& ret_value)
{
	if (Init()) {
		return false;
	}

	redisReply* reply = (redisReply *)redisCommand(m_pContext, "HGETALL %s", key.c_str());
	if (!reply) {
		sprintf("redisCommand failed:%s", m_pContext->errstr);
		redisFree(m_pContext);
		m_pContext = NULL;
		return false;
	}

	if ( (reply->type == REDIS_REPLY_ARRAY) && (reply->elements % 2 == 0) ) {
		for (size_t i = 0; i < reply->elements; i += 2) {
			redisReply* field_reply = reply->element[i];
			redisReply* value_reply = reply->element[i + 1];

			std::string field(field_reply->str, field_reply->len);
			std::string value(value_reply->str, value_reply->len);
			ret_value.insert(make_pair(field, value));
		}
	}

	freeReplyObject(reply);
	return true;
}

long RedisConn::hset(std::string key, std::string field, std::string value)
{
	if (Init()) {
		return -1;
	}

	redisReply* reply = (redisReply *)redisCommand(m_pContext, "HSET %s %s %s", key.c_str(), field.c_str(), value.c_str());
	if (!reply) {
		sprintf("redisCommand failed:%s", m_pContext->errstr);
		redisFree(m_pContext);
		m_pContext = NULL;
		return -1;
	}

	long ret_value = reply->integer;
	freeReplyObject(reply);
	return ret_value;
}

long RedisConn::hincrBy(std::string key, std::string field, long value)
{
	if (Init()) {
		return -1;
	}

	redisReply* reply = (redisReply *)redisCommand(m_pContext, "HINCRBY %s %s %ld", key.c_str(), field.c_str(), value);
	if (!reply) {
		sprintf("redisCommand failed:%s", m_pContext->errstr);
		redisFree(m_pContext);
		m_pContext = NULL;
		return -1;
	}

	long ret_value = reply->integer;
	freeReplyObject(reply);
	return ret_value;
}

long RedisConn::incrBy(std::string key, long value)
{
    if(Init())
    {
        return -1;
    }
    
    redisReply* reply = (redisReply*)redisCommand(m_pContext, "INCRBY %s %ld", key.c_str(), value);
    if(!reply)
    {
        sprintf("redis Command failed:%s", m_pContext->errstr);
        redisFree(m_pContext);
        m_pContext = NULL;
        return -1;
    }
    long ret_value = reply->integer;
    freeReplyObject(reply);
    return ret_value;
}

std::string RedisConn::hmset(std::string key, std::map<std::string, std::string>& hash)
{
	std::string ret_value;

	if (Init()) {
		return ret_value;
	}

	int argc = hash.size() * 2 + 2;
	std::unique_ptr<const char* []> argv = std::make_unique<const char* []>(argc);
	if (!argv) {
		return ret_value;
	}

	argv[0] = "HMSET";
	argv[1] = key.c_str();
	int i = 2;
	for (std::map<std::string, std::string>::iterator it = hash.begin(); it != hash.end(); it++) {
		argv[i++] = it->first.c_str();
		argv[i++] = it->second.c_str();
	}

	redisReply* reply = (redisReply *)redisCommandArgv(m_pContext, argc, argv, NULL);
	if (!reply) {
		sprintf("redisCommand failed:%s", m_pContext->errstr);

		redisFree(m_pContext);
		m_pContext = NULL;
		return ret_value;
	}

	ret_value.append(reply->str, reply->len);

	freeReplyObject(reply);
	return ret_value;
}

bool RedisConn::hmget(std::string key, std::list<std::string>& fields, std::list<std::string>& ret_value)
{
	if (Init()) {
		return false;
	}

	int argc = fields.size() + 2;
	std::unique_ptr<const char* []> argv = std::make_unique<const char* []>(argc);
	if (!argv) {
		return false;
	}

	argv[0] = "HMGET";
	argv[1] = key.c_str();
	int i = 2;
	for (std::list<std::string>::iterator it = fields.begin(); it != fields.end(); it++) {
		argv[i++] = it->c_str();
	}

	redisReply* reply = (redisReply *)redisCommandArgv(m_pContext, argc, (const char**)argv, NULL);
	if (!reply) {
		sprintf("redisCommand failed:%s", m_pContext->errstr);

		redisFree(m_pContext);
		m_pContext = NULL;

		return false;
	}

	if (reply->type == REDIS_REPLY_ARRAY) {
		for (size_t i = 0; i < reply->elements; i++) {
			redisReply* value_reply = reply->element[i];
			std::string value(value_reply->str, value_reply->len);
			ret_value.push_back(value);
		}
	}

	freeReplyObject(reply);
	return true;
}

long RedisConn::incr(std::string key)
{
    if(Init())
    {
        return -1;
    }
    
    redisReply* reply = (redisReply*)redisCommand(m_pContext, "INCR %s", key.c_str());
    if(!reply)
    {
        sprintf("redis Command failed:%s", m_pContext->errstr);
        redisFree(m_pContext);
        m_pContext = NULL;
        return -1;
    }
    long ret_value = reply->integer;
    freeReplyObject(reply);
    return ret_value;
}

long RedisConn::decr(std::string key)
{
    if(Init())
    {
        return -1;
    }
    
    redisReply* reply = (redisReply*)redisCommand(m_pContext, "DECR %s", key.c_str());
    if(!reply)
    {
        sprintf("redis Command failed:%s", m_pContext->errstr);
        redisFree(m_pContext);
        m_pContext = NULL;
        return -1;
    }
    long ret_value = reply->integer;
    freeReplyObject(reply);
    return ret_value;
}

long RedisConn::lpush(std::string key, std::string value)
{
	if (Init()) {
		return -1;
	}

	redisReply* reply = (redisReply *)redisCommand(m_pContext, "LPUSH %s %s", key.c_str(), value.c_str());
	if (!reply) {
		sprintf("redisCommand failed:%s", m_pContext->errstr);
		redisFree(m_pContext);
		m_pContext = NULL;
		return -1;
	}

	long ret_value = reply->integer;
	freeReplyObject(reply);
	return ret_value;
}

long RedisConn::rpush(std::string key, std::string value)
{
	if (Init()) {
		return -1;
	}

	redisReply* reply = (redisReply *)redisCommand(m_pContext, "RPUSH %s %s", key.c_str(), value.c_str());
	if (!reply) {
		sprintf("redisCommand failed:%s", m_pContext->errstr);
		redisFree(m_pContext);
		m_pContext = NULL;
		return -1;
	}

	long ret_value = reply->integer;
	freeReplyObject(reply);
	return ret_value;
}

long RedisConn::llen(std::string key)
{
	if (Init()) {
		return -1;
	}

	redisReply* reply = (redisReply *)redisCommand(m_pContext, "LLEN %s", key.c_str());
	if (!reply) {
		sprintf("redisCommand failed:%s", m_pContext->errstr);
		redisFree(m_pContext);
		m_pContext = NULL;
		return -1;
	}

	long ret_value = reply->integer;
	freeReplyObject(reply);
	return ret_value;
}

bool RedisConn::lrange(std::string key, long start, long end, std::list<std::string>& ret_value)
{
	if (Init()) {
		return false;
	}

	redisReply* reply = (redisReply *)redisCommand(m_pContext, "LRANGE %s %d %d", key.c_str(), start, end);
	if (!reply) {
		sprintf("redisCommand failed:%s", m_pContext->errstr);
		redisFree(m_pContext);
		m_pContext = NULL;
		return false;
	}

	if (reply->type == REDIS_REPLY_ARRAY) {
		for (size_t i = 0; i < reply->elements; i++) {
			redisReply* value_reply = reply->element[i];
			std::string value(value_reply->str, value_reply->len);
			ret_value.push_back(value);
		}
	}

	freeReplyObject(reply);
	return true;
}
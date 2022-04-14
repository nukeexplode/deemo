#pragma once
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <muduo/base/Logging.h>

#include "DatabaseMysql.h"
#include "DatabaseConn.h"
#include "../ConfigFileReader.h"

class MysqlManager
{
public:
	MysqlManager();
	virtual ~MysqlManager();

	int Init();

	std::shared_ptr<DatabaseConn> GetDBConn(const char* dbpool_name);
	void RelDBConn(DatabaseConn* pConn);

private:
	std::map<std::string, std::unique_ptr<DatabaseMysql>> m_pool_map;
};
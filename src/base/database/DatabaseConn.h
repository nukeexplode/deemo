#pragma once
#include <memory>
#include <mysql/mysql.h>
#include <muduo/base/Logging.h>

#include "QueryResult.h"

class DatabaseConn {
public:
	DatabaseConn(const char* ip, int port, const char* username, const char* password, const char* db_name);
	virtual ~DatabaseConn();
	int Init();

	std::unique_ptr<QueryResult> query(const char* sql_query);
	bool execute(const char* sql_query);
	// char* EscapeString(const char* content, uint32_t content_len);

	uint32_t GetInsertId() { return (uint32_t)mysql_insert_id(m_mysql); }

	// const char* GetPoolName();
	MYSQL* GetMysql() { return m_mysql; }
private:
    const char* m_ip;
    int m_port;
    const char* m_username;
    const char* m_password;
    const char* m_dbname;

	// CDBPool* 	m_pDBPool;	// to get MySQL server information
	MYSQL* 		m_mysql;
	//MYSQL_RES*	m_res;
	// char		m_escape_string[MAX_ESCAPE_STRING_LEN + 1];
};
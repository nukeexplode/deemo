#include "DatabaseConn.h"

DatabaseConn::DatabaseConn(const char* ip, int port, const char* username, const char* password, const char* db_name) {
    m_ip = ip;
    m_port = port;
    m_username = username;
    m_password = password;
    m_dbname = db_name;
    m_mysql = nullptr;
}

DatabaseConn::~DatabaseConn() {
    if (m_mysql != nullptr) {
        mysql_close(m_mysql);
    }
}

int DatabaseConn::Init() {
    m_mysql = mysql_init(NULL);
    if (m_mysql == nullptr) {
        LOG_ERROR << "DatabaseConn init fail";
        return -1;
    }
    
    bool reconnect = true;
	mysql_options(m_mysql, MYSQL_OPT_RECONNECT, &reconnect);
	mysql_options(m_mysql, MYSQL_SET_CHARSET_NAME, "utf8mb4");

    if (mysql_real_connect(m_mysql, m_ip, m_username, m_password, m_dbname, m_port, NULL, 0) == nullptr) {
        LOG_ERROR << "mysql_real_connect fail";
        return -1;
    }

    return 0;
}

std::unique_ptr<QueryResult> DatabaseConn::query(const char* sql_query)
{
	mysql_ping(m_mysql);

	if (mysql_real_query(m_mysql, sql_query, strlen(sql_query))) {
        char log[255];
		sprintf(log, "mysql_real_query failed: %s, sql: %s", mysql_error(m_mysql), sql_query);
        LOG_ERROR << log;
		return NULL;
	}

	MYSQL_RES* res = mysql_store_result(m_mysql);
	if (!res) {
        char log[255];
		sprintf(log, "mysql_store_result failed: %s", mysql_error(m_mysql));
        LOG_ERROR << log;
		return NULL;
	} 
    uint64_t rowCount = mysql_affected_rows(m_mysql);
    unsigned fieldCount = mysql_field_count(m_mysql);

	std::unique_ptr<QueryResult> up_result_set(std::make_unique<QueryResult>(res, rowCount, fieldCount));
    up_result_set->nextRow();
	return up_result_set;
}

bool DatabaseConn::execute(const char* sql_query)
{
	mysql_ping(m_mysql);

	if (mysql_real_query(m_mysql, sql_query, strlen(sql_query))) {
        char log[255];
		sprintf(log, "mysql_real_query failed: %s, sql: %s", mysql_error(m_mysql), sql_query);
        LOG_ERROR << log;
		return false;
	}

	if (mysql_affected_rows(m_mysql) > 0) {
		return true;
	} else {
		return false;
	}
}
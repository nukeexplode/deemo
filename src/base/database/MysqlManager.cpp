#include "MysqlManager.h"

MysqlManager::MysqlManager() {

}

MysqlManager::~MysqlManager() {

}

int MysqlManager::Init() {
    CConfigFileReader config("../../etc/database.conf");

    std::string db_instance = config.getConfigName("instances");
    if (db_instance.empty()) {
        LOG_DEBUG << "not config DBInstance";
        return -1;
    }

    std::string host = config.getConfigName("host");
	std::string str_port= config.getConfigName("port");
	std::string dbname = config.getConfigName("dbname");
	std::string username = config.getConfigName("username");
	std::string password = config.getConfigName("password");
	std::string str_maxconncnt = config.getConfigName("maxconncnt");

    if (host.empty() || str_port.empty() || dbname.empty() || username.empty() || password.empty() || str_maxconncnt.empty()) {
        LOG_ERROR << "config file error";
        return -1;
    }

    int port = std::stoi(str_port);
    int max_conncnt = std::stoi(str_maxconncnt);
    std::unique_ptr<DatabaseMysql> sp_DBpool = std::make_unique<DatabaseMysql>(dbname.c_str(), host.c_str(), port, username.c_str(), password.c_str(), dbname.c_str(), max_conncnt);
    if (sp_DBpool->Init() != 0) {
        LOG_ERROR << "init db instance failed: " << dbname;
        return -1;
    }

    m_pool_map[dbname] = std::move(sp_DBpool);
    return 0;
}

std::shared_ptr<DatabaseConn> MysqlManager::GetDBConn(const char* dbname) {
    std::string str_dbname(dbname);
    auto iter = m_pool_map.find(str_dbname);
    if (iter == m_pool_map.end()) {
        return nullptr;
    }

    return iter->second->GetDBConn();
}
#include <assert.h>
#include <iostream>
#include <memory>
#include <thread>

#include "database/MysqlManager.h"
#include "Singleton.h"
#include <muduo/base/Logging.h>

void threadfunc() {
    auto pDBConn = Singleton<MysqlManager>::Instance().GetDBConn("deemo");
    pDBConn->execute("insert into t_user(`f_user_id`, `f_username`, `f_password`, `f_signature`, `f_owner_id`) values (0, 'nukeexplode1', 'TKQ19990315', '111111111', '0')");
    std::unique_ptr<QueryResult> up_result = pDBConn->query("select * from t_user");

    while (true)
    {
        Field* pRow = up_result->fetch();
        if (pRow == NULL)
            break;

        int user_id = pRow[0].getInt32();
        std::string username = pRow[1].getCppString();
        std::string password = pRow[2].getCppString();
        int owner_id = pRow[3].getInt32();

        char log[255];
        sprintf(log, "userid: %d, username: %s, password: %s, owner_id: %d", user_id, username.c_str(), password.c_str(), owner_id);
        std::cout << log << std::endl;

        if (!up_result->nextRow())
        {
            break;
        }
    }
}

int main() {
    assert(Singleton<MysqlManager>::Instance().Init() == 0);

    std::vector<std::unique_ptr<std::thread>> threads;
    for (int i = 0; i < 100; i++) {
        std::unique_ptr<std::thread> t = std::make_unique<std::thread>(threadfunc);
        threads.push_back(std::move(t));
    }

    for (auto&& t : threads) {
        t->join();
    }

    return 0;
}
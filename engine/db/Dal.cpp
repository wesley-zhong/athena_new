//
// Created by zhongweiqi on 2025/10/18.
//
#include "Dal.hpp"

namespace Dal::DB {
    DBInterfaceMysql *mysql = nullptr;

    void init(const std::string &ip, unsigned int port, const std::string &dbname, const std::string &username,
              const std::string &password) {
        mysql = initDB<DBInterfaceMysql>(ip.c_str(), port, dbname.c_str(), username.c_str(), password.c_str());
    }

    int execute(DBResult *result, const char *cmd, int len) {
        return mysql->execute(result, cmd, len);
    }
}

namespace Dal::Cache {
    DBInterfaceRedis *redis = nullptr;

    void init(const std::string &ip, unsigned int port, const std::string &dbname, const std::string &username,
              const std::string &password) {
        redis = initDB<DBInterfaceRedis>(ip.c_str(), port, dbname.c_str(), username.c_str(), password.c_str());
    }

    int execute(DBResult *result, const char *cmd, int len) {
        return redis->execute(result, cmd, len);
    }
}

//
// Created by zhongweiqi on 2025/10/18.
//
#pragma once
#include <string>
#include "MysqlResult.h"
#include  "RedisResult.h"
#include "DB_Interface_mysql.h"
#include "hiredis.h"
#include "DB_Interface_redis.h"


namespace Dal {
    template<typename T>
    T *initDB(const char *host, unsigned int port, const char *dbname, const char *user, const char *pswd
    ) {
        T *obj = new T(host, port, dbname, user, pswd);
        obj->connect();
        return obj;
    }

    namespace DB {
        extern DBInterfaceMysql *mysql;

        void init(const std::string &ip, unsigned int port, const std::string &dbname, const std::string &username,
                  const std::string &password);

        int execute(DBResult *result, const char *cmd, int len);
    };

    namespace Cache {
        extern DBInterfaceRedis *redis;

        void init(const std::string &ip, unsigned int port, const std::string &dbname, const std::string &username,
                  const std::string &password);

        int execute(DBResult *result, const char *cmd, int len);
    }
}

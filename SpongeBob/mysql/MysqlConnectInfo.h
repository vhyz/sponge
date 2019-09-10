#pragma once

#include <string>

namespace SpongeBob {

namespace mysql {

struct MysqlConnectInfo {
    std::string host;
    uint16_t port;
    std::string user;
    std::string passwd;
    std::string db;
};

}  // namespace mysql

}  // namespace SpongeBob
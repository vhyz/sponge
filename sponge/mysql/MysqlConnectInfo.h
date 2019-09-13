#pragma once

#include <string>

namespace sponge {

namespace mysql {

struct MysqlConnectInfo {
    std::string host;
    uint16_t port;
    std::string user;
    std::string passwd;
    std::string db;
};

}  // namespace mysql

}  // namespace sponge
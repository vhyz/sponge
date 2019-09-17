#pragma once

#include <sponge/Channel.h>
#include <sponge/EventLoop.h>
#include "MysqlConnectInfo.h"

#include <mariadb/mysql.h>
#include <memory>
#include <string_view>

namespace sponge {

namespace mysql {

class MysqlConnection : std::enable_shared_from_this<MysqlConnection> {
   private:
    enum RunningState { kSendQuery = 0, kCommit, kRollback };

   public:
    using MysqlConnectionPtr = std::shared_ptr<MysqlConnection>;

    using MysqlCallBack = std::function<void()>;

    void sendQuery(std::string_view sql);
    void sendQueryAsync(std::string sql, MysqlCallBack callBack);

    void commit();
    void commitAsync(MysqlCallBack callBack);

    void rollback();
    void rollbackAsync(MysqlCallBack callBack);

    MYSQL& getMysqlData() { return mysql_; }

   private:
    MYSQL mysql_;

    MysqlConnectInfo mysqlConnectInfo_;

    // 每一次异步进行的回调函数
    MysqlCallBack mysqlCallBack_;

    std::string context_;

    EventLoop* loop_;

    std::unique_ptr<Channel> channel_;
};

}  // namespace mysql

}  // namespace sponge
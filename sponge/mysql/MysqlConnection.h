#pragma once

#include <sponge/EventLoop.h>

#include <mariadb/mysql.h>
#include <memory>
#include <string>

namespace sponge {

namespace mysql {

class MysqlError : std::exception {
   public:
    MysqlError() : mysqlErrno_(0) {}

    MysqlError(MYSQL* mysql)
        : mysqlErrno_(mysql_errno(mysql)), mysqlErrorMsg_(mysql_error(mysql)) {}

    operator bool() const { return mysqlErrno_ != 0; }

    unsigned int getErrno() const { return mysqlErrno_; }

    virtual const char* what() const noexcept override {
        return mysqlErrorMsg_.c_str();
    }

   private:
    unsigned int mysqlErrno_;

    std::string mysqlErrorMsg_;
};

class MysqlDabaBase {
   public:
   
   private:
    MYSQL* mysql_;
};

class MysqlConnection : std::enable_shared_from_this<MysqlConnection> {
   public:
    using MysqlConnectionPtr = std::shared_ptr<MysqlConnection>;

    using MysqlCallBack = std::function<void()>;

    using AffectedRowsCallBack =
        std::function<void(const MysqlError& err, size_t rows)>;

    void connect(const char* host, const char* user, const char* password,
                 const char* db, unsigned int port,
                 const char* unixSocket = nullptr,
                 unsigned long clientflag = 0);

    /*
        异步连接需要保证传入的字符串生命周期不会过早结束
        这是因为MysqlConnection不会保存连接信息
    */
    void connectAsync(const char* host, const char* user, const char* password,
                      const char* db, unsigned int port,
                      const char* unixSocket = nullptr,
                      unsigned long clientflag = 0);

    /*
        下面的异步操作无需保证字符串的生命周期
        这是因为MysqlConnection会保存该上下文信息
    */

    void query(const char* sql, size_t sqlLength);
    void queryAsync(const char* sql, size_t sqlLength, MysqlCallBack callBack);

    void execute(const char* sql, size_t sqlLength);
    void executeAsync(const char* sql, size_t sqlLength,
                      MysqlCallBack callBack);

    void commit();
    void commitAsync(MysqlCallBack callBack);

    void rollback();
    void rollbackAsync(MysqlCallBack callBack);

   private:
    EventLoop* loop_;

    // 每一次异步进行的回调函数
    MysqlCallBack mysqlCallBack_;

    std::string context_;
};

}  // namespace mysql

}  // namespace sponge
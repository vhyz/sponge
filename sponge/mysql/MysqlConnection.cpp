#include "MysqlConnection.h"

// TODO

namespace sponge {

namespace mysql {

void MysqlConnection::sendQuery(std::string_view sql) {
    mysql_send_query(&mysql_, sql.data(), sql.size());
}

void MysqlConnection::sendQueryAsync(std::string sql, MysqlCallBack callBack) {
    assert(loop_ != nullptr);
    context_ = std::move(sql);

    int ret, status;
    status = mysql_send_query_start(&ret, &mysql_, context_.c_str(),
                                    context_.size());

    if (status) {
        mysqlCallBack_ = std::move(callBack);
    } else {
        callBack();
    }
}

void MysqlConnection::commit() { mysql_commit(&mysql_); }

void MysqlConnection::commitAsync(MysqlCallBack callBack) {
    assert(loop_ != nullptr);
    my_bool ret;
    int status = mysql_commit_start(&ret, &mysql_);

    if (status) {
        mysqlCallBack_ = std::move(callBack);
    } else {
        callBack();
    }
}

void MysqlConnection::rollback() { mysql_rollback(&mysql_); }

void MysqlConnection::rollbackAsync(MysqlCallBack callBack) {
    assert(loop_ != nullptr);
    my_bool ret;
    int status = mysql_rollback_start(&ret, &mysql_);

    if (status) {
        mysqlCallBack_ = std::move(callBack);
    } else {
        callBack();
    }
}

}  // namespace mysql
}  // namespace sponge
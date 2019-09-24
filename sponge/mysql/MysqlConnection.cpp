#include "MysqlConnection.h"

// TODO

namespace sponge {

namespace mysql {

void MysqlConnection::query(std::string_view sql) {
    mysql_send_query(&mysql_, sql.data(), sql.size());
}

void MysqlConnection::queryAsync(std::string_view sql, MysqlCallBack callBack) {
    context_ = sql;
}

void MysqlConnection::commit() { mysql_commit(&mysql_); }

void MysqlConnection::commitAsync(MysqlCallBack callBack) {}

void MysqlConnection::rollback() { mysql_rollback(&mysql_); }

void MysqlConnection::rollbackAsync(MysqlCallBack callBack) {}

}  // namespace mysql
}  // namespace sponge
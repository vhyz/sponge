#pragma once

#include <SpongeBob/Channel.h>
#include <SpongeBob/EventLoop.h>
#include <mariadb/mysql.h>

namespace SpongeBob {

class MySQLConnection {
   public:
   private:
    MYSQL* mysql_;

    Channel* channel_;

    EventLoop* loop_;

    void f() {
        
    }
};

}  // namespace SpongeBob
#ifndef SPONGEBOB_CALLBACK_H
#define SPONGEBOB_CALLBACK_H
#include <functional>
#include <memory>
#include <string>

class TcpConnection;

using spTcpConnection = std::shared_ptr<TcpConnection>;

using CallBack = std::function<void()>;

using ConnectionCallBack = std::function<void(const spTcpConnection&)>;

using MessageCallBack =
    std::function<void(const spTcpConnection&, std::string&)>;

#endif
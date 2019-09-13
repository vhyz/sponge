#pragma once

#include <functional>
#include <memory>
#include <string>
#include "ChannelBuffer.h"

namespace sponge {

class TcpConnection;

using spTcpConnection = std::shared_ptr<TcpConnection>;

using CallBack = std::function<void()>;

using ConnectionCallBack = std::function<void(const spTcpConnection&)>;

using MessageCallBack =
    std::function<void(const spTcpConnection&, ChannelBuffer&)>;

}  // namespace sponge

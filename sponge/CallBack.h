#pragma once

#include <functional>
#include <memory>
#include <string>
#include "ChannelBuffer.h"

namespace sponge {

class TcpConnection;

using CallBack = std::function<void()>;

using ConnectionCallBack = std::function<void(const std::shared_ptr<TcpConnection>&)>;

using MessageCallBack =
    std::function<void(const std::shared_ptr<TcpConnection>&, ChannelBuffer&)>;

}  // namespace sponge

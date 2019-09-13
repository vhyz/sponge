#include "Channel.h"
#include <sys/epoll.h>
#include <iostream>
#include "Logger.h"

namespace sponge {

void Channel::handleEvent() {
    // FIXME: 为什么这里用的是if else
    // 原因：
    // 因为在处理下一个事件之前，TcpConnection可能已经close然后析构掉了，所以属于该连接的Channel也已经析构了
    // 所以如果再尝试处理下一个事件时，如果事件就绪，在尝试调用回调函数时程序会core
    // dump
    // 解决办法应该为每个Channel维持一个weak_ptr指向TcpConnection，每次在handleEvent时先创建一个shared_ptr
    // 然后在该share_ptr的生命周期内完成该函数调用过程
    // 这样TcpConnection就不会过早析构
    if (revents_ & EPOLLRDHUP) {
        if (closeCallBack_)
            closeCallBack_();
    } else if (revents_ & (EPOLLIN | EPOLLPRI)) {
        if (readCallBack_)
            readCallBack_();
    } else if (revents_ & EPOLLOUT) {
        if (writeCallBack_)
            writeCallBack_();
    } else {
        if (errorCallBack_)
            errorCallBack_();
    }
}

}  // namespace sponge
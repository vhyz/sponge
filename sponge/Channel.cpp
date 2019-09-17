#include "Channel.h"
#include <sys/epoll.h>
#include <iostream>
#include "Logger.h"

namespace sponge {

void Channel::handleEvent() {
    if (revents_ & EPOLLRDHUP) {
        if (closeCallBack_)
            closeCallBack_();
    }
    if (revents_ & (EPOLLIN | EPOLLPRI)) {
        if (readCallBack_)
            readCallBack_();
    }
    if (revents_ & EPOLLOUT) {
        if (writeCallBack_)
            writeCallBack_();
    }
    if (revents_ & EPOLLERR) {
        if (errorCallBack_)
            errorCallBack_();
    }
}

}  // namespace sponge
#include "Channel.h"
#include <sys/epoll.h>
#include <iostream>

void Channel::handleEvent() {
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
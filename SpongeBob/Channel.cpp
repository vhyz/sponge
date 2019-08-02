#include "Channel.h"
#include <sys/epoll.h>
#include <iostream>

void Channel::handleEvent() {
    if (events_ & EPOLLRDHUP) {
        if (closeCallBack_)
            closeCallBack_();
    } else if (events_ & (EPOLLIN | EPOLLPRI)) {
        if (readCallBack_)
            readCallBack_();
    } else if (events_ & EPOLLOUT) {
        if (writeCallBack_)
            writeCallBack_();
    } else {
        if (errorCallBack_)
            errorCallBack_();
    }
}
#include "Channel.h"
#include <sys/epoll.h>
#include <iostream>

void Channel::handleEvent() {
    if (events_ & EPOLLRDHUP) {
        //std::cout << "close" << std::endl;
        if (closeCallBack_)
            closeCallBack_();
    } else if (events_ & (EPOLLIN | EPOLLPRI)) {
        //std::cout << "read" << std::endl;
        if (readCallBack_)
            readCallBack_();
    } else if (events_ & EPOLLOUT) {
        //std::cout << "write" << std::endl;
        if (writeCallBack_)
            writeCallBack_();
    } else {
        //std::cout << "error" << std::endl;
        //std::cout << events_ << std::endl;
        if (errorCallBack_)
            errorCallBack_();
    }
}
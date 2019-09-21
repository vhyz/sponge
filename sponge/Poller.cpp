#include "Poller.h"
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>
#include "Event.h"
#include "Logger.h"

namespace sponge {

const int InitEventListSize = 16;
const int TimeOut = 1000;

Poller::Poller()
    : epollfd_(epoll_create1(EPOLL_CLOEXEC)), eventList_(InitEventListSize) {
    if (epollfd_ < 0) {
        FATAL("epoll_create1 error");
    }
}

Poller::~Poller() { close(epollfd_); }

void Poller::poll(int timeout) {
    int numEvents =
        epoll_wait(epollfd_, eventList_.data(), eventList_.size(), timeout);

    if (numEvents < 0) {
        ERROR("epoll_wait error, errno value: %d, errno string: %s", errno,
              strerror(errno));
        return;
    }

    for (int i = 0; i < numEvents; ++i) {
        uint32_t events = eventList_[i].events;

        Event* event = static_cast<Event*>(eventList_[i].data.ptr);

        event->handleEvent(events);
    }

    if (static_cast<size_t>(numEvents) == eventList_.size()) {
        eventList_.resize(2 * eventList_.size());
    }
}

void Poller::addEvent(Event* event) {
    int fd = event->getFd();

    epoll_event ev;
    ev.data.ptr = event;
    ev.events = event->getEvents();

    INFO("%d", fd);
    INFO("%d", event->getEvents());

    if (epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &ev) < 0) {
        ERROR("epoll_ctl error");
    }
}

void Poller::updateEvent(Event* event) {
    int fd = event->getFd();

    epoll_event ev;
    ev.data.ptr = event;
    ev.events = event->getEvents();

    if (epoll_ctl(epollfd_, EPOLL_CTL_MOD, fd, &ev) < 0) {
        ERROR("epoll_ctl error");
    }
}

void Poller::removeEvent(Event* event) {
    int fd = event->getFd();

    epoll_event ev;
    ev.data.ptr = event;
    ev.events = event->getEvents();

    if (epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, &ev) < 0) {
        ERROR("epoll_ctl error");
    }
}

}  // namespace sponge
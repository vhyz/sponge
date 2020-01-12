#include "Poller.h"
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>
#include "Event.h"
#include "Logger.h"

namespace sponge {

const int InitEventListSize = 16;

Poller::Poller()
    : epollfd_(epoll_create1(EPOLL_CLOEXEC)), eventList_(InitEventListSize) {
    if (epollfd_ < 0) {
        FATAL("epoll_create1 error");
    }
}

Poller::~Poller() { close(epollfd_); }

void Poller::poll(std::vector<Event*>* activeEventList, int timeout) {
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

        uint32_t activeEvents = Event::SPEV_NONE;
        if (events & (EPOLLIN | EPOLLPRI)) {
            activeEvents |= Event::SPEV_READ;
        }
        if (events & EPOLLOUT) {
            activeEvents |= Event::SPEV_WRITE;
        }

        event->setActiveEvents(activeEvents);
        activeEventList->push_back(event);
    }

    if (static_cast<size_t>(numEvents) == eventList_.size()) {
        eventList_.resize(2 * eventList_.size());
    }
}

uint32_t eventToEpollEvent(Event* event) {
    uint32_t res = 0;
    uint32_t events = event->getEvents();
    if (events & Event::SPEV_READ) {
        res |= (EPOLLIN | EPOLLPRI);
    }
    if (events & Event::SPEV_WRITE) {
        res |= EPOLLOUT;
    }
    return res;
}

void Poller::addEvent(Event* event) {
    int fd = event->getFd();

    epoll_event ev;
    ev.data.ptr = event;
    ev.events = eventToEpollEvent(event);

    if (epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &ev) < 0) {
        ERROR("epoll_ctl error");
    }
}

void Poller::updateEvent(Event* event) {
    int fd = event->getFd();

    epoll_event ev;
    ev.data.ptr = event;
    ev.events = eventToEpollEvent(event);

    if (epoll_ctl(epollfd_, EPOLL_CTL_MOD, fd, &ev) < 0) {
        ERROR("epoll_ctl error");
    }
}

void Poller::removeEvent(Event* event) {
    int fd = event->getFd();

    epoll_event ev;
    ev.data.ptr = event;
    ev.events = eventToEpollEvent(event);

    if (epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, &ev) < 0) {
        ERROR("epoll_ctl error");
    }
}

}  // namespace sponge
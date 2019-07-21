#include "Poller.h"
#include <assert.h>
#include <unistd.h>
#include <iostream>

const int InitEventListSize = 16;
const int TimeOut = 1000;

Poller::Poller()
    : epollfd_(epoll_create1(EPOLL_CLOEXEC)),
      eventList_(InitEventListSize),
      channelMap_() {
    if (epollfd_ < 0) {
        std::cout << "epoll_create1 error" << std::endl;
        exit(1);
    }
}

Poller::~Poller() { close(epollfd_); }

void Poller::poll(std::vector<Channel*>& activeChannelList) {
    int timeout = TimeOut;

    int numEvents =
        epoll_wait(epollfd_, eventList_.data(), eventList_.size(), timeout);

    if (numEvents < 0) {
        std::cout << "epoll_wait error" << std::endl;
        return;
    }

    for (int i = 0; i < numEvents; ++i) {
        int events = eventList_[i].events;

        Channel* channel = static_cast<Channel*>(eventList_[i].data.ptr);

        auto it = channelMap_.find(channel->getFd());

        if (it != channelMap_.end() && it->second == channel) {
            channel->setEvents(events);
            activeChannelList.push_back(channel);
        }
    }

    if (static_cast<size_t>(numEvents) == eventList_.size()) {
        eventList_.resize(2 * eventList_.size());
    }
}

void Poller::addChannel(Channel* channel) {
    int fd = channel->getFd();

    epoll_event event;

    event.data.ptr = channel;
    event.events = channel->getEvents();

    channelMap_[fd] = channel;

    if (epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &event) < 0) {
        std::cout << "epoll_ctl error" << std::endl;
        exit(1);
    }
}

void Poller::updateChannel(Channel* channel) {
    int fd = channel->getFd();

    epoll_event event;
    event.data.ptr = channel;
    event.events = channel->getEvents();

    if (epoll_ctl(epollfd_, EPOLL_CTL_MOD, fd, &event) < 0) {
        std::cout << "epoll_ctl error" << std::endl;
        exit(1);
    }
}

void Poller::deleteChannel(Channel* channel) {
    int fd = channel->getFd();

    epoll_event event;
    event.data.ptr = channel;
    event.events = channel->getEvents();

    channelMap_.erase(fd);

    if (epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, &event) < 0) {
        std::cout << "epoll_ctl error" << std::endl;
        exit(1);
    }
}
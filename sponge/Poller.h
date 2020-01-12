#pragma once

#include <vector>
#include "noncopyable.h"

struct epoll_event;

namespace sponge {

class Event;

class Poller : public noncopyable {
   public:
    Poller();
    ~Poller();

    // 等待事件
    void poll(std::vector<Event*>* activeEventList, int timeout);

    // 添加事件
    void addEvent(Event* event);

    // 更新事件
    void updateEvent(Event* event);

    // 删除事件
    void removeEvent(Event* event);

   private:
    int epollfd_;

    std::vector<epoll_event> eventList_;
};

}  // namespace sponge

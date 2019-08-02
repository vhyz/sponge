#ifndef SPONGEBOB_POLL_H
#define SPONGEBOB_POLL_H

#include <sys/epoll.h>
#include <map>
#include <vector>
#include "Channel.h"

class Poller {
   public:
    Poller();
    ~Poller();

    // 等待事件
    void poll(std::vector<Channel*>& avtiveChannelList);

    // 添加事件
    void addChannel(Channel* channel);

    // 更新事件
    void updateChannel(Channel* channel);

    // 删除事件
    void deleteChannel(Channel* channel);

   private:
    int epollfd_;

    std::vector<epoll_event> eventList_;

    std::map<int, Channel*> channelMap_;
};

#endif
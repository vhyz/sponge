#include "Event.h"
#include <sys/epoll.h>

namespace sponge {

const uint32_t Event::SPEV_READ = EPOLLIN | EPOLLPRI;
const uint32_t Event::SPEV_WRITE = EPOLLOUT;
const uint32_t Event::SPEV_NONE = 0;

}  // namespace sponge
#include <iostream>
#include "../SpongeBob/EventLoop.h"

void print(const char* msg) {
    std::cout << "TimeStamp: " << getNowTimeStamp() << "   " << msg
              << std::endl;
}
EventLoop loop;
void cancel(TimerId timerId) { loop.cancel(timerId); }

int main() {
    // loop.runAfter(1, std::bind(&print, "hello,world1."));
    loop.runEvery(1, std::bind(&print, "hello,world2."));
    TimerId timerId = loop.runEvery(1, std::bind(&print, "hello,world3."));
    loop.runAfter(5, std::bind(&cancel, timerId));
    loop.loop();
}
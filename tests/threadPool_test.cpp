#include <SpongeBob/EventLoop.h>
#include <SpongeBob/Logger.h>
#include <SpongeBob/ThreadPool.h>

int main() {
    ThreadPool pool(5);
    pool.start();
    for (int i = 0; i < 100; ++i)
        pool.putTask([=]() { INFO("%d", std::this_thread::get_id()); });
}
#include <sponge/EventLoop.h>
#include <sponge/Logger.h>
#include <sponge/ThreadPool.h>

using namespace sponge;

int main() {
    ThreadPool pool(5);
    pool.start();
    for (int i = 0; i < 100; ++i)
        pool.putTask([=]() { INFO("%d", std::this_thread::get_id()); });
}
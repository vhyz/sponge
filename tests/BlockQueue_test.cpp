#include <sponge/BlockQueue.h>
#include <sponge/EventLoop.h>
#include <sponge/ThreadPool.h>
#include <iostream>

using namespace std;
using namespace sponge;

int main() {
    EventLoop loop;
    ThreadPool pool(10);
    pool.start();
    loop.runEvery(1, [&]() {
        for (size_t i = 0; i < 1000; ++i) {
            pool.putTask([=]() {
                if (i == 999)
                    cout << std::this_thread::get_id() << endl;
            });
        }
    });
    loop.loop();
}
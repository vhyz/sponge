
add_library(
    SpongeBob
    BlockQueue.cpp
    Channel.cpp
    EchoServer.cpp
    EventLoop.cpp
    EventLoopThread.cpp
    EventLoopThreadPool.cpp
    Poller.cpp
    TcpConnection.cpp
    TcpServer.cpp
    ThreadPool.cpp
    TimerQueue.cpp
    TimeStamp.cpp
 )


target_link_libraries(SpongeBob pthread)
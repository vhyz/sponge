
add_library(
    SpongeBob
    Channel.cpp
    TimeStamp.cpp
    EventLoop.cpp
    EventLoopThreadPool.cpp
    Poller.cpp
    EchoServer.cpp
    TcpConnection.cpp
    TcpServer.cpp
    TimerQueue.cpp
    EventLoopThread.cpp
 )


target_link_libraries(SpongeBob pthread)
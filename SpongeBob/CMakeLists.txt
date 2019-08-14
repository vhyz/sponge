
add_library(
    SpongeBob
    BlockQueue.cpp
    Channel.cpp
    Connector.cpp
    EventLoop.cpp
    EventLoopThread.cpp
    EventLoopThreadPool.cpp
    InetAddress.cpp
    Poller.cpp
    TcpClient.cpp
    TcpConnection.cpp
    TcpServer.cpp
    ThreadPool.cpp
    TimerQueue.cpp
    TimeStamp.cpp
 )


target_link_libraries(SpongeBob pthread)
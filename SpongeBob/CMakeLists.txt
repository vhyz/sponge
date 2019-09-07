
add_library(
    SpongeBob
    BlockQueue.cpp
    Channel.cpp
    Connector.cpp
    EventLoop.cpp
    EventLoopThread.cpp
    EventLoopThreadPool.cpp
    InetAddress.cpp
    Logger.cpp
    Poller.cpp
    TcpClient.cpp
    TcpConnection.cpp
    TcpServer.cpp
    ThreadPool.cpp
    TimerManager.cpp
    TimeStamp.cpp
 )


target_link_libraries(SpongeBob pthread)
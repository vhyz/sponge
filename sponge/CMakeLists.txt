add_subdirectory(http)

add_library(
    sponge
    BlockQueue.cpp
    Connector.cpp
    Event.cpp
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


target_link_libraries(sponge pthread)
target_link_libraries(sponge sponge_http)
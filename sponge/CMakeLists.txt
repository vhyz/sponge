add_subdirectory(http)

add_library(
    sponge
    BlockQueue.cpp
    Connector.cpp
    EventLoop.cpp
    InetAddress.cpp
    Logger.cpp
    Poller.cpp
    TcpClient.cpp
    TcpConnection.cpp
    TcpServer.cpp
    ThreadPool.cpp
    Timer.cpp
    TimerManager.cpp
 )


target_link_libraries(sponge pthread)
target_link_libraries(sponge sponge_http)
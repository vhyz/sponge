
add_library(
    SpongeBob
    Channel.cpp
    EventLoop.cpp
    EventLoopThreadPool.cpp
    Poller.cpp
    EchoServer.cpp
    TcpConnection.cpp
    TcpServer.cpp
 )


target_link_libraries(SpongeBob pthread)

add_executable(
    SpongeBob_test
    test.cpp
)

target_link_libraries(SpongeBob_test SpongeBob)
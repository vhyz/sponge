cmake_minimum_required(VERSION 3.10)
project(SpongeBob)

set(CMAKE_CXX_STANDARD 17)

add_definitions("-Wall -g")
add_executable(
    SpongeBob
    Channel.cpp
    EventLoop.cpp
    EventLoopThreadPool.cpp
    Poller.cpp
    test.cpp
    EchoServer.cpp
    TcpConnection.cpp
    TcpServer.cpp
    HttpTestServer.cpp
 )


target_link_libraries(SpongeBob pthread)
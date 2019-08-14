add_executable(
    echoServer
    echoServer.cpp
)

add_executable(
    echoClient
    echoClient.cpp
)

target_link_libraries(echoServer SpongeBob)
target_link_libraries(echoClient SpongeBob)
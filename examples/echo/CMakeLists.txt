add_executable(
    echoServer
    echoServer.cpp
)

add_executable(
    echoClient
    echoClient.cpp
)

target_link_libraries(echoServer sponge)
target_link_libraries(echoClient sponge)
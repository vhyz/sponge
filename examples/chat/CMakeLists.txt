add_executable(
    chatServer
    chatServer.cpp
)

add_executable(
    chatClient
    chatClient.cpp
)

target_link_libraries(chatServer sponge)
target_link_libraries(chatClient sponge)
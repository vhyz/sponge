add_executable(
    chatServer
    chatServer.cpp
)

add_executable(
    chatClient
    chatClient.cpp
)

target_link_libraries(chatServer SpongeBob)
target_link_libraries(chatClient SpongeBob)
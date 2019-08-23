add_executable(
    addServer
    addServer.cpp
)

add_executable(
    addClient
    addClient.cpp
)

target_link_libraries(addServer SpongeBob)
target_link_libraries(addClient SpongeBob)
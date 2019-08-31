add_subdirectory(echo)
add_subdirectory(chat)
add_subdirectory(sort)

add_executable(
    http_echo
    http_echo.cpp
)

target_link_libraries(http_echo SpongeBob)
add_subdirectory(echo)
add_subdirectory(add)

add_executable(
    http_echo
    http_echo.cpp
)

target_link_libraries(http_echo SpongeBob)
add_executable(
    TimerQueue_test
    TimerQueue_test.cpp
)
target_link_libraries(TimerQueue_test sponge)

add_executable(
    CircularBuffer_test
    CircularBuffer_test.cpp
)

add_executable(
    BlockQueue_test
    BlockQueue_test.cpp
)
target_link_libraries(BlockQueue_test sponge)

add_executable(
    logger_test
    logger_test.cpp
)
target_link_libraries(logger_test sponge)

add_executable(
    threadPool_test
    threadPool_test.cpp
)
target_link_libraries(threadPool_test sponge)

add_executable(
    ChannelBuffer_test
    ChannelBuffer_test.cpp
)
target_link_libraries(ChannelBuffer_test sponge)

add_executable(
    SharedBuffer_test
    SharedBuffer_test.cpp
)
target_link_libraries(SharedBuffer_test sponge)
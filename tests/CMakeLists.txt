add_executable(
    TimerQueue_test
    TimerQueue_test.cpp
)
target_link_libraries(TimerQueue_test SpongeBob)

add_executable(
    CircularBuffer_test
    CircularBuffer_test.cpp
)

add_executable(
    BlockQueue_test
    BlockQueue_test.cpp
)
target_link_libraries(BlockQueue_test SpongeBob)

add_executable(
    logger_test
    logger_test.cpp
)
target_link_libraries(logger_test SpongeBob)

add_executable(
    threadPool_test
    threadPool_test.cpp
)
target_link_libraries(threadPool_test SpongeBob)
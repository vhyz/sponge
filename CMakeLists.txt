cmake_minimum_required(VERSION 3.9)
project(sponge)

set(CMAKE_CXX_STANDARD 17)
set(CXX_FLAGS -g)


set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(sponge)
add_subdirectory(tests)
add_subdirectory(examples)
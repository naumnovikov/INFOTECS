cmake_minimum_required(VERSION 3.16)
project(INFOTECS_Project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

# Static by default. Use "cmake -B build -DBUILD_SHARED_LIBS=ON" to compile dynamic.
option(BUILD_SHARED_LIBS "Build library as shared (dynamic)" OFF)

add_subdirectory(src/user_app)
add_subdirectory(src/lib)

add_subdirectory(src/stat_app)
enable_testing()
add_subdirectory(tests)
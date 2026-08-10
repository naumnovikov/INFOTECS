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
option(BUILD_LIB        "Build library" OFF)
option(BUILD_USER_APP   "Build user application" OFF)
option(BUILD_STAT_APP   "Build statistics application" OFF)
option(BUILD_TESTS      "Build tests" OFF)

if(BUILD_LIB)
    add_subdirectory(src/lib)
endif()

if(BUILD_USER_APP)
    add_subdirectory(src/user_app)
endif()

if(BUILD_STAT_APP)
    add_subdirectory(src/stat_app)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(BUILD_USER_APP OR BUILD_TESTS)
    if(NOT BUILD_LIB)
        message(FATAL_ERROR "BUILD_LIB must be enabled for user application or tests")
    endif()
endif()
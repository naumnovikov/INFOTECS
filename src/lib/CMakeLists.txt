add_library(LIB 
    file_logger.cpp 
    socket_logger.cpp
)

target_include_directories(LIB PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/lib
)

target_link_libraries(LIB PUBLIC Threads::Threads)
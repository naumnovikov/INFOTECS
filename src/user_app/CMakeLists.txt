set(FA_SOURCES
    ${CMAKE_SOURCE_DIR}/src/common/business_worker.cpp          
    ${CMAKE_SOURCE_DIR}/src/user_app/user_controller.cpp
    ${CMAKE_SOURCE_DIR}/src/user_app/menu/change.cpp
    ${CMAKE_SOURCE_DIR}/src/user_app/menu/help.cpp
    ${CMAKE_SOURCE_DIR}/src/user_app/menu/log.cpp
    ${CMAKE_SOURCE_DIR}/src/user_app/main_UA.cpp
)

add_executable(FA ${FA_SOURCES})

target_include_directories(FA PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/common
    ${CMAKE_SOURCE_DIR}/include/user_app
    ${CMAKE_SOURCE_DIR}/include/user_app/menu
    ${CMAKE_SOURCE_DIR}/include/lib
)

target_link_libraries(FA PRIVATE LIB Threads::Threads)
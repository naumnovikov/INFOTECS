add_executable(stat_app
    main_SA.cpp
    stat_controller.cpp
)

target_include_directories(stat_app PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/stat_app
)

target_link_libraries(stat_app PRIVATE Threads::Threads)
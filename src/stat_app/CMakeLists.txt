add_executable(SA
    main_SA.cpp
    stat_controller.cpp
)

target_include_directories(SA PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/stat_app
)

target_link_libraries(SA PRIVATE Threads::Threads)
add_executable(TESTS 
    main_TEST.cpp
)

target_link_libraries(TESTS PRIVATE LIB)

add_test(NAME LoggerTests COMMAND TESTS)
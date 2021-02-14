add_executable(tests
    main.cc
    test_calc.cc
)
target_link_libraries(
    tests
    PUBLIC
    calculator
    catchy
    ${CMAKE_THREAD_LIBS_INIT}
    PRIVATE
    project_options
    project_warnings
)

include(CTest)
catch_discover_tests(tests)

add_executable(dscanner_test src/dscanner_test.cpp)
target_link_libraries(dscanner_test dscanner GTest::Main)
add_test(NAME dscanner_test 
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND dscanner_test
        )

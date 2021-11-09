add_executable(fscanner_test src/fscanner_test.cpp)
target_link_libraries(fscanner_test fscanner GTest::Main)
add_test(NAME fscanner_test 
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND fscanner_test
        )

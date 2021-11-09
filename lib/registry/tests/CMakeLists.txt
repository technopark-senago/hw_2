add_executable(registry_test src/registry_test.cpp)
target_link_libraries(registry_test registry GTest::Main)
add_test(NAME registry_test 
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND registry_test
        )

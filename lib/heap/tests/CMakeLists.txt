add_executable(heap_test src/heap_test.cpp)
target_link_libraries(heap_test heap GTest::Main)
add_test(NAME heap_test COMMAND heap_test)

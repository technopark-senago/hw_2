if(PARALLEL)
  set(DSCANNER_LIB "parallel")
  set(DSCANNER_TYPE "SHARED")
else()
  set(DSCANNER_LIB "sequential")
  set(DSCANNER_TYPE "STATIC")
endif()
  
add_library(dscanner ${DSCANNER_TYPE} src/${DSCANNER_LIB}/dscanner.c)
target_include_directories(dscanner PUBLIC include)
target_link_libraries(dscanner PUBLIC heap fscanner registry errors)

install(
    TARGETS dscanner
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

if(ENABLE_TESTS)
    add_subdirectory(tests)
endif()

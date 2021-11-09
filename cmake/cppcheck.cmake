find_program(CMAKE_CXX_CPPCHECK cppcheck)
if(CMAKE_CXX_CPPCHECK)
  list(
    APPEND CMAKE_CXX_CPPCHECK
    "--enable=all" "--quiet"
    "--suppress=missingInclude" "--suppress=unusedFunction"
    "lib"
  )
    
  add_custom_target(cppcheck ALL DEPENDS ${PROJECT_NAME}
    COMMAND ${CMAKE_CXX_CPPCHECK}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Static code analysis using cppcheck"
  )
endif()

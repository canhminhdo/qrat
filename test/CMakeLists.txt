# building googletest as a part of project
if(NOT TARGET gtest OR NOT TARGET gmock)
    # Prevent overriding the parent project's compiler/linker settings on Windows
    set(gtest_force_shared_crt # cmake-lint: disable=C0103
        ON
        CACHE BOOL "" FORCE)
    add_subdirectory("${PROJECT_SOURCE_DIR}/extern/googletest" "extern/googletest" EXCLUDE_FROM_ALL)
    set_target_properties(gtest gtest_main gmock gmock_main PROPERTIES FOLDER extern)
endif()

message("Building tests for QRAT project")

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/testScript.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(GLOB DIRECTORIES LIST_DIRECTORIES true "${CMAKE_CURRENT_SOURCE_DIR}/*")
foreach(DIR ${DIRECTORIES})
    if(IS_DIRECTORY ${DIR})
        file(COPY ${DIR} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    endif()
endforeach()

## for testing
package_add_tests(
    ${PROJECT_NAME}_test ""
    test_programs.cpp
)
# building googletest as a part of project
if(NOT TARGET gtest OR NOT TARGET gmock)
    # Prevent overriding the parent project's compiler/linker settings on Windows
    set(gtest_force_shared_crt # cmake-lint: disable=C0103
        ON
        CACHE BOOL "" FORCE)
    add_subdirectory("${PROJECT_SOURCE_DIR}/extern/googletest" "extern/googletest" EXCLUDE_FROM_ALL)
#    set_target_properties(gtest gtest_main gmock gmock_main PROPERTIES FOLDER extern)
endif()

message("Building tests for QRAT project")

## for testing
#add_executable(appTest
#    app_test.cpp)
#
## for production
#add_executable(main
#    main.cpp)
#
#target_link_libraries(main ${PROJECT_NAME})
#target_link_libraries(appTest ${PROJECT_NAME} gtest_main)
#gtest_discover_tests(appTest)

## for production
add_executable(bootstrap
    bootstrap.cpp)

package_add_test(
    ${PROJECT_NAME}_test
    Parser
    main.cpp
    test.cpp
)
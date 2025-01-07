configure_file(${PROJECT_SOURCE_DIR}/examples/teleport/teleport.qw prog.qw)
configure_file(${PROJECT_SOURCE_DIR}/examples/loop/prog-loop.qw prog-loop.qw)
configure_file(${PROJECT_SOURCE_DIR}/examples/grover/grover.qw grover.qw)

add_executable(
    ${PROJECT_NAME}
    main/main.cpp
    utility/printUtils.cpp
)

if(FORMAT_SOURCE_CODE)
    set(FORMAT_DIRECTORIES
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/test
    )
    Format(${PROJECT_NAME} ${FORMAT_DIRECTORIES})
endif()

if(CLANG_TIDY_ANALYSIS)
    AddClangTidy(${PROJECT_NAME})
endif()

# set include directories
include_directories(
    ${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/include)

target_link_libraries(${PROJECT_NAME} Parser)

add_subdirectory(parser)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/dist CACHE STRING "Install prefix" FORCE)
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
# install the project and some quantum programs
install(TARGETS ${PROJECT_NAME}
        RUNTIME
        DESTINATION .)
install(
        FILES
        ${PROJECT_SOURCE_DIR}/examples/teleport/teleport.qw
        ${PROJECT_SOURCE_DIR}/examples/loop/prog-loop.qw
        ${PROJECT_SOURCE_DIR}/examples/grover/grover.qw
        DESTINATION .)

# CPack Configuration
include(CPack)
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_VENDOR ${PROJECT_VENDOR})
set(CPACK_PACKAGE_CONTACT ${PROJECT_BUGREPORT})
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}")
set(CPACK_GENERATOR "ZIP" CACHE STRING "Set package format" FORCE) # Use .zip for Windows, MacOS, and Linux
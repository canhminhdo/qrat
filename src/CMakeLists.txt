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
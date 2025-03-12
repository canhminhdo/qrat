configure_file(${PROJECT_SOURCE_DIR}/examples/teleport/teleport.qw prog.qw)
configure_file(${PROJECT_SOURCE_DIR}/examples/loop/loop.qw loop.qw)
configure_file(${PROJECT_SOURCE_DIR}/examples/grover/grover.qw grover.qw)

add_executable(
    ${PROJECT_NAME}
    main/main.cpp
    utility/printUtils.cpp
)

# set include directories
include_directories(
    ${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/include)

target_link_libraries(${PROJECT_NAME} Parser)

add_subdirectory(parser)
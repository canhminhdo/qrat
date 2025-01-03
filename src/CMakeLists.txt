configure_file(${PROJECT_SOURCE_DIR}/generator/teleport/teleport.qw prog.qw)
configure_file(${PROJECT_SOURCE_DIR}/generator/loop/prog-loop.qw prog-loop.qw)
configure_file(${PROJECT_SOURCE_DIR}/generator/grover/grover.qw grover.qw)

add_executable(
    ${PROJECT_NAME}
    main/main.cpp
    main/printInfo.cpp
)

# set include directories
include_directories(
    ${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/include)

# link to the MQT::Core libararies
#target_link_libraries(${PROJECT_NAME} PUBLIC MQT::CoreDD MQT::CoreZX Parser)

target_link_libraries(${PROJECT_NAME} Parser)

add_subdirectory(parser)
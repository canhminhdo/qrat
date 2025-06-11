configure_file(${PROJECT_SOURCE_DIR}/examples/teleport/teleport.qw teleport.qw)
configure_file(${PROJECT_SOURCE_DIR}/src/parser/prog.qw prog.qw)
configure_file(${PROJECT_SOURCE_DIR}/examples/loop/loop.qw loop.qw)
configure_file(${PROJECT_SOURCE_DIR}/examples/grover/grover.qw grover.qw)
configure_file(${PROJECT_SOURCE_DIR}/examples/secretsharing/secretsharing.qw secretsharing.qw)
configure_file(${PROJECT_SOURCE_DIR}/examples/replay/replay.qw replay.qw)
configure_file(${PROJECT_SOURCE_DIR}/examples/biteleport/biteleport.qw biteleport.qw)
configure_file(${PROJECT_SOURCE_DIR}/examples/entangleswap/entangleswap.qw entangleswap.qw)
configure_file(${PROJECT_SOURCE_DIR}/examples/networkcoding/networkcoding.qw networkcoding.qw)
configure_file(${PROJECT_SOURCE_DIR}/examples/wmgrover/wmgrover.qw wmgrover.qw)

add_executable(
    ${PROJECT_NAME}
    main/main.cpp
    utility/printUtils.cpp
    model/DTMC.cpp
    model/PrismRunner.cpp
    model/Runner.cpp
    model/StormRunner.cpp
)

# set include directories
include_directories(
    ${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/include)

target_link_libraries(${PROJECT_NAME} Parser)

add_subdirectory(parser)
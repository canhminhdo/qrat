set(QW_EXAMPLE_FILES
    examples/teleport/teleport.qw
    src/parser/prog.qw
    examples/loop/loop.qw
    examples/grover/grover.qw
    examples/secretsharing/secretsharing.qw
    examples/relay/relay.qw
    examples/biteleport/biteleport.qw
    examples/entangleswap/entangleswap.qw
    examples/networkcoding/networkcoding.qw
    examples/wmgrover/wmgrover.qw
)

foreach(qw_file IN LISTS QW_EXAMPLE_FILES)
    get_filename_component(qw_name "${qw_file}" NAME)
    configure_file(
        "${PROJECT_SOURCE_DIR}/${qw_file}"
        "${qw_name}"
        COPYONLY
    )
endforeach()

add_executable(
    ${PROJECT_NAME}
    main/main.cpp
    utility/printUtils.cpp
    model/DTMC.cpp
    model/PrismRunner.cpp
    model/Runner.cpp
    model/StormRunner.cpp
)

# Warning suppression policy should not be forced on consumers
target_compile_options(${PROJECT_NAME} PRIVATE -Wno-deprecated-literal-operator)

# set include directories
target_include_directories(
    ${PROJECT_NAME} PRIVATE
    ${PROJECT_BINARY_DIR}/include
)

add_subdirectory(parser)

target_link_libraries(${PROJECT_NAME} PRIVATE Parser)
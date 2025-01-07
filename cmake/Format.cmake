function(Format target directories)
    find_program(CLANG_FORMAT_PATH clang-format REQUIRED)
    set(ALL_FILES)
    foreach(directory ${directories})
        file(GLOB_RECURSE CPP_FILES "${directory}/*.cpp")
        file(GLOB_RECURSE HPP_FILES "${directory}/*.hpp")
        list(APPEND ALL_FILES ${CPP_FILES} ${HPP_FILES})
    endforeach()
    add_custom_command(TARGET ${target} PRE_BUILD COMMAND
        ${CLANG_FORMAT_PATH} --style=file -i ${ALL_FILES}
    )
endfunction()
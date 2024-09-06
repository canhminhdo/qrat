function(Doxygen input output)
    find_package(Doxygen REQUIRED)
    message("Generate HTML document 2")
    if(NOT DOXYGEN_FOUND)
        add_custom_target(doxygen COMMAND false
            COMMENT "Doxygen not found")
        return()
    endif()
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_HTML_OUTPUT
        ${PROJECT_BINARY_DIR}/${output})
    set(DOXYGEN_EXCLUDE_PATTERNS
        ${PROJECT_SOURCE_DIR}/extern
        ${PROJECT_SOURCE_DIR}/cmake
        ${PROJECT_SOURCE_DIR}/doc
        ${PROJECT_SOURCE_DIR}/sample
        ${PROJECT_SOURCE_DIR}/test
        ${PROJECT_SOURCE_DIR}/build
    )
    doxygen_add_docs(doxygen
        ${PROJECT_SOURCE_DIR}
        COMMENT "Generate HTML document")
endfunction()
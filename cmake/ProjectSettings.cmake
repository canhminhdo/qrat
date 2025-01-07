if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed")
endif()

set(BUILD_QRAT_TESTS
    ON CACHE BOOL "Build QRAT tests" FORCE)

set(BUILD_QRAT_DOC
    OFF CACHE BOOL "Build QRAT document" FORCE)

set(FORMAT_SOURCE_CODE
    ON CACHE BOOL "Format source code for the project" FORCE)

set(CLANG_TIDY_ANALYSIS
    OFF CACHE BOOL "Analysis the source code with clang-tidy for the project" FORCE)

macro(TODAY RESULT)
    if(WIN32)
        execute_process(COMMAND "date" OUTPUT_VARIABLE ${RESULT})
        string(REGEX REPLACE "\n" "" ${RESULT} ${${RESULT}})
    elseif(UNIX)
        execute_process(COMMAND "date" "+%b %d %Y %T" OUTPUT_VARIABLE ${RESULT})
        string(REGEX REPLACE "\n" "" ${RESULT} ${${RESULT}})
    else()
        message(SEND_ERROR "date not implemented")
        set(${RESULT} "unknown")
    endif(WIN32)
endmacro()

today(RESULT)
set(PROJECT_BUILD_DATE ${RESULT})
set(PROJECT_BUGREPORT "canhdo@jaist.ac.jp")
set(PROJECT_VENDOR "Ogata Laboratory (JAIST)")

# generate a header file with the information from the project
configure_file(
    ${PROJECT_SOURCE_DIR}/include/config.h.in
    ${PROJECT_BINARY_DIR}/include/config.h
    @ONLY
)
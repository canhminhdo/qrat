if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed")
endif()

set(BUILD_QRAT_TESTS
    ON CACHE BOOL "Build QRAT tests" FORCE)

set(BUILD_QRAT_DOC
    OFF CACHE BOOL "Build QRAT document" FORCE)

MACRO (TODAY RESULT)
    IF (WIN32)
        execute_process(COMMAND "date" OUTPUT_VARIABLE ${RESULT})
        string(REGEX REPLACE "\n" "" ${RESULT} ${${RESULT}})
    ELSEIF(UNIX)
        execute_process(COMMAND "date" "+%b %d %Y %T" OUTPUT_VARIABLE ${RESULT})
        string(REGEX REPLACE "\n" "" ${RESULT} ${${RESULT}})
    ELSE (WIN32)
        message(SEND_ERROR "date not implemented")
        set(${RESULT} "unknown")
    ENDIF (WIN32)
ENDMACRO (TODAY)

today(RESULT)
set(PROJECT_BUILD_DATE ${RESULT})
set(PROJECT_BUGREPORT "canhdo@jaist.ac.jp")

# generate a header file with the information from the project
configure_file(
    ${PROJECT_SOURCE_DIR}/include/config.h.in
    ${PROJECT_BINARY_DIR}/include/config.h
    @ONLY
)
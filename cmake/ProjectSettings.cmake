if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed")
endif()

set(BUILD_QRAT_TESTS
    ON CACHE BOOL "Build QRAT tests" FORCE)

set(BUILD_QRAT_DOC
    OFF CACHE BOOL "Build QRAT document" FORCE)
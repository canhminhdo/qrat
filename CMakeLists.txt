cmake_minimum_required(VERSION 3.20..3.26)
project(qrat
    VERSION 1.0
    DESCRIPTION "QRAT - A Reachability Analysis Tool for Quantum Programs"
    HOMEPAGE_URL "https://github.com/canhminhdo/QRAT"
    LANGUAGES CXX
)

# project settings
include(cmake/ProjectSettings.cmake)
# set c++ standard for the project
include(cmake/CompilerOptions.cmake)
# check external dependencies
include(cmake/ExternalDependencies.cmake)
# for doxygen documentation
include(cmake/Doxygen.cmake)
# package add test macro
include(cmake/PackageAddTests.cmake)
# format source codes
include(cmake/Format.cmake)
# source code analysis with clang-tidy
include(cmake/ClangTidy.cmake)

add_subdirectory(src)
include_directories(include)

# add documentation
option(BUILD_QRAT_DOC "Build documentation for the QRAT project" ON) # only sets value if it is not already defined
if(BUILD_QRAT_DOC)
    add_subdirectory(doc)
endif()

# add test code
option(BUILD_QRAT_TESTS "Build tests for the QRAT project" ON)
if(BUILD_QRAT_TESTS)
    enable_testing()
    include(GoogleTest) # this modules defines functions to automatically discover test cases for Cmake with the use of GoogleTest
    add_subdirectory(test)
endif()

# add format source code
option(FORMAT_SOURCE_CODE "Format source code for QRAT" ON)
if(FORMAT_SOURCE_CODE)
    set(FORMAT_DIRECTORIES
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/test
    )
    Format(${PROJECT_NAME} ${FORMAT_DIRECTORIES})
endif()

# add clang-tidy analysis
option(CLANG_TIDY_ANALYSIS "Clang-tidy analysis for QRAT" ON)
if(CLANG_TIDY_ANALYSIS)
    AddClangTidy(${PROJECT_NAME})
    AddClangTidy(Parser)
endif()

# add install & package
include(cmake/InstallAndPackage.cmake)
# set the install prefix
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install)
# install the project with some quantum programs
install(TARGETS ${PROJECT_NAME}
    RUNTIME
    DESTINATION .)
install(
    FILES
    ${PROJECT_SOURCE_DIR}/examples/teleport/teleport.qw
    ${PROJECT_SOURCE_DIR}/examples/loop/loop.qw
    ${PROJECT_SOURCE_DIR}/examples/grover/grover.qw
    ${PROJECT_SOURCE_DIR}/examples/biteleport/biteleport.qw
    ${PROJECT_SOURCE_DIR}/examples/entangleswap/entangleswap.qw
    ${PROJECT_SOURCE_DIR}/examples/networkcoding/networkcoding.qw
    ${PROJECT_SOURCE_DIR}/examples/relay/relay.qw
    ${PROJECT_SOURCE_DIR}/examples/secretsharing/secretsharing.qw
    DESTINATION .)

# CPack Configuration
include(CPack)
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_VENDOR ${PROJECT_VENDOR})
set(CPACK_PACKAGE_CONTACT ${PROJECT_BUGREPORT})
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}")
set(CPACK_GENERATOR "ZIP") # Use .zip for Windows, MacOS, and Linux
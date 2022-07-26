# Target name
project(faber_io)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Create library target
add_library(
    ${PROJECT_NAME} SHARED
    "${INCLUDE_DIR}/io.h"
    "${SRC_DIR}/file_monitor.cpp"
)
set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 20
)

# Compiler setup
target_include_directories(
    ${PROJECT_NAME}
    PRIVATE ${INCLUDE_DIR}
)

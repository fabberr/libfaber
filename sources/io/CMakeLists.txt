# Target name
project(faber_io CXX)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Create library target
add_library(
	${PROJECT_NAME} SHARED
	"${INCLUDE_DIR}/io.h"
	"${SRC_DIR}/io.cpp"
)

set_target_properties(
	${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 20
)

# Compiler setup
target_include_directories(
	${PROJECT_NAME}
	PRIVATE ${INCLUDE_DIR}
)

# Target name
project(test)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

# Create library target
add_executable(
	${PROJECT_NAME}
	"${INCLUDE_DIR}/main.cpp"
)
set_target_properties(
	${PROJECT_NAME} PROPERTIES
	LINKER_LANGUAGE CXX
	CXX_STANDARD 20
)

# Compiler setup
target_link_libraries(${PROJECT_NAME} PRIVATE faber_io)
target_include_directories(
	${PROJECT_NAME}
	PRIVATE ${INCLUDE_DIR}
	PRIVATE "${CMAKE_SOURCE_DIR}/io/include"
)

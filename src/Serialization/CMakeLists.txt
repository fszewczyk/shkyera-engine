find_package(protobuf CONFIG REQUIRED)

add_library(
	shkyera-proto
	OBJECT
	${CMAKE_CURRENT_LIST_DIR}/Registry.proto
)

target_link_libraries(
	shkyera-proto
	PUBLIC
	protobuf::libprotobuf
)

set(PROTO_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(
	shkyera-proto
	PUBLIC
	"$<BUILD_INTERFACE:${PROTO_BINARY_DIR}>"
)

protobuf_generate(
    TARGET shkyera-proto
    IMPORT_DIRS "${CMAKE_CURRENT_LIST_DIR}"
    PROTOC_OUT_DIR "${PROTO_BINARY_DIR}")

add_library(
	Serialization
	STATIC

	${CMAKE_CURRENT_LIST_DIR}/Builders.cpp
)

target_include_directories(
	Serialization
	PUBLIC

	${CMAKE_SOURCE_DIR}/src
)

target_include_directories(
	Serialization
	PRIVATE

	${CMAKE_CURRENT_BINARY_DIR}
	${PROTO_BINARY_DIR}
)

target_link_libraries(
	Serialization
	PRIVATE
	
	shkyera-proto
	ECS
	Components
)


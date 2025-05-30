cmake_minimum_required(VERSION 3.31.6)
project(
	chess
	VERSION 1.0
	LANGUAGES C
	DESCRIPTION "A chess library written in c"
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(chess src/chess.c)
target_include_directories(chess PUBLIC include)
target_compile_options(
	chess
	PRIVATE -Werror
			-Wall
			-Wextra
			-pedantic
			-Wfloat-equal
			-Wundef
			-Wshadow
			-Wpointer-arith
			-Wcast-align
			-Wstrict-prototypes
			-Wstrict-overflow=5
			-Wwrite-strings
			-Wcast-qual
			-Wconversion
			-Wno-enum-enum-conversion
)

add_executable(main src/main.c)
target_link_libraries(main chess)
target_compile_options(
	main
	PRIVATE -Werror
			-Wall
			-Wextra
			-pedantic
			-Wfloat-equal
			-Wundef
			-Wshadow
			-Wpointer-arith
			-Wcast-align
			-Wstrict-prototypes
			-Wstrict-overflow=5
			-Wwrite-strings
			-Wcast-qual
			-Wconversion
)

if(UNIT_TESTING)
	list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmocka)

	find_package(cmocka REQUIRED)

	include(AddCMockaTest)

	add_subdirectory(tests)
endif(UNIT_TESTING)

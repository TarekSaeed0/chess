cmake_minimum_required(VERSION 3.31.6)
project(
	chess
	VERSION 1.0
	LANGUAGES C
	DESCRIPTION "A chess library written in c"
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(
	chess
	src/chess/color.c
	src/chess/piece_type.c
	src/chess/piece.c
	src/chess/file.c
	src/chess/rank.c
	src/chess/square.c
	src/chess/castling_rights.c
	src/chess/position.c
	src/chess/position_counter.c
	src/chess/move.c
	src/chess/moves.c
)
target_include_directories(
	chess
	PUBLIC include
	PRIVATE src
)
if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
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
	)
elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
	target_compile_options(chess PRIVATE /WX /W4)
endif()

add_executable(main src/main.c)
target_link_libraries(main chess)
if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
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
	)
elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
	target_compile_options(main PRIVATE /WX /W4)
endif()

if(UNIT_TESTING)
	list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmocka)

	find_package(cmocka REQUIRED)

	include(AddCMockaTest)

	add_subdirectory(tests)
endif(UNIT_TESTING)

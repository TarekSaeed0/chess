set(CMOCKA_TESTS color piece_type piece file rank square move moves)

foreach(_CMOCKA_TEST ${CMOCKA_TESTS})
	add_cmocka_test(
		${_CMOCKA_TEST}
		SOURCES
		chess/${_CMOCKA_TEST}.c
		COMPILE_OPTIONS
		${DEFAULT_C_COMPILE_FLAGS}
		LINK_LIBRARIES
		cmocka::cmocka
		chess
		LINK_OPTIONS
		${DEFAULT_LINK_FLAGS}
	)
	target_include_directories(
		${_CMOCKA_TEST} PRIVATE ../include ../src ${cmocka_BINARY_DIR}
	)

	add_cmocka_test_environment(${_CMOCKA_TEST})
endforeach()

set(CMOCKA_TESTS color piece_type piece file rank square position_counter move moves)

if(ENABLE_COVERAGE)
	list(APPEND DEFAULT_C_COMPILE_FLAGS "--coverage")
	list(APPEND DEFAULT_LINK_FLAGS "--coverage")
endif()

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

if(ENABLE_COVERAGE)
	find_program(GCOVR_BIN gcovr)
	if(GCOVR_BIN)
		add_custom_target(coverage
			COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
			COMMAND ${GCOVR_BIN} -r ${CMAKE_CURRENT_SOURCE_DIR} . --gcov-executable "llvm-cov gcov" --txt --html-details coverage.html
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
			COMMENT "Running tests and generating coverage report..."
		)
	else()
		message(WARNING "gcovr not found. Install it to generate automated reports.")
	endif()
endif()

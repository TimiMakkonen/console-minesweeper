
set(TEST_SOURCE_LIST
	console_minesweeper_tests.cpp
)

set(TEST_APP_NAME "ConsoleMinesweeperTests")
add_executable("${TEST_APP_NAME}" ${TEST_SOURCE_LIST})

target_link_libraries("${TEST_APP_NAME}" 
	gtest_main
	"${LIBRARY_NAME}"
)

add_test(
	NAME "${TEST_APP_NAME}"
	COMMAND "${TEST_APP_NAME}"
)


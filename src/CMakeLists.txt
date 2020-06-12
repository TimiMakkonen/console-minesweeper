
# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

# Public header files
set(HEADER_FOLDER "${PROJECT_SOURCE_DIR}/include/console-minesweeper")
set(HEADER_LIST
    "${HEADER_FOLDER}/game.h"
)

# Private header files
set(PRIVATE_HEADER_FOLDER "console-minesweeper")
set(PRIVATE_HEADER_LIST
    # Add private header files of the library here
)

# Source files
set(SOURCE_FOLDER "console-minesweeper")
set(SOURCE_LIST
    "${SOURCE_FOLDER}/game.cpp"
)



# Make an automatic library - will be static or dynamic based on user setting
add_library("${LIBRARY_NAME}"
            ${HEADER_LIST}
            ${PRIVATE_HEADER_LIST}
            ${SOURCE_LIST}
)

# We need this directory, and users of our library will need it too
target_include_directories("${LIBRARY_NAME}" PUBLIC "${PROJECT_SOURCE_DIR}/include")

# We need this directory (private headers)
target_include_directories("${LIBRARY_NAME}" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

# This console minesweeper library depends on minesweeper library
target_link_libraries("${LIBRARY_NAME}"
	PUBLIC # Needs to be public to use 'Random' in main app
		# Consider adding new 'Random' to this lib to remove need to make 'libminesweeper' headers available publicly
	libminesweeper
)

# All users of this library will need at least C++17
target_compile_features(${LIBRARY_NAME} PUBLIC cxx_std_17)

# IDEs should put the headers in a nice place
# (This might not work as intended)
# source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})
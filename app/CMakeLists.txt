
set(APP_SOURCE
	main.cpp
)

set(APP_NAME "ConsoleMinesweeper")
add_executable("${APP_NAME}" ${APP_SOURCE})

target_compile_features("${APP_NAME}" PRIVATE cxx_std_17)

target_link_libraries("${APP_NAME}"
    PRIVATE
	"${LIBRARY_NAME}"
)

cmake_minimum_required (VERSION 3.8)

project(ConsoleMinesweeper 
        VERSION 7.1.0
        DESCRIPTION "Console Minesweeper Game"
        LANGUAGES CXX)


set(LIBRARY_NAME "libconsoleminesweeper")

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    # Set global c++ standard
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    # Use -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Let's nicely support folders in IDE's
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Testing only available if this is the main app
    # Note this needs to be done in the main CMakeLists
    # since it calls enable_testing, which must be in the
    # main CMakeLists.
    include(CTest)

    
    # TO BE ADDED
    # Docs only available if this is the main app
    #find_package(Doxygen)
    #if(Doxygen_FOUND)
    #    add_subdirectory(docs)
    #else()
    #    message(STATUS "Doxygen not found, not building docs")
    #endif()




    ################################
    # Needed to download Google test
    ################################

    # Download and unpack googletest at configure time
    configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
      RESULT_VARIABLE result
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
    if(result)
      message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
      RESULT_VARIABLE result
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
    if(result)
      message(FATAL_ERROR "Build step for googletest failed: ${result}")
    endif()

    # Prevent overriding the parent project's compiler/linker
    # settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Add googletest directly to our build. This defines
    # the gtest and gtest_main targets.
    add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                    ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                    EXCLUDE_FROM_ALL)

    # The gtest/gtest_main targets carry header search path
    # dependencies automatically when using CMake 2.8.11 or
    # later. Otherwise we have to add them here ourselves.
    if (CMAKE_VERSION VERSION_LESS 2.8.11)
      include_directories("${gtest_SOURCE_DIR}/include")
    endif()

    # Now simply link against gtest or gtest_main as needed. Eg.
    # add_executable(example example.cpp)
    # target_link_libraries(example gtest_main)
    # add_test(NAME example_test COMMAND example)

    ##########################
    # End of Google Test stuff
    ##########################

endif()


# The external minesweeper library is here
add_subdirectory(extern/minesweeper)


# The compiled library code is here
add_subdirectory(src)


# The executable app is here
add_subdirectory(app)


# Testing only available if this is the main app
# Emergency override CONSOLE_MINESWEEPER_LIB_BUILD_TESTING provided as well
# Use set(CONSOLE_MINESWEEPER_LIB_BUILD_TESTING ON) in parent folder to enable testing in this project as well
if(((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) OR CONSOLE_MINESWEEPER_LIB_BUILD_TESTING) 
            AND BUILD_TESTING)
    add_subdirectory(tests)
endif()

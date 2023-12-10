cmake_minimum_required(VERSION 3.14)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

project(io_dev VERSION 0.1 LANGUAGES CXX C)
option(DEVELOPER_BUILD "Enable developer mode (ccache, checks, sanitizers, extensive warnings)" OFF)
option(IO_DEV_ENABLE_TESTS "Enable tests" ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 90)

include(FetchContent)

FetchContent_Declare(
    _project_options
    URL https://github.com/aminya/project_options/archive/refs/tags/v0.33.0.zip
)
FetchContent_MakeAvailable(_project_options)

set(TEXT_PROCESSING_ENABLE_TESTS OFF)
FetchContent_Declare(
	stp
	GIT_REPOSITORY https://github.com/bialasjaroslaw/stp.git
	GIT_TAG        df8359facbeaddee20f8f0061e4a3ca7da626dfa
)
FetchContent_MakeAvailable(stp)

add_library(io_dev 
    include/io_dev/dev.h
    include/io_dev/file.h
    src/file.cpp
)
target_include_directories(io_dev INTERFACE include)
target_include_directories(io_dev PRIVATE include/io_dev)
target_link_libraries(io_dev PUBLIC text_processing)

if(DEVELOPER_BUILD)
    set(ENABLE_CPPCHECK "ENABLE_CPPCHECK")
    if(NOT CXX_CPPCHECK)
        set(ENABLE_CPPCHECK "")
    endif()

    check_sanitizers_support(
        ENABLE_SANITIZER_ADDRESS
        ENABLE_SANITIZER_UNDEFINED_BEHAVIOR
        ENABLE_SANITIZER_LEAK
        ENABLE_SANITIZER_THREAD
        ENABLE_SANITIZER_MEMORY
    )

    include(${_project_options_SOURCE_DIR}/Index.cmake)
    include(${_project_options_SOURCE_DIR}/src/DynamicProjectOptions.cmake)
    project_options(
        PREFIX ${PROJECT_NAME}
    	ENABLE_CACHE
    	${ENABLE_CPPCHECK}
    	${ENABLE_CLANG_TIDY}
    	${ENABLE_SANITIZER_ADDRESS}
    	${ENABLE_SANITIZER_LEAK}
    	${ENABLE_SANITIZER_UNDEFINED_BEHAVIOR}
    	ENABLE_COVERAGE
        ENABLE_CONTROL_FLOW_PROTECTION
        ENABLE_STACK_PROTECTION
        ENABLE_OVERFLOW_PROTECTION
        WARNINGS_AS_ERRORS
        ENABLE_GCC_ANALYZER
        GCC_WARNINGS
    )

    target_link_libraries(io_dev INTERFACE 
        ${PROJECT_NAME}_project_options 
        ${PROJECT_NAME}_project_warnings
    )
endif()

if(IO_DEV_ENABLE_TESTS)
    include(FetchContent)
    include(CTest)
    include(GoogleTest)

    enable_testing()
    FetchContent_Declare(
    	googletest
    	GIT_REPOSITORY https://github.com/google/googletest.git
    	GIT_TAG        58d77fa8070e8cec2dc1ed015d66b454c8d78850 # release-1.12.1
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(main_test 
	   tests/test_file.cpp
    )

    target_link_libraries(main_test PRIVATE io_dev gtest_main)
    target_link_libraries(main_test PRIVATE 
        ${PROJECT_NAME}_project_options 
        ${PROJECT_NAME}_project_warnings
    )
    gtest_discover_tests(main_test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests/data)
    
endif()
cmake_minimum_required(VERSION 3.14)
include(AutoCCache.cmake)
# Add project_options v0.20.0
# https://github.com/cpp-best-practices/project_options
include(FetchContent)
FetchContent_Declare(_project_options URL https://github.com/aminya/project_options/archive/refs/tags/v0.20.0.zip)
FetchContent_MakeAvailable(_project_options)
include(${_project_options_SOURCE_DIR}/Index.cmake)

FetchContent_Declare(
	stp
	GIT_REPOSITORY https://github.com/bialasjaroslaw/stp.git
	GIT_TAG        df8359facbeaddee20f8f0061e4a3ca7da626dfa
)
FetchContent_MakeAvailable(stp)

project(io_dev VERSION 0.2 LANGUAGES CXX)
option(DEVELOPER_BUILD "Enable developer mode" OFF)
option(IO_DEV_ENABLE_TESTS "Enable tests" ON)

set(ENABLE_SANITIZER_ADDRESS "ENABLE_SANITIZER_ADDRESS")
if(MSVC)
    string(FIND "$ENV{PATH}" "$ENV{VSINSTALLDIR}" index_of_vs_install_dir)
    if(NOT "${index_of_vs_install_dir}" STREQUAL "-1")
        set(ENABLE_SANITIZER_ADDRESS "")
    endif()
endif()

if(DEVELOPER_BUILD)
    include(${_project_options_SOURCE_DIR}/src/DynamicProjectOptions.cmake)
    dynamic_project_options(
	ENABLE_CACHE
	ENABLE_CPPCHECK
	ENABLE_CLANG_TIDY
	${ENABLE_SANITIZER_ADDRESS}
	ENABLE_SANITIZER_LEAK
	ENABLE_SANITIZER_UNDEFINED_BEHAVIOR
	ENABLE_COVERAGE
    )
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

add_library(io_dev 
    include/io_dev/dev.h
    include/io_dev/file.h
    src/file.cpp
)
target_include_directories(io_dev INTERFACE include)
target_include_directories(io_dev PRIVATE include/io_dev)
target_link_libraries(io_dev PUBLIC text_processing)

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
    if(DEVELOPER_BUILD)
	target_link_libraries(main_test PRIVATE project_options project_warnings)
    endif()
    gtest_discover_tests(main_test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests/data)
    
endif()
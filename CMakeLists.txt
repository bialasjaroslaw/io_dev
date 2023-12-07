cmake_minimum_required(VERSION 3.14)
include(AutoCCache.cmake)
# Add project_options v0.20.0
# https://github.com/cpp-best-practices/project_options
include(FetchContent)
FetchContent_Declare(_project_options URL https://github.com/aminya/project_options/archive/refs/tags/v0.20.0.zip)
FetchContent_MakeAvailable(_project_options)
include(${_project_options_SOURCE_DIR}/Index.cmake)

project(io_dev VERSION 0.2 LANGUAGES CXX)
option(DEVELOPER_BUILD "Enable developer mode" OFF)

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


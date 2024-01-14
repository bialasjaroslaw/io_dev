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

include(Packages.cmake)
add_dependency(
    NAME googletest
    PACKAGE GTest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
add_dependency(
    NAME _project_options 
    URL https://github.com/aminya/project_options/archive/refs/tags/v0.33.0.zip
)
set(TEXT_PROCESSING_ENABLE_TESTS OFF)
add_dependency(
    NAME stp 
    REPO https://github.com/bialasjaroslaw/stp.git
    HASH aef0329b3cd8aa7dc52ba838d15221b515a91ff8
)
use_dependency(stp)

add_library(io_dev 
    include/io_dev/dev.h
    include/io_dev/file.h
    src/file.cpp
)
target_include_directories(io_dev INTERFACE include)
target_include_directories(io_dev PRIVATE include/io_dev)
target_link_libraries(io_dev PUBLIC text_processing)

if(DEVELOPER_BUILD)
    use_dependency(_project_options)

    include(${_project_options_SOURCE_DIR}/Index.cmake)

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

    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(ENABLE_GCC_ANALYZER "ENABLE_GCC_ANALYZER")
        set(GCC_WARNINGS "GCC_WARNINGS")
        set(ENABLE_COVERAGE "ENABLE_COVERAGE")
        set(ENABLE_CONTROL_FLOW_PROTECTION "ENABLE_CONTROL_FLOW_PROTECTION")
        set(ENABLE_STACK_PROTECTION "ENABLE_STACK_PROTECTION")
        set(ENABLE_OVERFLOW_PROTECTION "ENABLE_OVERFLOW_PROTECTION")
        set(WARNINGS_AS_ERRORS "WARNINGS_AS_ERRORS")

        target_compile_definitions(io_dev INTERFACE 
            _GLIBCXX_CONCEPT_CHECKS 
            _GLIBCXX_ASSERTIONS 
            _GLIBCXX_DEBUG 
            _GLIBCXX_DEBUG_PEDANTIC
            _GLIBCXX_DEBUG_BACKTRACE
            _GLIBCXX_SANITIZE_VECTOR
        )
    else()
        set(ENABLE_GCC_ANALYZER "")
        set(GCC_WARNINGS "")
        set(ENABLE_SANITIZER_ADDRESS "")
        set(ENABLE_SANITIZER_UNDEFINED_BEHAVIOR "")
        set(ENABLE_SANITIZER_LEAK "")
        set(ENABLE_SANITIZER_THREAD "")
        set(ENABLE_SANITIZER_MEMORY "")
    endif()
    include(${_project_options_SOURCE_DIR}/src/DynamicProjectOptions.cmake)
    project_options(
        PREFIX ${PROJECT_NAME}
    	ENABLE_CACHE
    	${ENABLE_CPPCHECK}
    	${ENABLE_CLANG_TIDY}
    	${ENABLE_SANITIZER_ADDRESS}
    	${ENABLE_SANITIZER_LEAK}
    	${ENABLE_SANITIZER_UNDEFINED_BEHAVIOR}
    	${ENABLE_COVERAGE}
        ${ENABLE_CONTROL_FLOW_PROTECTION}
        ${ENABLE_STACK_PROTECTION}
        ${ENABLE_OVERFLOW_PROTECTION}
        ${WARNINGS_AS_ERRORS}
        ${ENABLE_GCC_ANALYZER}
        ${GCC_WARNINGS}
    )

    target_link_libraries(io_dev INTERFACE 
        ${PROJECT_NAME}_project_options 
        ${PROJECT_NAME}_project_warnings
    )
endif()

if(IO_DEV_ENABLE_TESTS)
    include(CTest)
    include(GoogleTest)

    enable_testing()
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    use_dependency(GTest)

    add_executable(main_test 
	   tests/test_file.cpp
    )

    target_link_libraries(main_test PRIVATE io_dev GTest::gtest_main GTest::gmock)
    gtest_discover_tests(main_test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests/data)
    
endif()
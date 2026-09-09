cmake_minimum_required(VERSION 4.0.0)

project(Cayleyizer)

#
# C++
#
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#
# Project paths
#
set(CAYLEYIZER_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(CAYLEYIZER_SOURCE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set(CAYLEYIZER_PYTHON_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/python
)

set(CAYLEYIZER_TEST_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/test
)

#
# igraph
#
set(IGRAPH_ROOT
    "/usr/local/src/igraph-0.10.15"
    CACHE PATH
    "Path to the igraph source tree"
)

set(IGRAPH_INCLUDE_DIR
    ${IGRAPH_ROOT}/include
)

set(IGRAPH_LIBRARY
    ${IGRAPH_ROOT}/build/src/libigraph.a
)

#
# Cayleyizer source files
#
file(
    GLOB
    CAYLEYIZER_SOURCE_FILES
    CONFIGURE_DEPENDS
    "${CAYLEYIZER_SOURCE_DIR}/*.cpp"
)

#
# Core C++ library
#
add_library(
    Cayleyizer
    STATIC
    ${CAYLEYIZER_SOURCE_FILES}
)

target_include_directories(
    Cayleyizer

    PUBLIC
        ${CAYLEYIZER_INCLUDE_DIR}

    PRIVATE
        ${IGRAPH_INCLUDE_DIR}
)

target_link_libraries(
    Cayleyizer

    PRIVATE
        ${IGRAPH_LIBRARY}
)

#
# Cayleyizer is linked into the Python shared library,
# so its object code must be position independent.
#
set_target_properties(
    Cayleyizer

    PROPERTIES
        POSITION_INDEPENDENT_CODE ON
)

#
# C++ test
#
add_executable(
    cayleyizer_test
    ${CAYLEYIZER_TEST_DIR}/test.cpp
)

target_link_libraries(
    cayleyizer_test

    PRIVATE
        Cayleyizer
)

#
# Python
#
find_package(
    Python3
    REQUIRED
    COMPONENTS
        Interpreter
        Development
)

#
# Find the pybind11 CMake package belonging to
# the Python installation we're using.
#
execute_process(
    COMMAND
        ${Python3_EXECUTABLE}
        -m
        pybind11
        --cmakedir

    OUTPUT_VARIABLE
        PYBIND11_CMAKE_DIR

    OUTPUT_STRIP_TRAILING_WHITESPACE
)

list(
    APPEND
    CMAKE_PREFIX_PATH
    ${PYBIND11_CMAKE_DIR}
)

find_package(
    pybind11
    CONFIG
    REQUIRED
)

#
# Python extension:
#
#     import cayleyizer
#
pybind11_add_module(
    cayleyizer
    ${CAYLEYIZER_PYTHON_DIR}/Bindings.cpp
)

target_include_directories(
    cayleyizer

    PRIVATE
        ${CAYLEYIZER_INCLUDE_DIR}
)

target_link_libraries(
    cayleyizer

    PRIVATE
        Cayleyizer
)

#
# Put cayleyizer.so in build/python
#
set_target_properties(
    cayleyizer

    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY
            ${CMAKE_BINARY_DIR}/python
)

#
# Run test.cpp
#
add_custom_target(
    run_cpp_test

    COMMAND
        $<TARGET_FILE:cayleyizer_test>

    DEPENDS
        cayleyizer_test

    WORKING_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}

    USES_TERMINAL
)

#
# Run test.py
#
add_custom_target(
    run_python_test

    COMMAND
        ${CMAKE_COMMAND}
        -E
        env
        "PYTHONPATH=${CMAKE_BINARY_DIR}/python"
        ${Python3_EXECUTABLE}
        ${CAYLEYIZER_PYTHON_DIR}/test.py

    DEPENDS
        cayleyizer

    WORKING_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}

    USES_TERMINAL
)

#
# CTest
#
enable_testing()

add_test(
    NAME
        cpp_test

    COMMAND
        $<TARGET_FILE:cayleyizer_test>
)

add_test(
    NAME
        python_test

    COMMAND
        ${CMAKE_COMMAND}
        -E
        env
        "PYTHONPATH=${CMAKE_BINARY_DIR}/python"
        ${Python3_EXECUTABLE}
        ${CAYLEYIZER_PYTHON_DIR}/test.py
)
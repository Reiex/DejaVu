#######################################################################################################################
##! \file
##! \author Pélégrin Marius
##! \copyright The MIT License (MIT)
##! \date 2020-2023
#######################################################################################################################

cmake_minimum_required(VERSION 3.21)

project(DejaVu)

set(CMAKE_CXX_STANDARD 23)


add_executable(
    dejavu-examples
    ${CMAKE_CURRENT_LIST_DIR}/examples/main.cpp
)

target_include_directories(
    dejavu-examples
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/external/Ruc/include
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/external/SciPP/include
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/external/Diskon/include
)

target_link_libraries(
    dejavu-examples
    diskon
)


set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/Diskon)

add_dependencies(
    dejavu-examples
    diskon
)

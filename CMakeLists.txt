#######################################################################################################################
##! \file
##! \author Pélégrin Marius
##! \copyright The MIT License (MIT)
##! \date 2020-2023
#######################################################################################################################

# Global project definitions

cmake_minimum_required(VERSION 3.21)

project(DejaVu)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

option(DEJAVU_LIBRARY_ONLY "Build only the library")

# Add external dependencies

set(RUC_LIBRARY_ONLY ON)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/Ruc)

set(SCIPP_LIBRARY_ONLY ON)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/SciPP)

set(DISKON_LIBRARY_ONLY ON)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/Diskon)

# DejaVu examples

if(NOT DEJAVU_LIBRARY_ONLY)

    add_executable(
        dejavu-examples
        ${CMAKE_CURRENT_LIST_DIR}/examples/main.cpp
    )

    add_dependencies(
        dejavu-examples
        diskon
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

endif()

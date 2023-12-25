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

# Add external dependencies

option(DEJAVU_ADD_DEPENDENCIES "Add external dependencies to the project" ON)

if(DEJAVU_ADD_DEPENDENCIES)

    set(RUC_ADD_EXAMPLES OFF)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/Ruc)

    set(SCIPP_ADD_EXAMPLES OFF)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/SciPP)

    set(DISKON_ADD_DEPENDENCIES OFF)
    set(DISKON_ADD_EXAMPLES OFF)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/Diskon)

endif()

# DejaVu examples

option(DEJAVU_ADD_EXAMPLES "Add target dejavu-examples" ON)

if(DEJAVU_ADD_EXAMPLES)

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

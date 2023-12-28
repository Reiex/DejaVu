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

if(NOT TARGET scipp)
    set(SCIPP_ADD_EXAMPLES OFF)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/SciPP)
endif()

if(NOT TARGET ruc)
    set(RUC_ADD_EXAMPLES OFF)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/Ruc)
endif()

if(NOT TARGET diskon)
    set(DISKON_ADD_EXAMPLES OFF)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/Diskon)
endif()

# DejaVu

add_custom_target(
    dejavu SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/include/DejaVu/DejaVu.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/DejaVu/DejaVuDecl.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/DejaVu/DejaVuTypes.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/DejaVu/Core/Core.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/DejaVu/Core/CoreDecl.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/DejaVu/Core/CoreTypes.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/DejaVu/Core/Image.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/DejaVu/Core/Pixel.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/DejaVu/Core/Shape.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/DejaVu/Core/templates/Image.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/DejaVu/Core/templates/Pixel.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/DejaVu/Core/templates/Shape.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/DejaVu/Processing/Processing.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/DejaVu/Processing/ProcessingDecl.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/DejaVu/Processing/ProcessingTypes.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/DejaVu/Processing/PrImage.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/DejaVu/Processing/templates/PrImage.hpp
)

add_dependencies(
    dejavu
    ruc
    scipp
    diskon
)

# DejaVu examples

option(DEJAVU_ADD_EXAMPLES "Add target dejavu-examples" ON)

if(DEJAVU_ADD_EXAMPLES)

    add_executable(
        dejavu-examples
        ${CMAKE_CURRENT_LIST_DIR}/examples/main.cpp
    )

    add_dependencies(
        dejavu-examples
        dejavu
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

#
# Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE(SWIGPython)

# Get list of enum modules to add to dependencies, as well as the import
# string.
EXECUTE_PROCESS(
    COMMAND ${PYTHON_EXECUTABLE} ${PKMN_SOURCE_DIR}/scripts/print_enum_names_for_cmake.py --enum-dir=${PKMN_SOURCE_DIR}/include/pkmn/enums --print-as-cmake-list
    OUTPUT_VARIABLE ENUM_LIST OUTPUT_STRIP_TRAILING_WHITESPACE
)
SET(ENUM_LIST ${ENUM_LIST} CACHE STRING "Enum list")
EXECUTE_PROCESS(
    COMMAND ${PYTHON_EXECUTABLE} ${PKMN_SOURCE_DIR}/scripts/print_enum_names_for_cmake.py --enum-dir=${PKMN_SOURCE_DIR}/include/pkmn/enums --print-as-python-imports
    OUTPUT_VARIABLE ENUM_IMPORT OUTPUT_STRIP_TRAILING_WHITESPACE
)
SET(ENUM_IMPORT ${ENUM_IMPORT} CACHE STRING "Enum import")

#
# Set variables needed for CMake macros
#

SET(SWIG_MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

SET(SWIG_PYTHON_INCLUDE_DIRS
    ${SWIG_MODULE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/..
    ${SWIG_MODULE_DIR}/../..
    ${SWIG_MODULE_DIR}/../../modules
    ${PKMN_SOURCE_DIR}/include
    ${PKMN_BINARY_DIR}/include
)

SET(SWIG_PYTHON_LIBRARIES
    pkmn
)

SWIG_BUILD_PYTHON_MODULE(pkmn_python Core pkmn TRUE)
SWIG_BUILD_PYTHON_MODULE(stl_python  STL  pkmn TRUE)

ADD_SUBDIRECTORY(breeding)
ADD_SUBDIRECTORY(buildinfo)
ADD_SUBDIRECTORY(calculations)
ADD_SUBDIRECTORY(database)
ADD_SUBDIRECTORY(qt)
ADD_SUBDIRECTORY(paths)

# Oh, SWIG. We need to build this for other SWIG modules to treat C++ enum
# parameters properly, but we don't need to install it.
SWIG_BUILD_PYTHON_MODULE_BUILD_ONLY(enums_python Enums pkmn TRUE)

# Generate .py files with enums.
SET(py_enums_deps
    ${PKMN_SOURCE_DIR}/include/pkmn/enums/CMakeLists.txt
    ${PKMN_SOURCE_DIR}/scripts/generate_python_enums.py
)
FOREACH(enum ${ENUM_LIST})
    LIST(APPEND py_enums
        "${CMAKE_CURRENT_BINARY_DIR}/${enum}.py"
    )
ENDFOREACH()

ADD_CUSTOM_COMMAND(
    DEPENDS ${py_enums_deps}
    OUTPUT ${py_enums}
    COMMAND ${PYTHON_EXECUTABLE} ${PKMN_SOURCE_DIR}/scripts/generate_python_enums.py --enum-dir=${PKMN_SOURCE_DIR}/include/pkmn/enums --output-dir=${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating Python enums"
)
ADD_CUSTOM_TARGET(
    pkmn_python_enums ALL
    DEPENDS ${py_enums}
)

INSTALL(
    FILES ${py_enums}
    DESTINATION ${PYTHON_INSTALL_DIR}/pkmn
    COMPONENT Python
)

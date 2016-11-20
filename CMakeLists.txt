#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

########################################################################
# CMake configuration
########################################################################
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)
PROJECT(PKMN C CXX)

SET(PKMN_VERSION_MAJOR 0)
SET(PKMN_VERSION_MINOR 0)
SET(PKMN_VERSION_PATCH 1)
SET(PKMN_VERSION "${PKMN_VERSION_MAJOR}.${PKMN_VERSION_MINOR}.${PKMN_VERSION_PATCH}")

SET(PKMN_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET(PKMN_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

IF(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${PKMN_SOURCE_DIR})
    SET(PKMN_USED_AS_SUBPROJECT TRUE)
    IF(NOT DEFINED PKMN_DONT_INSTALL_HEADERS)
        SET(PKMN_DONT_INSTALL_HEADERS TRUE)
    ENDIF(NOT DEFINED PKMN_DONT_INSTALL_HEADERS)
ENDIF(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${PKMN_SOURCE_DIR})

SET(CMAKE_MODULE_PATH "${PKMN_SOURCE_DIR}/cmake/Modules;${CMAKE_MODULE_PATH}")

####################################################################
# Project configuration
####################################################################
INCLUDE(PKMNComponent)

IF(NOT PKMN_USED_AS_SUBPROJECT)
    FIND_PACKAGE(Doxygen)
ENDIF(NOT PKMN_USED_AS_SUBPROJECT)

########################################################################
# Find C++ dependencies
########################################################################
SET(BOOST_REQUIRED_COMPONENTS
    filesystem
    thread
    unit_test_framework
)
IF(MSVC)
    SET(BOOST_ALL_DYN_LINK "${BOOST_ALL_DYN_LINK}" CACHE BOOL "Enable Boost dynamic linking")
    IF(BOOST_ALL_DYN_LINK)
        ADD_DEFINITIONS(-DBOOST_ALL_DYN_LINK)
    ELSE(BOOST_ALL_DYN_LINK)
        SET(BOOST_REQUIRED_COMPONENTS)
    ENDIF(BOOST_ALL_DYN_LINK)
ENDIF(MSVC)
FIND_PACKAGE(Boost 1.55 REQUIRED COMPONENTS ${BOOST_REQUIRED_COMPONENTS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

# We need Boost.Predef for a check here
INCLUDE(PKMNPlatform)

########################################################################
# Find other dependencies
########################################################################
FIND_PACKAGE(SWIG)
FIND_PACKAGE(CSharp)
FIND_PACKAGE(PythonInterp 2)
FIND_PACKAGE(PythonLibs 2)

INCLUDE(PKMNPython)
PYTHON_CHECK_MODULE("PLY" "ply" "True" PYTHON_PLY_FOUND)
PYTHON_CHECK_MODULE("CppHeaderParser" "CppHeaderParser" "True" PYTHON_CPPHEADERPARSER_FOUND)
IF(PYTHON_PLY_FOUND AND PYTHON_CPPHEADERPARSER_FOUND)
    SET(PYTHON_MODULES_FOUND TRUE)
ENDIF(PYTHON_PLY_FOUND AND PYTHON_CPPHEADERPARSER_FOUND)

########################################################################
# Build PKSav
########################################################################
INCLUDE(PKSavConfig)
ADD_SUBDIRECTORY(pksav)

########################################################################
# Create uninstall target
########################################################################
IF(NOT PKMN_USED_AS_SUBPROJECT)
    CONFIGURE_FILE(
        ${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
    @ONLY)

    ADD_CUSTOM_TARGET(uninstall
        ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
    )
ENDIF(NOT PKMN_USED_AS_SUBPROJECT)

####################################################################
# Set install subdirectories
####################################################################
IF(NOT DEFINED LIB_SUFFIX AND REDHAT AND CMAKE_SYSTEM_PROCESSOR MATCHES "64$")
    SET(LIB_SUFFIX 64)
ENDIF()
SET(PKMN_LIB_SUFFIX ${LIB_SUFFIX} CACHE STRING "lib directory suffix")
SET(PKMN_RUNTIME_DIR bin)
SET(PKMN_LIBRARY_DIR lib${LIB_SUFFIX})
SET(PKMN_INCLUDE_DIR include)
SET(PKMN_PKG_DATA_DIR share/pkmn)
SET(PKMN_DOCS_DIR share/doc/pkmn)

####################################################################
# Components
####################################################################
PKMN_REGISTER_COMPONENT("Core C++ Library" PKMN_ENABLE_CORELIB ON "" OFF)
PKMN_REGISTER_COMPONENT("C API"            PKMN_ENABLE_C ON "PKMN_ENABLE_CORELIB" OFF)
PKMN_REGISTER_COMPONENT("C# API"           PKMN_ENABLE_CSHARP  ON
    "SWIG_FOUND;CSHARP_FOUND;PYTHONINTERP_FOUND;PYTHON_MODULES_FOUND" OFF)
PKMN_REGISTER_COMPONENT("Python API"       PKMN_ENABLE_PYTHON  ON
    "SWIG_FOUND;PYTHONINTERP_FOUND;PYTHONLIBS_FOUND;PYTHON_MODULES_FOUND" OFF)

IF(NOT PKMN_USED_AS_SUBPROJECT)
    PKMN_REGISTER_COMPONENT("Doxygen Documentation" PKMN_ENABLE_DOCS  ON "DOXYGEN_FOUND" OFF)
    PKMN_REGISTER_COMPONENT("Unit Tests"            PKMN_ENABLE_TESTS ON "" OFF)
ENDIF(NOT PKMN_USED_AS_SUBPROJECT)

####################################################################
# Actual building
####################################################################
ADD_SUBDIRECTORY(libpkmn-database)
ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(lib)

IF(PKMN_ENABLE_DOCS)
    ADD_SUBDIRECTORY(docs)
ENDIF(PKMN_ENABLE_DOCS)

IF(PKMN_ENABLE_TESTS)
    ENABLE_TESTING()
    ADD_SUBDIRECTORY(testing)
ENDIF(PKMN_ENABLE_TESTS)

####################################################################
# Final display
####################################################################
PKMN_PRINT_COMPONENT_SUMMARY()
IF(NOT PKMN_USED_AS_SUBPROJECT)
    MESSAGE(STATUS "")
    MESSAGE(STATUS "Building LibPKMN.")
    MESSAGE(STATUS " - Build type: ${CMAKE_BUILD_TYPE}")
    MESSAGE(STATUS " - Compiler: ${CMAKE_C_COMPILER_ID}")
    MESSAGE(STATUS " - Install prefix: ${CMAKE_INSTALL_PREFIX}")
    MESSAGE(STATUS "")
ENDIF(NOT PKMN_USED_AS_SUBPROJECT)

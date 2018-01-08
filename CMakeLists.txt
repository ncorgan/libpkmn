#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

########################################################################
# CMake configuration
########################################################################
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)
PROJECT(PKMN C CXX)

SET(PKMN_VERSION_MAJOR 0)
SET(PKMN_VERSION_MINOR 3)
SET(PKMN_VERSION_PATCH 0)
SET(PKMN_VERSION "${PKMN_VERSION_MAJOR}.${PKMN_VERSION_MINOR}.${PKMN_VERSION_PATCH}")

SET(PKMN_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET(PKMN_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

IF(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${PKMN_SOURCE_DIR})
    SET(PKMN_USED_AS_SUBPROJECT TRUE)
    IF(NOT DEFINED PKMN_DONT_INSTALL_HEADERS)
        SET(PKMN_DONT_INSTALL_HEADERS TRUE)
    ENDIF()
ENDIF()

SET(CMAKE_MODULE_PATH "${PKMN_SOURCE_DIR}/cmake/Modules;${CMAKE_MODULE_PATH}")

####################################################################
# Project configuration
####################################################################
INCLUDE(PKMNComponent)

IF(NOT PKMN_USED_AS_SUBPROJECT)
    INCLUDE(CPack)
    FIND_PACKAGE(Doxygen)
ENDIF()

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

########################################################################
# Find C++ dependencies
########################################################################
SET(BOOST_REQUIRED_COMPONENTS
    date_time
    filesystem
    random
    system
    thread
)
IF(MSVC)
    SET(BOOST_ALL_DYN_LINK "${BOOST_ALL_DYN_LINK}" CACHE BOOL "Enable Boost dynamic linking")
    IF(BOOST_ALL_DYN_LINK)
        ADD_DEFINITIONS(-DBOOST_ALL_DYN_LINK)
    ELSE(BOOST_ALL_DYN_LINK)
        SET(BOOST_REQUIRED_COMPONENTS)
    ENDIF()
ENDIF()
FIND_PACKAGE(Boost 1.55 REQUIRED COMPONENTS ${BOOST_REQUIRED_COMPONENTS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

INCLUDE(PKMNQt)

# We need Boost.Predef for a check here
INCLUDE(PKMNPlatform)

########################################################################
# Find other dependencies
########################################################################
FIND_PACKAGE(SWIG)
FIND_PACKAGE(CSharp)
FIND_PACKAGE(Lua 5.1)
FIND_PACKAGE(PythonInterp 2)
FIND_PACKAGE(PythonLibs 2)
FIND_PACKAGE(Ruby)

########################################################################
# Check SWIG version for each wrapper
########################################################################

INCLUDE(VersionMacros)

# Lua: fix some errors
# See https://github.com/swig/swig/issues/92
COMPARE_VERSION_STR(swig_check ${SWIG_VERSION} "2.0.12")
IF(swig_check GREATER -1)
    SET(SWIG_VERSION_CHECK_LUA TRUE)
ENDIF()

# Python: Python 2 requires support for a specific macro
IF("${PYTHONLIBS_VERSION_STRING}" STRLESS "3.0.0")
    COMPARE_VERSION_STR(swig_check ${SWIG_VERSION} "3.0.8")
    IF(swig_check GREATER -1)
        SET(SWIG_VERSION_CHECK_PYTHON TRUE)
    ENDIF()
ELSE()
    SET(SWIG_VERSION_CHECK_PYTHON TRUE)
ENDIF()

# Ruby: need fix for templated constructors
# See https://github.com/swig/swig/issues/245
COMPARE_VERSION_STR(swig_check ${SWIG_VERSION} "3.0.3")
IF(swig_check GREATER -1)
    SET(SWIG_VERSION_CHECK_RUBY TRUE)
ENDIF()

INCLUDE(PKMNPython)
PYTHON_CHECK_MODULE("PLY" "ply" "True" PYTHON_PLY_FOUND)
PYTHON_CHECK_MODULE("CppHeaderParser" "CppHeaderParser" "True" PYTHON_CPPHEADERPARSER_FOUND)
IF(PYTHON_PLY_FOUND AND PYTHON_CPPHEADERPARSER_FOUND)
    SET(PYTHON_MODULES_FOUND TRUE)
ENDIF()

########################################################################
# Build submodule libraries
########################################################################
ADD_SUBDIRECTORY(PkmGCTools/LibPkmGC)

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
ENDIF()

########################################################################
# Check for specified test tools
########################################################################
INCLUDE(StaticAnalysisTools)

####################################################################
# Components
####################################################################
PKMN_REGISTER_COMPONENT("Core C++ Library" PKMN_ENABLE_CORELIB ON
    "Boost_FOUND;PYTHONINTERP_FOUND" OFF)
PKMN_REGISTER_COMPONENT(" * Qt Widgets"    PKMN_ENABLE_QT ON
    "PKMN_ENABLE_CORELIB;QT_FOUND" OFF)
PKMN_REGISTER_COMPONENT("C API"            PKMN_ENABLE_C ON "PKMN_ENABLE_CORELIB" OFF)
PKMN_REGISTER_COMPONENT("C# API"           PKMN_ENABLE_CSHARP  ON
    "PKMN_ENABLE_CORELIB;SWIG_FOUND;CSHARP_FOUND;PYTHONINTERP_FOUND;PYTHON_MODULES_FOUND" OFF)
PKMN_REGISTER_COMPONENT("Lua API"          PKMN_ENABLE_LUA  ON
    "PKMN_ENABLE_CORELIB;SWIG_FOUND;SWIG_VERSION_CHECK_LUA;LUA_FOUND;PYTHON_MODULES_FOUND" OFF)
PKMN_REGISTER_COMPONENT("Python API"       PKMN_ENABLE_PYTHON  ON
    "PKMN_ENABLE_CORELIB;SWIG_FOUND;SWIG_VERSION_CHECK_PYTHON;PYTHONINTERP_FOUND;PYTHONLIBS_FOUND;PYTHON_MODULES_FOUND" OFF)
# TODO: reenable when classes are renamed back
#PKMN_REGISTER_COMPONENT("Ruby API"         PKMN_ENABLE_RUBY  ON
#    "PKMN_ENABLE_CORELIB;SWIG_FOUND;SWIG_VERSION_CHECK_RUBY;RUBY_FOUND;PYTHON_MODULES_FOUND" OFF)

IF(NOT PKMN_USED_AS_SUBPROJECT)
    PKMN_REGISTER_COMPONENT("Doxygen Documentation" PKMN_ENABLE_DOCS  ON "DOXYGEN_FOUND" OFF)
    PKMN_REGISTER_COMPONENT("Unit Tests"            PKMN_ENABLE_TESTS ON "" OFF)
ENDIF()

####################################################################
# Actual building
####################################################################
ADD_SUBDIRECTORY(libpkmn-database)
ADD_SUBDIRECTORY(images)
ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(lib)

IF(PKMN_ENABLE_DOCS)
    ADD_SUBDIRECTORY(docs)
ENDIF()

IF(PKMN_ENABLE_TESTS)
    ENABLE_TESTING()
    ADD_SUBDIRECTORY(testing)
ENDIF()

####################################################################
# Final display
####################################################################
PKMN_PRINT_COMPONENT_SUMMARY()
IF(NOT PKMN_USED_AS_SUBPROJECT)
    MESSAGE(STATUS "")
    MESSAGE(STATUS "Building LibPKMN.")
    MESSAGE(STATUS " - Build type: ${CMAKE_BUILD_TYPE}")
    MESSAGE(STATUS " - Compiler: ${PKMN_COMPILER_NAME}")
    MESSAGE(STATUS " - Install prefix: ${CMAKE_INSTALL_PREFIX}")
    MESSAGE(STATUS "")
ENDIF()

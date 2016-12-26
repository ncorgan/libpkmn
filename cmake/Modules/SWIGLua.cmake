#
# Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying FILE LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

########################################################################
# This file creates the macro SWIG_BUILD_LUA_MODULE. It builds a
# Lua SWIG module and installs it in the desired location without the
# user having to deal with CMake SWIG details.
#
# Macro parameters:
#  * module_name: The module filename, minus the .i extension.
#  * cplusplus:   TRUE or FALSE, whether or not this is a C++ module.
#
# Assumes the following variables are set:
#  * SWIG_MODULE_DIR:       The source directory that contains the .i
#                           file. This is needed so the .i file can
#                           include other local .i files.
#
#  * SWIG_LUA_INCLUDE_DIRS: C/C++ include directories needed by the
#                           source files.
#
#  * SWIG_LUA_LIBRARIES:    C/C++ libraries the Lua module should
#                           link against.
#
#  * SWIG_LUA_FLAGS:        Flags to pass into the SWIG command (optional).
#
########################################################################

# Only run the install location check once
IF(DEFINED __INCLUDED_SWIGLUA_CMAKE)
    RETURN()
ENDIF(DEFINED __INCLUDED_SWIGLUA_CMAKE)
SET(__INCLUDED_SWIGLUA_CMAKE TRUE)

########################################################################
# Set the Lua install directory
########################################################################
SET(LUA_LIB_INSTALL_DIR ${PKMN_LIBRARY_DIR}/lua/${LUA_VERSION_MAJOR}.${LUA_VERSION_MINOR})
SET(LUA_LIB_INSTALL_DIR ${LUA_LIB_INSTALL_DIR} CACHE FILEPATH "Lua library install directory")

SET(LUA_SHARE_INSTALL_DIR share/lua/${LUA_VERSION_MAJOR}.${LUA_VERSION_MINOR})
SET(LUA_SHARE_INSTALL_DIR ${LUA_SHARE_INSTALL_DIR} CACHE FILEPATH "Lua .lua install directory")

########################################################################
# Macro to build and install Lua SWIG modules
########################################################################
MACRO(SWIG_BUILD_LUA_MODULE module_name cplusplus)
    INCLUDE(UseSWIG)

    SET(SWIG_INCLUDE_DIRS
        ${Boost_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${SWIG_MODULE_DIR}
        ${SWIG_LUA_INCLUDE_DIRS}
        ${LUA_INCLUDE_DIR}
    )
    INCLUDE_DIRECTORIES(${SWIG_INCLUDE_DIRS})

    SET(SWIG_LIBRARIES
        ${Boost_LIBRARIES}
        ${SWIG_LUA_LIBRARIES}
        ${LUA_LIBRARIES}
    )

    # Set flags to pass into SWIG call
    SET(CMAKE_SWIG_FLAGS -module ${module_name} ${SWIG_LUA_FLAGS})
    FOREACH(dir ${SWIG_INCLUDE_DIRS})
        LIST(APPEND CMAKE_SWIG_FLAGS "-I${dir}")
    ENDFOREACH(dir ${SWIG_INCLUDE_DIRS})

    # Allows CMake variables to be placed in SWIG .i files
    CONFIGURE_FILE(
        ${SWIG_MODULE_DIR}/${module_name}.i
        ${CMAKE_CURRENT_BINARY_DIR}/${module_name}.i
    @ONLY)

    # Set SWIG's C++ flag if specified by the user
    IF(${cplusplus})
        SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_BINARY_DIR}/${module_name}.i PROPERTIES CPLUSPLUS ON)
    ENDIF(${cplusplus})

    # The actual CMake call for SWIG
    SWIG_ADD_MODULE(${module_name} lua ${CMAKE_CURRENT_BINARY_DIR}/${module_name}.i)
    SET_TARGET_PROPERTIES(${SWIG_MODULE_${module_name}_REAL_NAME} PROPERTIES PREFIX "")
    SWIG_LINK_LIBRARIES(${module_name} ${SWIG_LIBRARIES})

    SET_TARGET_PROPERTIES(${SWIG_MODULE_${module_name}_REAL_NAME}
        PROPERTIES COMPILE_FLAGS "${PKMN_CXX_FLAGS}"
    )

    INSTALL(
        TARGETS ${SWIG_MODULE_${module_name}_REAL_NAME}
        DESTINATION ${LUA_LIB_INSTALL_DIR}
        COMPONENT Lua
    )
ENDMACRO(SWIG_BUILD_LUA_MODULE module_name cplusplus)

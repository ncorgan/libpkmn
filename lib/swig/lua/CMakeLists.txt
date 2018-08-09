#
# Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE(SWIGLua)

# Get list of enum modules to add to dependencies, as well as the import
# string.
EXECUTE_PROCESS(
    COMMAND ${PYTHON_EXECUTABLE} ${PKMN_SOURCE_DIR}/scripts/print_enum_names_for_cmake.py --enum-dir=${PKMN_SOURCE_DIR}/include/pkmn/enums --print-as-cmake-list-lua
    OUTPUT_VARIABLE ENUM_LIST OUTPUT_STRIP_TRAILING_WHITESPACE
)
SET(ENUM_LIST ${ENUM_LIST} CACHE STRING "Enum list")
EXECUTE_PROCESS(
    COMMAND ${PYTHON_EXECUTABLE} ${PKMN_SOURCE_DIR}/scripts/print_enum_names_for_cmake.py --enum-dir=${PKMN_SOURCE_DIR}/include/pkmn/enums --print-as-lua-imports
    OUTPUT_VARIABLE ENUM_IMPORT OUTPUT_STRIP_TRAILING_WHITESPACE
)
SET(ENUM_IMPORT ${ENUM_IMPORT} CACHE STRING "Enum import")

#
# Set variables needed for CMake macros
#

SET(SWIG_MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

SET(SWIG_LUA_INCLUDE_DIRS
    ${SWIG_MODULE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/..
    ${SWIG_MODULE_DIR}/..
    ${SWIG_MODULE_DIR}/../modules
    ${PKMN_SOURCE_DIR}/include
    ${PKMN_BINARY_DIR}/include
)

SET(SWIG_LUA_LIBRARIES
    pkmn
)

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/pkmn.lua
    ${CMAKE_CURRENT_BINARY_DIR}/pkmn.lua
@ONLY)
INSTALL(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/pkmn.lua
    DESTINATION ${LUA_SHARE_INSTALL_DIR}
    COMPONENT Lua
)

SWIG_BUILD_LUA_MODULE(pkmn_breeding_lua     Breeding     TRUE)
SWIG_BUILD_LUA_MODULE(pkmn_buildinfo_lua    "Build Info" TRUE)
SWIG_BUILD_LUA_MODULE(pkmn_calculations_lua Calculations TRUE)
SWIG_BUILD_LUA_MODULE(pkmn_core_lua         Core         TRUE)
SWIG_BUILD_LUA_MODULE(pkmn_database_lua     Database     TRUE)
SWIG_BUILD_LUA_MODULE(pkmn_gui_lua          GUI          TRUE)
SWIG_BUILD_LUA_MODULE(pkmn_paths_lua        Paths        TRUE)
SWIG_BUILD_LUA_MODULE(pkmn_stl_lua          STL          TRUE)

# Oh, SWIG. We need to build this for other SWIG modules to treat C++ enum
# parameters properly, but we don't need to install it.
SWIG_BUILD_LUA_MODULE_BUILD_ONLY(pkmn_enums_lua Enums TRUE)

# Generate .lua files with enums.
SET(lua_enums_deps
    ${PKMN_SOURCE_DIR}/include/pkmn/enums/CMakeLists.txt
    ${PKMN_SOURCE_DIR}/scripts/generate_swig_enums.py
)
FOREACH(enum ${ENUM_LIST})
    LIST(APPEND lua_enums
        "${CMAKE_CURRENT_BINARY_DIR}/${enum}.lua"
    )
ENDFOREACH()

ADD_CUSTOM_COMMAND(
    DEPENDS ${lua_enums_deps}
    OUTPUT ${lua_enums}
    COMMAND ${PYTHON_EXECUTABLE} ${PKMN_SOURCE_DIR}/scripts/generate_swig_enums.py --lua --enum-dir=${PKMN_SOURCE_DIR}/include/pkmn/enums --output-dir=${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating Lua enums"
)
ADD_CUSTOM_TARGET(
    pkmn_lua_enums ALL
    DEPENDS ${lua_enums}
)

INSTALL(
    FILES ${lua_enums}
    DESTINATION ${LUA_SHARE_INSTALL_DIR}
    COMPONENT Lua
)

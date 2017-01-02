#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE(SWIGLua)

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

SWIG_BUILD_LUA_MODULE(pkmn_calculations_lua TRUE)
SWIG_BUILD_LUA_MODULE(pkmn_database_lua     TRUE)
SWIG_BUILD_LUA_MODULE(pkmn_paths_lua        TRUE)
SWIG_BUILD_LUA_MODULE(pkmn_stl_lua          TRUE)

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/pkmn.lua
    ${CMAKE_CURRENT_BINARY_DIR}/pkmn.lua
@ONLY)
INSTALL(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/pkmn.lua
    DESTINATION ${LUA_SHARE_INSTALL_DIR}
    COMPONENT Lua
)

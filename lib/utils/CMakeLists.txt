#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

# Set path variables
SET(PKMN_DATABASE_PATH "${CMAKE_INSTALL_PREFIX}/${PKMN_PKG_DATA_DIR}/libpkmn.db")
SET(PKMN_IMAGES_DIR    "${CMAKE_INSTALL_PREFIX}/${PKMN_PKG_DATA_DIR}/images")

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/paths.cpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/paths.cpp
@ONLY)

SET(pkmn_utils_sources
    ${CMAKE_CURRENT_BINARY_DIR}/paths.cpp
PARENT_SCOPE)

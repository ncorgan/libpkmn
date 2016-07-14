#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/paths.cpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/paths.cpp
@ONLY)

SET(pkmn_utils_sources
    ${CMAKE_CURRENT_BINARY_DIR}/paths.cpp
PARENT_SCOPE)

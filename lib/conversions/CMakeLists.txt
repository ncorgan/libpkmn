#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(pkmn_conversions_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/conversions_common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gb_conversions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gen3_conversions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gen4_conversions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gen5_conversions.cpp
PARENT_SCOPE)

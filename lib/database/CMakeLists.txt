#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(pkmn_database_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/database_common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/id_to_index.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/id_to_string.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/index_to_string.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/item_entry.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lists.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/move_entry.cpp
PARENT_SCOPE)

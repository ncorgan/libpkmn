#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/config.hpp
)

#
# Create version header based on CMake variables
#
MATH(EXPR PKMN_VERSION_NUMBER "(${PKMN_VERSION_MAJOR} * 1000000) +
                               (${PKMN_VERSION_MINOR} * 1000) +
                                ${PKMN_VERSION_PATCH}")
CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.hpp
)

IF(NOT PKMN_DONT_INSTALL_HEADERS)
    SET(pkmn_headers
        build_info.hpp
        ${CMAKE_CURRENT_BINARY_DIR}/config.hpp
        exception.hpp
        game_save.hpp
        item_bag.hpp
        item_list.hpp
        item_slot.hpp
        move_slot.hpp
        pokemon.hpp
        pokemon_box.hpp
        pokemon_party.hpp
        pokemon_pc.hpp
        ${CMAKE_CURRENT_BINARY_DIR}/version.hpp
    )

    INSTALL(
        FILES ${pkmn_headers}
        DESTINATION ${PKMN_INCLUDE_DIR}/pkmn
        COMPONENT Headers
    )
ENDIF()

ADD_SUBDIRECTORY(calculations)
ADD_SUBDIRECTORY(database)
ADD_SUBDIRECTORY(qt)
ADD_SUBDIRECTORY(types)
ADD_SUBDIRECTORY(utils)

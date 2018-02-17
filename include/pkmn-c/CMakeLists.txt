#
# Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

#
# Create version header based on CMake variables
#
MATH(EXPR PKMN_VERSION_NUMBER "(${PKMN_VERSION_MAJOR} * 1000000) +
                               (${PKMN_VERSION_MINOR} * 1000) +
                                ${PKMN_VERSION_PATCH}")
CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.h
)

IF(NOT PKMN_DONT_INSTALL_HEADERS)
    SET(pkmn_c_headers
        build_info.h
        config.h
        error.h
        game_save.h
        item_bag.h
        item_list.h
        pokedex.h
        pokemon.h
        pokemon_box.h
        pokemon_party.h
        pokemon_pc.h
        ${CMAKE_CURRENT_BINARY_DIR}/version.h
    )

    INSTALL(
        FILES ${pkmn_c_headers}
        DESTINATION ${PKMN_INCLUDE_DIR}/pkmn-c
        COMPONENT Headers
    )
ENDIF()

ADD_SUBDIRECTORY(calculations)
ADD_SUBDIRECTORY(database)
ADD_SUBDIRECTORY(gui)
ADD_SUBDIRECTORY(types)
ADD_SUBDIRECTORY(utils)

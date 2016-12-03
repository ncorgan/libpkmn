#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/config.hpp
)

IF(NOT PKMN_DONT_INSTALL_HEADERS)
    SET(pkmn_headers
        build_info.hpp
        ${CMAKE_CURRENT_BINARY_DIR}/config.hpp
        item_bag.hpp
        item_list.hpp
        item_slot.hpp
        move_slot.hpp
        pokemon.hpp
        pokemon_box.hpp
    )

    INSTALL(
        FILES ${pkmn_headers}
        DESTINATION ${PKMN_INCLUDE_DIR}/pkmn
        COMPONENT Headers
    )
ENDIF(NOT PKMN_DONT_INSTALL_HEADERS)

ADD_SUBDIRECTORY(calculations)
ADD_SUBDIRECTORY(database)
ADD_SUBDIRECTORY(types)
ADD_SUBDIRECTORY(utils)

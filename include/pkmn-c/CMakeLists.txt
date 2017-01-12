#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(NOT PKMN_DONT_INSTALL_HEADERS)
    SET(pkmn_c_headers
        build_info.h
        config.h
        error.h
        item_bag.h
        item_list.h
        pokemon.h
        pokemon_box.h
        pokemon_pc.h
    )

    INSTALL(
        FILES ${pkmn_c_headers}
        DESTINATION ${PKMN_INCLUDE_DIR}/pkmn-c
        COMPONENT Headers
    )
ENDIF(NOT PKMN_DONT_INSTALL_HEADERS)

ADD_SUBDIRECTORY(calculations)
ADD_SUBDIRECTORY(database)
ADD_SUBDIRECTORY(types)
ADD_SUBDIRECTORY(utils)

#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(NOT PKMN_DONT_INSTALL_HEADERS)
    SET(pkmn_c_types_headers
        datetime.h
        gender.h
        hidden_power.h
        int_pair.h
        item_pockets.h
        item_slot.h
        levelup_move.h
        move_slot.h
        pokemon_list.h
        pokemon_box_list.h
        stats.h
        string_types.h
        trainer_info.h
    )

    INSTALL(
        FILES ${pkmn_c_types_headers}
        DESTINATION ${PKMN_INCLUDE_DIR}/pkmn-c/types
        COMPONENT Headers
    )
ENDIF(NOT PKMN_DONT_INSTALL_HEADERS)

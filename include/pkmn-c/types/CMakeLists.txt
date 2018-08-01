#
# Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(NOT PKMN_DONT_INSTALL_HEADERS)
    SET(pkmn_c_types_headers
        attribute_names.h
        hidden_power.h
        int_pair.h
        item_slot.h
        levelup_move.h
        move_slot.h
        natural_gift.h
        pokemon_list.h
        pokemon_box_list.h
        stat_list.h
        string_types.h
        time_duration.h
        trainer_id.h
        trainer_info.h
    )

    INSTALL(
        FILES ${pkmn_c_types_headers}
        DESTINATION ${PKMN_INCLUDE_DIR}/pkmn-c/types
        COMPONENT Headers
    )
ENDIF()

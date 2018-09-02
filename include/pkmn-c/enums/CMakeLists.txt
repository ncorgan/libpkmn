#
# Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(NOT PKMN_DONT_INSTALL_HEADERS)
    SET(pkmn_c_types_headers
        ability.h
        ball.h
        condition.h
        contest_stat.h
        egg_group.h
        enum_to_string.h
        game.h
        game_save_type.h
        gender.h
        item.h
        language.h
        marking.h
        move.h
        move_damage_class.h
        move_target.h
        nature.h
        species.h
        stat.h
        type.h
    )

    INSTALL(
        FILES ${pkmn_c_types_headers}
        DESTINATION ${PKMN_INCLUDE_DIR}/pkmn-c/types
        COMPONENT Headers
    )
ENDIF()

/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/database/lists.h>

#include <pkmn/database/lists.hpp>

enum pkmn_error pkmn_database_ability_list(
    int generation,
    struct pkmn_string_list* p_ability_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_ability_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_ability_list(
                generation
            ),
            p_ability_list_out
        )
    )
}

enum pkmn_error pkmn_database_game_list(
    int generation,
    bool include_previous,
    struct pkmn_string_list* p_game_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_game_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_game_list(
                generation, include_previous
            ),
            p_game_list_out
        )
    )
}

enum pkmn_error pkmn_database_gamecube_shadow_pokemon_list(
    bool colosseum,
    struct pkmn_string_list* p_gamecube_shadow_pokemon_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_gamecube_shadow_pokemon_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_gamecube_shadow_pokemon_list(colosseum),
            p_gamecube_shadow_pokemon_list_out
        );
    )
}

enum pkmn_error pkmn_database_hm_move_list(
    const char* p_game,
    struct pkmn_string_list* p_hm_move_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_game);
    PKMN_CHECK_NULL_PARAM(p_hm_move_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_hm_move_list(
                p_game
            ),
            p_hm_move_list_out
        )
    )
}

enum pkmn_error pkmn_database_item_list(
    const char* p_game,
    struct pkmn_string_list* p_item_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_game);
    PKMN_CHECK_NULL_PARAM(p_item_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_item_list(
                p_game
            ),
            p_item_list_out
        )
    )
}

enum pkmn_error pkmn_database_location_list(
    const char* p_game,
    bool whole_generation,
    struct pkmn_string_list* p_location_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_game);
    PKMN_CHECK_NULL_PARAM(p_location_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_location_list(
                p_game, whole_generation
            ),
            p_location_list_out
        )
    )
}

enum pkmn_error pkmn_database_move_list(
    const char* p_game,
    struct pkmn_string_list* p_move_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_game);
    PKMN_CHECK_NULL_PARAM(p_move_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_move_list(
                p_game
            ),
            p_move_list_out
        )
    )
}

enum pkmn_error pkmn_database_nature_list(
    struct pkmn_string_list* p_nature_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_nature_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_nature_list(),
            p_nature_list_out
        )
    )
}

enum pkmn_error pkmn_database_pokemon_list(
    int generation,
    bool include_previous,
    struct pkmn_string_list* p_pokemon_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_pokemon_list(
                generation, include_previous
            ),
            p_pokemon_list_out
        )
    )
}

enum pkmn_error pkmn_database_region_list(
    struct pkmn_string_list* p_region_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_region_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_region_list(),
            p_region_list_out
        )
    )
}

enum pkmn_error pkmn_database_ribbon_list(
    int generation,
    struct pkmn_string_list* p_ribbon_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_ribbon_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_ribbon_list(
                generation
            ),
            p_ribbon_list_out
        )
    )
}

enum pkmn_error pkmn_database_super_training_medal_list(
    struct pkmn_string_list* p_super_training_medal_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_super_training_medal_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_super_training_medal_list(),
            p_super_training_medal_list_out
        )
    )
}

enum pkmn_error pkmn_database_tm_move_list(
    const char* p_game,
    struct pkmn_string_list* p_tm_move_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_game);
    PKMN_CHECK_NULL_PARAM(p_tm_move_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_tm_move_list(
                p_game
            ),
            p_tm_move_list_out
        )
    )
}

enum pkmn_error pkmn_database_type_list(
    const char* p_game,
    struct pkmn_string_list* p_type_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_game);
    PKMN_CHECK_NULL_PARAM(p_type_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_type_list(
                p_game
            ),
            p_type_list_out
        )
    )
}

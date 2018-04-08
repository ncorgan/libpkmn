/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
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
    struct pkmn_string_list* ability_list_out
)
{
    PKMN_CHECK_NULL_PARAM(ability_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_ability_list(
                generation
            ),
            ability_list_out
        )
    )
}

enum pkmn_error pkmn_database_game_list(
    int generation,
    bool include_previous,
    struct pkmn_string_list* game_list_out
)
{
    PKMN_CHECK_NULL_PARAM(game_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_game_list(
                generation, include_previous
            ),
            game_list_out
        )
    )
}

enum pkmn_error pkmn_database_gamecube_shadow_pokemon_list(
    bool colosseum,
    struct pkmn_string_list* gamecube_shadow_pokemon_list_out
)
{
    PKMN_CHECK_NULL_PARAM(gamecube_shadow_pokemon_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_gamecube_shadow_pokemon_list(colosseum),
            gamecube_shadow_pokemon_list_out
        );
    )
}

enum pkmn_error pkmn_database_item_list(
    const char* game,
    struct pkmn_string_list* item_list_out
)
{
    PKMN_CHECK_NULL_PARAM(game);
    PKMN_CHECK_NULL_PARAM(item_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_item_list(
                game
            ),
            item_list_out
        )
    )
}

enum pkmn_error pkmn_database_location_list(
    const char* game,
    bool whole_generation,
    struct pkmn_string_list* location_list_out
)
{
    PKMN_CHECK_NULL_PARAM(game);
    PKMN_CHECK_NULL_PARAM(location_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_location_list(
                game, whole_generation
            ),
            location_list_out
        )
    )
}

enum pkmn_error pkmn_database_move_list(
    const char* game,
    struct pkmn_string_list* move_list_out
)
{
    PKMN_CHECK_NULL_PARAM(game);
    PKMN_CHECK_NULL_PARAM(move_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_move_list(
                game
            ),
            move_list_out
        )
    )
}

enum pkmn_error pkmn_database_nature_list(
    struct pkmn_string_list* nature_list_out
)
{
    PKMN_CHECK_NULL_PARAM(nature_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_nature_list(),
            nature_list_out
        )
    )
}

enum pkmn_error pkmn_database_pokemon_list(
    int generation,
    bool include_previous,
    struct pkmn_string_list* pokemon_list_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_pokemon_list(
                generation, include_previous
            ),
            pokemon_list_out
        )
    )
}

enum pkmn_error pkmn_database_region_list(
    struct pkmn_string_list* region_list_out
)
{
    PKMN_CHECK_NULL_PARAM(region_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_region_list(),
            region_list_out
        )
    )
}

enum pkmn_error pkmn_database_ribbon_list(
    int generation,
    struct pkmn_string_list* ribbon_list_out
)
{
    PKMN_CHECK_NULL_PARAM(ribbon_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_ribbon_list(
                generation
            ),
            ribbon_list_out
        )
    )
}

enum pkmn_error pkmn_database_super_training_medal_list(
    struct pkmn_string_list* super_training_medal_list_out
)
{
    PKMN_CHECK_NULL_PARAM(super_training_medal_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_super_training_medal_list(),
            super_training_medal_list_out
        )
    )
}

enum pkmn_error pkmn_database_type_list(
    const char* game,
    struct pkmn_string_list* type_list_out
)
{
    PKMN_CHECK_NULL_PARAM(game);
    PKMN_CHECK_NULL_PARAM(type_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_type_list(
                game
            ),
            type_list_out
        )
    )
}

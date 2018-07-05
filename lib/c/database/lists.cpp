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

enum pkmn_error pkmn_database_ability_name_list(
    int generation,
    struct pkmn_ability_enum_list* p_ability_enum_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_ability_enum_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::list_cpp_to_c<pkmn::e_ability, enum pkmn_ability, struct pkmn_ability_enum_list>(
            pkmn::database::get_ability_list(generation),
            p_ability_enum_list_out
        );
    )
}

enum pkmn_error pkmn_database_ability_name_list(
    int generation,
    struct pkmn_string_list* p_ability_name_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_ability_name_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_ability_name_list(
                generation
            ),
            p_ability_name_list_out
        )
    )
}

enum pkmn_error pkmn_database_game_list(
    int generation,
    bool include_previous,
    struct pkmn_game_enum_list* p_game_enum_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_game_enum_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::list_cpp_to_c<pkmn::e_game, enum pkmn_game, struct pkmn_game_enum_list>(
            pkmn::database::get_game_list(
                generation,
                include_previous
            ),
            p_game_enum_list_out
        );
    )
}

enum pkmn_error pkmn_database_game_name_list(
    int generation,
    bool include_previous,
    struct pkmn_string_list* p_game_name_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_game_name_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_game_name_list(
                generation, include_previous
            ),
            p_game_name_list_out
        )
    )
}

enum pkmn_error pkmn_database_gamecube_shadow_pokemon_list(
    bool colosseum,
    struct pkmn_species_enum_list* p_gamecube_shadow_pokemon_enum_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_gamecube_shadow_pokemon_enum_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::list_cpp_to_c<pkmn::e_species, enum pkmn_species, struct pkmn_species_enum_list>(
            pkmn::database::get_gamecube_shadow_pokemon_list(colosseum),
            p_gamecube_shadow_pokemon_enum_list_out
        );
    )
}

enum pkmn_error pkmn_database_gamecube_shadow_pokemon_name_list(
    bool colosseum,
    struct pkmn_string_list* p_gamecube_shadow_pokemon_name_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_gamecube_shadow_pokemon_name_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_gamecube_shadow_pokemon_name_list(colosseum),
            p_gamecube_shadow_pokemon_name_list_out
        );
    )
}

enum pkmn_error pkmn_database_hm_move_list(
    enum pkmn_game game,
    struct pkmn_move_enum_list* p_hm_move_enum_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_hm_move_enum_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::list_cpp_to_c<pkmn::e_move, enum pkmn_move, struct pkmn_move_enum_list>(
            pkmn::database::get_hm_move_list(static_cast<pkmn::e_game>(game)),
            p_hm_move_enum_list_out
        );
    )
}

enum pkmn_error pkmn_database_hm_move_name_list(
    enum pkmn_game game,
    struct pkmn_string_list* p_hm_move_name_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_hm_move_name_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_hm_move_name_list(static_cast<pkmn::e_game>(game)),
            p_hm_move_name_list_out
        )
    )
}

enum pkmn_error pkmn_database_item_list(
    enum pkmn_game game,
    struct pkmn_item_enum_list* p_item_enum_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_item_enum_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::list_cpp_to_c<pkmn::e_item, enum pkmn_item, struct pkmn_item_enum_list>(
            pkmn::database::get_item_list(static_cast<pkmn::e_game>(game)),
            p_item_enum_list_out
        );
    )
}

enum pkmn_error pkmn_database_item_name_list(
    enum pkmn_game game,
    struct pkmn_string_list* p_item_name_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_item_name_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_item_name_list(static_cast<pkmn::e_game>(game)),
            p_item_name_list_out
        )
    )
}

enum pkmn_error pkmn_database_location_name_list(
    enum pkmn_game game,
    bool whole_generation,
    struct pkmn_string_list* p_location_name_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_location_name_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_location_name_list(
                static_cast<pkmn::e_game>(game),
                whole_generation
            ),
            p_location_name_list_out
        )
    )
}

enum pkmn_error pkmn_database_move_list(
    enum pkmn_game game,
    struct pkmn_move_enum_list* p_move_enum_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_move_enum_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::list_cpp_to_c<pkmn::e_move, enum pkmn_move, struct pkmn_move_enum_list>(
            pkmn::database::get_move_list(static_cast<pkmn::e_game>(game)),
            p_move_enum_list_out
        );
    )
}

enum pkmn_error pkmn_database_move_name_list(
    enum pkmn_game game,
    struct pkmn_string_list* p_move_name_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_move_name_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_move_name_list(static_cast<pkmn::e_game>(game)),
            p_move_name_list_out
        )
    )
}

enum pkmn_error pkmn_database_nature_list(
    struct pkmn_nature_enum_list* p_nature_enum_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_nature_enum_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::list_cpp_to_c<pkmn::e_nature, enum pkmn_nature, struct pkmn_nature_enum_list>(
            pkmn::database::get_nature_list(),
            p_nature_enum_list_out
        );
    )
}

enum pkmn_error pkmn_database_nature_name_list(
    struct pkmn_string_list* p_nature_name_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_nature_name_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_nature_name_list(),
            p_nature_name_list_out
        )
    )
}

enum pkmn_error pkmn_database_pokemon_list(
    int generation,
    bool include_previous,
    struct pkmn_species_enum_list* p_pokemon_enum_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_enum_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::list_cpp_to_c<pkmn::e_species, enum pkmn_species, struct pkmn_species_enum_list>(
            pkmn::database::get_pokemon_list(
                generation,
                include_previous
            ),
            p_pokemon_enum_list_out
        );
    )
}

enum pkmn_error pkmn_database_pokemon_name_list(
    int generation,
    bool include_previous,
    struct pkmn_string_list* p_pokemon_name_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_name_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_pokemon_name_list(
                generation, include_previous
            ),
            p_pokemon_name_list_out
        )
    )
}

enum pkmn_error pkmn_database_region_name_list(
    struct pkmn_string_list* p_region_name_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_region_name_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_region_name_list(),
            p_region_name_list_out
        )
    )
}

enum pkmn_error pkmn_database_ribbon_name_list(
    int generation,
    struct pkmn_string_list* p_ribbon_name_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_ribbon_name_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_ribbon_name_list(
                generation
            ),
            p_ribbon_name_list_out
        )
    )
}

enum pkmn_error pkmn_database_super_training_medal_name_list(
    struct pkmn_string_list* p_super_training_medal_name_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_super_training_medal_name_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_super_training_medal_name_list(),
            p_super_training_medal_name_list_out
        )
    )
}

enum pkmn_error pkmn_database_tm_move_list(
    enum pkmn_game game,
    struct pkmn_move_enum_list* p_tm_move_enum_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_tm_move_enum_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::list_cpp_to_c<pkmn::e_move, enum pkmn_move, struct pkmn_move_enum_list>(
            pkmn::database::get_tm_move_list(static_cast<pkmn::e_game>(game)),
            p_tm_move_enum_list_out
        );
    )
}

enum pkmn_error pkmn_database_tm_move_name_list(
    enum pkmn_game game,
    struct pkmn_string_list* p_tm_move_name_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_tm_move_name_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_tm_move_name_list(static_cast<pkmn::e_game>(game)),
            p_tm_move_name_list_out
        )
    )
}

enum pkmn_error pkmn_database_type_list(
    enum pkmn_game game,
    struct pkmn_type_enum_list* p_type_enum_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_type_enum_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::list_cpp_to_c<pkmn::e_type, enum pkmn_type, struct pkmn_type_enum_list>(
            pkmn::database::get_type_list(static_cast<pkmn::e_game>(game)),
            p_type_enum_list_out
        );
    )
}

enum pkmn_error pkmn_database_type_name_list(
    enum pkmn_game game,
    struct pkmn_string_list* p_type_name_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_type_name_list_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_list_cpp_to_c(
            pkmn::database::get_type_name_list(static_cast<pkmn::e_game>(game)),
            p_type_name_list_out
        )
    )
}

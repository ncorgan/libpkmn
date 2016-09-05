/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/database/lists.h>

#include <pkmn/database/lists.hpp>

pkmn_error_t pkmn_database_ability_list(
    int generation,
    pkmn_string_list_t* ability_list_out,
    size_t* list_length_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_vector_std_string_to_string_list(
            pkmn::database::get_ability_list(
                generation
            ),
            ability_list_out,
            list_length_out
        )
    )
}

pkmn_error_t pkmn_database_game_list(
    int generation,
    bool include_previous,
    pkmn_string_list_t* game_list_out,
    size_t* list_length_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_vector_std_string_to_string_list(
            pkmn::database::get_game_list(
                generation, include_previous
            ),
            game_list_out,
            list_length_out
        )
    )
}

pkmn_error_t pkmn_database_item_list(
    const char* game,
    pkmn_string_list_t* item_list_out,
    size_t* list_length_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_vector_std_string_to_string_list(
            pkmn::database::get_item_list(
                game
            ),
            item_list_out,
            list_length_out
        )
    )
}

pkmn_error_t pkmn_database_location_list(
    const char* game,
    bool whole_generation,
    pkmn_string_list_t* location_list_out,
    size_t* list_length_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_vector_std_string_to_string_list(
            pkmn::database::get_location_list(
                game, whole_generation
            ),
            location_list_out,
            list_length_out
        )
    )
}

pkmn_error_t pkmn_database_move_list(
    const char* game,
    pkmn_string_list_t* move_list_out,
    size_t* list_length_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_vector_std_string_to_string_list(
            pkmn::database::get_move_list(
                game
            ),
            move_list_out,
            list_length_out
        )
    )
}

pkmn_error_t pkmn_database_nature_list(
    pkmn_string_list_t* nature_list_out,
    size_t* list_length_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_vector_std_string_to_string_list(
            pkmn::database::get_nature_list(),
            nature_list_out,
            list_length_out
        )
    )
}

pkmn_error_t pkmn_database_pokemon_list(
    int generation,
    bool include_previous,
    pkmn_string_list_t* pokemon_list_out,
    size_t* list_length_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_vector_std_string_to_string_list(
            pkmn::database::get_pokemon_list(
                generation, include_previous
            ),
            pokemon_list_out,
            list_length_out
        )
    )
}

pkmn_error_t pkmn_database_region_list(
    pkmn_string_list_t* region_list_out,
    size_t* list_length_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_vector_std_string_to_string_list(
            pkmn::database::get_region_list(),
            region_list_out,
            list_length_out
        )
    )
}

pkmn_error_t pkmn_database_ribbon_list(
    int generation,
    pkmn_string_list_t* ribbon_list_out,
    size_t* list_length_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_vector_std_string_to_string_list(
            pkmn::database::get_ribbon_list(
                generation
            ),
            ribbon_list_out,
            list_length_out
        )
    )
}

pkmn_error_t pkmn_database_super_training_medal_list(
    pkmn_string_list_t* super_training_medal_list_out,
    size_t* list_length_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_vector_std_string_to_string_list(
            pkmn::database::get_super_training_medal_list(),
            super_training_medal_list_out,
            list_length_out
        )
    )
}

pkmn_error_t pkmn_database_type_list(
    const char* game,
    pkmn_string_list_t* type_list_out,
    size_t* list_length_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_vector_std_string_to_string_list(
            pkmn::database::get_type_list(
                game
            ),
            type_list_out,
            list_length_out
        )
    )
}

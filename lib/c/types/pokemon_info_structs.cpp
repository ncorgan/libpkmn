/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/types/pokemon_info_structs.h>

pkmn_error_t pkmn_pokemon_filepaths_free(
    pkmn_pokemon_filepaths_t* pokemon_filepaths_ptr
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_filepaths_ptr);

    pkmn::c::free_pointer_and_set_to_null(
        &pokemon_filepaths_ptr->icon_filepath
    );
    pkmn::c::free_pointer_and_set_to_null(
        &pokemon_filepaths_ptr->sprite_filepath
    );

    return PKMN_ERROR_NONE;
}

pkmn_error_t pkmn_pokemon_info_free(
    pkmn_pokemon_info_t* pokemon_info_ptr
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_info_ptr);

    pokemon_info_ptr->is_shiny = false;
    pokemon_info_ptr->is_egg = false;
    pokemon_info_ptr->pokerus_duration = 0;

    pokemon_info_ptr->personality = 0U;
    pokemon_info_ptr->gender = PKMN_GENDER_MALE;
    pkmn::c::free_pointer_and_set_to_null(
        &pokemon_info_ptr->ability
    );

    pkmn::c::free_pointer_and_set_to_null(
        &pokemon_info_ptr->nickname
    );
    pkmn::c::free_pointer_and_set_to_null(
        &pokemon_info_ptr->held_item
    );

    pokemon_info_ptr->experience = 0;

    return PKMN_ERROR_NONE;
}

pkmn_error_t pkmn_pokemon_origin_info_free(
    pkmn_pokemon_origin_info_t* pokemon_origin_info_ptr
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_origin_info_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;

    error = pkmn_trainer_info_free(
                &pokemon_origin_info_ptr->original_trainer_info
            );
    if(!error)
    {
        pkmn::c::free_pointer_and_set_to_null(
            &pokemon_origin_info_ptr->original_game
        );
        pkmn::c::free_pointer_and_set_to_null(
            &pokemon_origin_info_ptr->ball
        );
    }

    return error;
}

pkmn_error_t pkmn_pokemon_current_trainer_info_free(
    pkmn_pokemon_current_trainer_info_t* pokemon_current_trainer_info_ptr
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_current_trainer_info_ptr);

    pokemon_current_trainer_info_ptr->current_trainer_friendship = 0;

    pokemon_current_trainer_info_ptr->level_met = 0;

    pkmn::c::free_pointer_and_set_to_null(
        &pokemon_current_trainer_info_ptr->location_met
    );
    pkmn::c::free_pointer_and_set_to_null(
        &pokemon_current_trainer_info_ptr->location_met_as_egg
    );

    return PKMN_ERROR_NONE;
}

pkmn_error_t pkmn_pokemon_battle_info_free(
    pkmn_pokemon_battle_info_t* pokemon_battle_info_ptr
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_battle_info_ptr);

    pokemon_battle_info_ptr->current_hp = 0;

    pokemon_battle_info_ptr->condition = PKMN_CONDITION_NONE;

    return PKMN_ERROR_NONE;
}

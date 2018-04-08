/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/database/pokemon_entry.h>

#include <pkmn/database/pokemon_entry.hpp>

#include <cstdlib>
#include <cstring>

enum pkmn_error pkmn_database_get_pokemon_entry(
    const char* species,
    const char* game,
    const char* form,
    struct pkmn_database_pokemon_entry* pokemon_entry_out
)
{
    PKMN_CHECK_NULL_PARAM(species);
    PKMN_CHECK_NULL_PARAM(game);
    PKMN_CHECK_NULL_PARAM(form);
    PKMN_CHECK_NULL_PARAM(pokemon_entry_out);

    PKMN_CPP_TO_C(
        pkmn::database::pokemon_entry pokemon_entry_cpp(
                                          species,
                                          game,
                                          form
                                      );

        pkmn::c::pokemon_entry_cpp_to_c(
            pokemon_entry_cpp,
            pokemon_entry_out
        );
    )
}

enum pkmn_error pkmn_database_pokemon_entry_set_form(
    struct pkmn_database_pokemon_entry* pokemon_entry,
    const char* form
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_entry);
    PKMN_CHECK_NULL_PARAM(form);

    PKMN_CPP_TO_C(
        // Just to see if it exists
        pkmn::database::pokemon_entry new_entry(
                                          pokemon_entry->name,
                                          pokemon_entry->game,
                                          form
                                      );

        pkmn_database_pokemon_entry_free(pokemon_entry);
        pkmn::c::pokemon_entry_cpp_to_c(
            new_entry,
            pokemon_entry
        );
    )
}

enum pkmn_error pkmn_database_pokemon_entry_experience_at_level(
    struct pkmn_database_pokemon_entry* pokemon_entry,
    int level,
    int* experience_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_entry);
    PKMN_CHECK_NULL_PARAM(experience_out);

    PKMN_CPP_TO_C(
        *experience_out = pkmn::database::pokemon_entry(
                              pokemon_entry->name,
                              pokemon_entry->game,
                              pokemon_entry->form
                          ).get_experience_at_level(level);
    )
}

enum pkmn_error pkmn_database_pokemon_entry_level_at_experience(
    struct pkmn_database_pokemon_entry* pokemon_entry,
    int experience,
    int* level_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_entry);
    PKMN_CHECK_NULL_PARAM(level_out);

    PKMN_CPP_TO_C(
        *level_out = pkmn::database::pokemon_entry(
                         pokemon_entry->name,
                         pokemon_entry->game,
                         pokemon_entry->form
                     ).get_level_at_experience(experience);
    )
}

enum pkmn_error pkmn_database_pokemon_entry_icon_filepath(
    struct pkmn_database_pokemon_entry* pokemon_entry,
    bool shiny,
    char* icon_filepath_out,
    size_t buffer_len,
    size_t* icon_filepath_length_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_entry);
    PKMN_CHECK_NULL_PARAM(icon_filepath_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_cpp_to_c(
            pkmn::database::pokemon_entry(
                pokemon_entry->name,
                pokemon_entry->game,
                pokemon_entry->form
            ).get_icon_filepath(shiny),
            icon_filepath_out,
            buffer_len,
            icon_filepath_length_out
        );
    )
}

enum pkmn_error pkmn_database_pokemon_entry_sprite_filepath(
    struct pkmn_database_pokemon_entry* pokemon_entry,
    bool female,
    bool shiny,
    char* sprite_filepath_out,
    size_t buffer_len,
    size_t* sprite_filepath_length_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_entry);
    PKMN_CHECK_NULL_PARAM(sprite_filepath_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_cpp_to_c(
            pkmn::database::pokemon_entry(
                pokemon_entry->name,
                pokemon_entry->game,
                pokemon_entry->form
            ).get_sprite_filepath(female, shiny),
            sprite_filepath_out,
            buffer_len,
            sprite_filepath_length_out
        );
    )
}

enum pkmn_error pkmn_database_pokemon_entry_free(
    struct pkmn_database_pokemon_entry* pokemon_entry
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_entry);

    pkmn::c::free_pointer_and_set_to_null(&pokemon_entry->name);
    pkmn::c::free_pointer_and_set_to_null(&pokemon_entry->game);
    pkmn::c::free_pointer_and_set_to_null(&pokemon_entry->species);
    pkmn::c::free_pointer_and_set_to_null(&pokemon_entry->form);
    pkmn::c::free_pointer_and_set_to_null(&pokemon_entry->pokedex_entry);
    pkmn::c::free_pointer_and_set_to_null(&pokemon_entry->hidden_ability);

    pokemon_entry->height = 0.0f;
    pokemon_entry->weight = 0.0f;
    pokemon_entry->chance_male = 0.0f;
    pokemon_entry->chance_female = 0.0f;
    pokemon_entry->has_gender_differences = false;
    pokemon_entry->base_friendship = 0;
    pokemon_entry->experience_yield = 0;

    for(int index = 0; index < 7; ++index)
    {
        pokemon_entry->base_stats[index] = 0;
        pokemon_entry->EV_yields[index] = 0;
    }

    pkmn_string_pair_free(&pokemon_entry->types);
    pkmn_string_pair_free(&pokemon_entry->abilities);
    pkmn_string_pair_free(&pokemon_entry->egg_groups);

    pkmn_string_list_free(&pokemon_entry->tm_hm_moves);
    pkmn_string_list_free(&pokemon_entry->egg_moves);
    pkmn_string_list_free(&pokemon_entry->tutor_moves);
    pkmn_string_list_free(&pokemon_entry->forms);
    pkmn_string_list_free(&pokemon_entry->evolutions);

    pkmn_levelup_moves_free(&pokemon_entry->levelup_moves);

    return PKMN_ERROR_NONE;
}

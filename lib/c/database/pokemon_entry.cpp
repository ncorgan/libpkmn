/*
 * p_Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * p_Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * p_or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/database/pokemon_entry.h>

#include <pkmn/database/pokemon_entry.hpp>

#include <cstdlib>
#include <cstring>

enum pkmn_error pkmn_database_get_pokemon_entry(
    const char* p_species,
    const char* p_game,
    const char* p_form,
    struct pkmn_database_pokemon_entry* p_pokemon_entry_out
)
{
    PKMN_CHECK_NULL_PARAM(p_species);
    PKMN_CHECK_NULL_PARAM(p_game);
    PKMN_CHECK_NULL_PARAM(p_form);
    PKMN_CHECK_NULL_PARAM(p_pokemon_entry_out);

    PKMN_CPP_TO_C(
        pkmn::database::pokemon_entry pokemon_entry_cpp(
                                          p_species,
                                          p_game,
                                          p_form
                                      );

        pkmn::c::pokemon_entry_cpp_to_c(
            pokemon_entry_cpp,
            p_pokemon_entry_out
        );
    )
}

enum pkmn_error pkmn_database_pokemon_entry_set_form(
    struct pkmn_database_pokemon_entry* p_pokemon_entry,
    const char* p_form
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_entry);
    PKMN_CHECK_NULL_PARAM(p_form);

    PKMN_CPP_TO_C(
        // Just to see if it exists
        pkmn::database::pokemon_entry new_entry(
                                          p_pokemon_entry->p_name,
                                          p_pokemon_entry->p_game,
                                          p_form
                                      );

        pkmn_database_pokemon_entry_free(p_pokemon_entry);
        pkmn::c::pokemon_entry_cpp_to_c(
            new_entry,
            p_pokemon_entry
        );
    )
}

enum pkmn_error pkmn_database_pokemon_entry_experience_at_level(
    struct pkmn_database_pokemon_entry* p_pokemon_entry,
    int level,
    int* p_experience_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_entry);
    PKMN_CHECK_NULL_PARAM(p_experience_out);

    PKMN_CPP_TO_C(
        *p_experience_out = pkmn::database::pokemon_entry(
                                p_pokemon_entry->p_name,
                                p_pokemon_entry->p_game,
                                p_pokemon_entry->p_form
                            ).get_experience_at_level(level);
    )
}

enum pkmn_error pkmn_database_pokemon_entry_level_at_experience(
    struct pkmn_database_pokemon_entry* p_pokemon_entry,
    int experience,
    int* p_level_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_entry);
    PKMN_CHECK_NULL_PARAM(p_level_out);

    PKMN_CPP_TO_C(
        *p_level_out = pkmn::database::pokemon_entry(
                           p_pokemon_entry->p_name,
                           p_pokemon_entry->p_game,
                           p_pokemon_entry->p_form
                       ).get_level_at_experience(experience);
    )
}

enum pkmn_error pkmn_database_pokemon_entry_icon_filepath(
    struct pkmn_database_pokemon_entry* p_pokemon_entry,
    bool shiny,
    char* p_icon_filepath_out,
    size_t buffer_len,
    size_t* p_icon_filepath_length_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_entry);
    PKMN_CHECK_NULL_PARAM(p_icon_filepath_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_cpp_to_c(
            pkmn::database::pokemon_entry(
                p_pokemon_entry->p_name,
                p_pokemon_entry->p_game,
                p_pokemon_entry->p_form
            ).get_icon_filepath(shiny),
            p_icon_filepath_out,
            buffer_len,
            p_icon_filepath_length_out
        );
    )
}

enum pkmn_error pkmn_database_pokemon_entry_sprite_filepath(
    struct pkmn_database_pokemon_entry* p_pokemon_entry,
    bool female,
    bool shiny,
    char* p_sprite_filepath_out,
    size_t buffer_len,
    size_t* p_sprite_filepath_length_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_entry);
    PKMN_CHECK_NULL_PARAM(p_sprite_filepath_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_cpp_to_c(
            pkmn::database::pokemon_entry(
                p_pokemon_entry->p_name,
                p_pokemon_entry->p_game,
                p_pokemon_entry->p_form
            ).get_sprite_filepath(female, shiny),
            p_sprite_filepath_out,
            buffer_len,
            p_sprite_filepath_length_out
        );
    )
}

enum pkmn_error pkmn_database_pokemon_entry_free(
    struct pkmn_database_pokemon_entry* p_pokemon_entry
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon_entry);

    pkmn::c::free_pointer_and_set_to_null(&p_pokemon_entry->p_name);
    pkmn::c::free_pointer_and_set_to_null(&p_pokemon_entry->p_game);
    pkmn::c::free_pointer_and_set_to_null(&p_pokemon_entry->p_species);
    pkmn::c::free_pointer_and_set_to_null(&p_pokemon_entry->p_form);
    pkmn::c::free_pointer_and_set_to_null(&p_pokemon_entry->p_pokedex_entry);
    pkmn::c::free_pointer_and_set_to_null(&p_pokemon_entry->p_hidden_ability);

    p_pokemon_entry->height = 0.0f;
    p_pokemon_entry->weight = 0.0f;
    p_pokemon_entry->chance_male = 0.0f;
    p_pokemon_entry->chance_female = 0.0f;
    p_pokemon_entry->has_gender_differences = false;
    p_pokemon_entry->base_friendship = 0;
    p_pokemon_entry->experience_yield = 0;

    std::memset(
        p_pokemon_entry->base_stats,
        0,
        sizeof(p_pokemon_entry->base_stats)
    );
    std::memset(
        p_pokemon_entry->EV_yields,
        0,
        sizeof(p_pokemon_entry->EV_yields)
    );

    pkmn_string_pair_free(&p_pokemon_entry->types);
    pkmn_string_pair_free(&p_pokemon_entry->abilities);
    pkmn_string_pair_free(&p_pokemon_entry->egg_groups);

    pkmn_string_list_free(&p_pokemon_entry->tm_hm_moves);
    pkmn_string_list_free(&p_pokemon_entry->egg_moves);
    pkmn_string_list_free(&p_pokemon_entry->tutor_moves);
    pkmn_string_list_free(&p_pokemon_entry->forms);
    pkmn_string_list_free(&p_pokemon_entry->evolutions);

    pkmn_levelup_moves_free(&p_pokemon_entry->levelup_moves);

    return PKMN_ERROR_NONE;
}

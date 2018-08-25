/*
 * p_Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * p_Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * p_or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include "common/misc.hpp"

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
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_species,
                "p_species"
            );
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_game,
                    "p_game"
                );
    }
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_form,
                    "p_form"
                );
    }
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_pokemon_entry_out,
                    "p_pokemon_entry_out"
                );
    }
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::database::pokemon_entry pokemon_entry_cpp(
                                              p_species,
                                              p_game,
                                              p_form
                                          );

            pkmn::c::pokemon_entry_cpp_to_c(
                pokemon_entry_cpp,
                p_pokemon_entry_out
            );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

enum pkmn_error pkmn_database_pokemon_entry_set_form(
    struct pkmn_database_pokemon_entry* p_pokemon_entry,
    const char* p_form
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_pokemon_entry,
                "p_pokemon_entry"
            );
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_form,
                    "p_form"
                );
    }
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::database::pokemon_entry new_entry(
                                              p_pokemon_entry->p_name,
                                              p_pokemon_entry->p_game,
                                              p_form
                                          );

            error = pkmn_database_pokemon_entry_free(p_pokemon_entry);
            if(!error)
            {
                pkmn::c::pokemon_entry_cpp_to_c(
                    new_entry,
                    p_pokemon_entry
                );
            }
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

enum pkmn_error pkmn_database_pokemon_entry_experience_at_level(
    struct pkmn_database_pokemon_entry* p_pokemon_entry,
    int level,
    int* p_experience_out
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_pokemon_entry,
                "p_pokemon_entry"
            );
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_experience_out,
                    "p_experience_out"
                );
    }
    if(!error)
    {
        auto impl = [&]()
        {
            *p_experience_out = pkmn::database::pokemon_entry(
                                    p_pokemon_entry->p_name,
                                    p_pokemon_entry->p_game,
                                    p_pokemon_entry->p_form
                                ).get_experience_at_level(level);
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

enum pkmn_error pkmn_database_pokemon_entry_level_at_experience(
    struct pkmn_database_pokemon_entry* p_pokemon_entry,
    int experience,
    int* p_level_out
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_pokemon_entry,
                "p_pokemon_entry"
            );
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_level_out,
                    "p_level_out"
                );
    }
    if(!error)
    {
        auto impl = [&]()
        {
            *p_level_out = pkmn::database::pokemon_entry(
                               p_pokemon_entry->p_name,
                               p_pokemon_entry->p_game,
                               p_pokemon_entry->p_form
                           ).get_level_at_experience(experience);
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

enum pkmn_error pkmn_database_pokemon_entry_icon_filepath(
    struct pkmn_database_pokemon_entry* p_pokemon_entry,
    bool shiny,
    char* p_icon_filepath_out,
    size_t buffer_len,
    size_t* p_icon_filepath_length_out
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_pokemon_entry,
                "p_pokemon_entry"
            );
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_icon_filepath_out,
                    "p_icon_filepath_out"
                );
    }
    if(!error)
    {
        auto impl = [&]()
        {
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
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
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
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_pokemon_entry,
                "p_pokemon_entry"
            );
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_sprite_filepath_out,
                    "p_sprite_filepath_out"
                );
    }
    if(!error)
    {
        auto impl = [&]()
        {
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
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

enum pkmn_error pkmn_database_pokemon_entry_free(
    struct pkmn_database_pokemon_entry* p_pokemon_entry
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_pokemon_entry,
                "p_pokemon_entry"
            );
    if(!error)
    {
        auto impl = [&]()
        {
            error = pkmn_string_pair_free(&p_pokemon_entry->types);
            if(!error)
            {
                error = pkmn_string_pair_free(&p_pokemon_entry->abilities);
            }
            if(!error)
            {
                error = pkmn_string_pair_free(&p_pokemon_entry->egg_groups);
            }

            if(!error)
            {
                error = pkmn_string_list_free(&p_pokemon_entry->tm_hm_moves);
            }
            if(!error)
            {
                error = pkmn_string_list_free(&p_pokemon_entry->egg_moves);
            }
            if(!error)
            {
                error = pkmn_string_list_free(&p_pokemon_entry->tutor_moves);
            }
            if(!error)
            {
                error = pkmn_string_list_free(&p_pokemon_entry->forms);
            }
            if(!error)
            {
                error = pkmn_string_list_free(&p_pokemon_entry->evolutions);
            }

            if(!error)
            {
                error = pkmn_levelup_moves_free(&p_pokemon_entry->levelup_moves);
            }

            if(!error)
            {
                pkmn::c::free_pointer_and_set_to_null(&p_pokemon_entry->p_name);
                pkmn::c::free_pointer_and_set_to_null(&p_pokemon_entry->p_game);
                pkmn::c::free_pointer_and_set_to_null(&p_pokemon_entry->p_species);
                pkmn::c::free_pointer_and_set_to_null(&p_pokemon_entry->p_form);
                pkmn::c::free_pointer_and_set_to_null(&p_pokemon_entry->p_pokedex_entry);
                pkmn::c::free_pointer_and_set_to_null(&p_pokemon_entry->p_hidden_ability);

                std::memset(p_pokemon_entry, 0, sizeof(*p_pokemon_entry));
            }
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

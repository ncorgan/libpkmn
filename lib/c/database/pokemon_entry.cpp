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
    enum pkmn_species species,
    enum pkmn_game game,
    const char* p_form,
    struct pkmn_database_pokemon_entry* p_pokemon_entry_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_param(
                                p_form,
                                "p_form"
                            );
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
                                              static_cast<pkmn::e_species>(species),
                                              static_cast<pkmn::e_game>(game),
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
    enum pkmn_error error = pkmn::c::check_for_null_param(
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
                                              static_cast<pkmn::e_species>(p_pokemon_entry->species),
                                              static_cast<pkmn::e_game>(p_pokemon_entry->game),
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
    enum pkmn_error error = pkmn::c::check_for_null_param(
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
                                    static_cast<pkmn::e_species>(p_pokemon_entry->species),
                                    static_cast<pkmn::e_game>(p_pokemon_entry->game),
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
    enum pkmn_error error = pkmn::c::check_for_null_param(
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
                               static_cast<pkmn::e_species>(p_pokemon_entry->species),
                               static_cast<pkmn::e_game>(p_pokemon_entry->game),
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
    enum pkmn_error error = pkmn::c::check_for_null_param(
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
                    static_cast<pkmn::e_species>(p_pokemon_entry->species),
                    static_cast<pkmn::e_game>(p_pokemon_entry->game),
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
    enum pkmn_error error = pkmn::c::check_for_null_param(
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
                    static_cast<pkmn::e_species>(p_pokemon_entry->species),
                    static_cast<pkmn::e_game>(p_pokemon_entry->game),
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
    enum pkmn_error error = pkmn::c::check_for_null_param(
                                p_pokemon_entry,
                                "p_pokemon_entry"
                            );
    if(!error)
    {
        auto impl = [&]()
        {
            error = pkmn_stat_enum_map_free(&p_pokemon_entry->base_stats);
            if(!error)
            {
                error = pkmn_stat_enum_map_free(&p_pokemon_entry->EV_yields);
            }

            if(!error)
            {
                error = pkmn_move_enum_list_free(&p_pokemon_entry->tm_hm_moves);
            }
            if(!error)
            {
                error = pkmn_move_enum_list_free(&p_pokemon_entry->egg_moves);
            }
            if(!error)
            {
                error = pkmn_move_enum_list_free(&p_pokemon_entry->tutor_moves);
            }
            if(!error)
            {
                error = pkmn_string_list_free(&p_pokemon_entry->forms);
            }
            if(!error)
            {
                error = pkmn_species_enum_list_free(&p_pokemon_entry->evolutions);
            }

            if(!error)
            {
                error = pkmn_levelup_moves_free(&p_pokemon_entry->levelup_moves);
            }

            if(!error)
            {
                pkmn::c::free_pointer_and_set_to_null(&p_pokemon_entry->p_species_name);
                pkmn::c::free_pointer_and_set_to_null(&p_pokemon_entry->p_category);
                pkmn::c::free_pointer_and_set_to_null(&p_pokemon_entry->p_form);
                pkmn::c::free_pointer_and_set_to_null(&p_pokemon_entry->p_pokedex_entry);

                std::memset(
                    p_pokemon_entry,
                    0,
                    sizeof(*p_pokemon_entry)
                );
            }
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "enum_maps.hpp"
#include "error_internal.hpp"

#include <pkmn-c/breeding/child_info.h>

#include <pkmn/breeding/child_info.hpp>

enum pkmn_error pkmn_breeding_get_possible_child_species(
    enum pkmn_species mother_species,
    enum pkmn_species father_species,
    enum pkmn_game game,
    struct pkmn_species_enum_list* p_possible_child_species_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_param(
                                p_possible_child_species_out,
                                "p_possible_child_species_out"
                            );
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::c::species_enum_list_cpp_to_c(
                pkmn::breeding::get_possible_child_species(
                    static_cast<pkmn::e_species>(mother_species),
                    static_cast<pkmn::e_species>(father_species),
                    static_cast<pkmn::e_game>(game)
                ),
                p_possible_child_species_out
            );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

PKMN_C_API enum pkmn_error pkmn_breeding_get_child_moves(
    const struct pkmn_pokemon* p_mother,
    const struct pkmn_pokemon* p_father,
    enum pkmn_species child_species,
    struct pkmn_move_enum_list* p_child_moves_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_param(
                                p_mother,
                                "p_mother"
                            );
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_father,
                    "p_father"
                );
    }
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_child_moves_out,
                    "p_child_moves_out"
                );
    }
    if(!error)
    {
        auto impl = [&]()
        {
            auto* p_mother_internal = pkmn::c::get_pokemon_internal_ptr(p_mother);
            auto* p_father_internal = pkmn::c::get_pokemon_internal_ptr(p_father);

            BOOST_ASSERT(p_mother_internal != nullptr);
            BOOST_ASSERT(p_father_internal != nullptr);

            pkmn::c::move_enum_list_cpp_to_c(
                pkmn::breeding::get_child_moves(
                    p_mother_internal->cpp,
                    p_father_internal->cpp,
                    static_cast<pkmn::e_species>(child_species)
                ),
                p_child_moves_out
            );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

enum pkmn_error pkmn_breeding_get_ideal_child_IVs(
    const struct pkmn_pokemon* p_mother,
    const struct pkmn_pokemon* p_father,
    enum pkmn_gender gender,
    struct pkmn_stat_enum_map* p_ideal_child_IVs_out
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_mother,
                "p_mother"
            );
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_father,
                    "p_father"
                );
    }
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_ideal_child_IVs_out,
                    "p_ideal_child_IVs_out"
                );
    }
    if(!error)
    {
        auto impl = [&]()
        {
            auto* p_mother_internal = pkmn::c::get_pokemon_internal_ptr(p_mother);
            auto* p_father_internal = pkmn::c::get_pokemon_internal_ptr(p_father);

            BOOST_ASSERT(p_mother_internal != nullptr);
            BOOST_ASSERT(p_father_internal != nullptr);

            pkmn::c::stat_enum_map_cpp_to_c(
                pkmn::breeding::get_ideal_child_IVs(
                    p_mother_internal->cpp,
                    p_father_internal->cpp,
                    static_cast<pkmn::e_gender>(gender)
                ),
                p_ideal_child_IVs_out
            );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

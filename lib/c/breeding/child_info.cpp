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
    PKMN_CHECK_NULL_PARAM(p_possible_child_species_out);

    PKMN_CPP_TO_C(
        pkmn::c::species_enum_list_cpp_to_c(
            pkmn::breeding::get_possible_child_species(
                static_cast<pkmn::e_species>(mother_species),
                static_cast<pkmn::e_species>(father_species),
                static_cast<pkmn::e_game>(game)
            ),
            p_possible_child_species_out
        );
    )
}

PKMN_C_API enum pkmn_error pkmn_breeding_get_child_moves(
    const struct pkmn_pokemon* p_mother,
    const struct pkmn_pokemon* p_father,
    enum pkmn_species child_species,
    struct pkmn_move_enum_list* p_child_moves_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(p_mother);
    PKMN_CHECK_NULL_WRAPPER_PARAM(p_father);
    PKMN_CHECK_NULL_PARAM(p_child_moves_out);

    PKMN_CPP_TO_C(
        pkmn::c::move_enum_list_cpp_to_c(
            pkmn::breeding::get_child_moves(
                POKEMON_INTERNAL_RCAST(p_mother->p_internal)->cpp,
                POKEMON_INTERNAL_RCAST(p_father->p_internal)->cpp,
                static_cast<pkmn::e_species>(child_species)
            ),
            p_child_moves_out
        );
    )
}

enum pkmn_error pkmn_breeding_get_ideal_child_IVs(
    const struct pkmn_pokemon* p_mother,
    const struct pkmn_pokemon* p_father,
    enum pkmn_gender gender,
    struct pkmn_stat_enum_map* p_ideal_child_IVs_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(p_mother);
    PKMN_CHECK_NULL_WRAPPER_PARAM(p_father);
    PKMN_CHECK_NULL_PARAM(p_ideal_child_IVs_out);

    PKMN_CPP_TO_C(
        pkmn::c::stat_enum_map_cpp_to_c(
            pkmn::breeding::get_ideal_child_IVs(
                POKEMON_INTERNAL_RCAST(p_mother->p_internal)->cpp,
                POKEMON_INTERNAL_RCAST(p_father->p_internal)->cpp,
                static_cast<pkmn::e_gender>(gender)
            ),
            p_ideal_child_IVs_out
        );
    )
}

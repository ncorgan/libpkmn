/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "error_internal.hpp"

#include <pkmn-c/breeding/compatibility.h>

#include <pkmn/breeding/compatibility.hpp>

enum pkmn_error pkmn_breeding_are_pokemon_species_compatible(
    enum pkmn_species species1,
    enum pkmn_species species2,
    bool* p_are_pokemon_species_compatible_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_param(
                                p_are_pokemon_species_compatible_out,
                                "p_are_pokemon_species_compatible_out"
                            );
    if(!error)
    {
        auto impl = [&]()
        {
            *p_are_pokemon_species_compatible_out = pkmn::breeding::are_pokemon_species_compatible(
                                                        static_cast<pkmn::e_species>(species1),
                                                        static_cast<pkmn::e_species>(species2)
                                                    );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

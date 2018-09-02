/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_BREEDING_COMPATIBILITY_HPP
#define PKMN_BREEDING_COMPATIBILITY_HPP

#include <pkmn/config.hpp>

#include <pkmn/enums/species.hpp>

namespace pkmn { namespace breeding {

    PKMN_API bool are_pokemon_species_compatible(
        pkmn::e_species species1,
        pkmn::e_species species2
    );

}}

#endif /* PKMN_BREEDING_COMPATIBILITY_HPP */

/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMN_CONVERSIONS_GB_CONVERSIONS_HPP
#define INCLUDED_PKMN_CONVERSIONS_GB_CONVERSIONS_HPP

#include <pksav/gen1/pokemon.h>
#include <pksav/gen2/pokemon.h>

namespace pkmn { namespace conversions {

    void gen1_pc_pokemon_to_gen2(
        const struct pksav_gen1_pc_pokemon* from,
        struct pksav_gen2_pc_pokemon* to
    );

    void gen1_party_pokemon_to_gen2(
        const struct pksav_gen1_party_pokemon* from,
        struct pksav_gen2_party_pokemon* to
    );

    void gen2_pc_pokemon_to_gen1(
        const struct pksav_gen2_pc_pokemon* from,
        struct pksav_gen1_pc_pokemon* to
    );

    void gen2_party_pokemon_to_gen1(
        const struct pksav_gen2_party_pokemon* from,
        struct pksav_gen1_party_pokemon* to
    );

}}

#endif /* INCLUDED_PKMN_CONVERSIONS_GB_CONVERSIONS_HPP */

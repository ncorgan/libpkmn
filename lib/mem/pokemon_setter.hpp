/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMN_MEM_POKEMON_SETTER_HPP
#define INCLUDED_PKMN_MEM_POKEMON_SETTER_HPP

#include <pkmn/config.hpp>

#include "../pokemon_impl.hpp"
#include "../pokemon_box_impl.hpp"
#include "../pokemon_party_impl.hpp"

namespace pkmn { namespace mem {

    void set_pokemon_in_box(
        pokemon_impl* new_pokemon,
        pokemon_box_impl* box,
        int index
    );

    void set_pokemon_in_party(
        pokemon_impl* new_pokemon,
        pokemon_party_impl* party,
        int index
    );

}}

#endif /* INCLUDED_PKMN_MEM_POKEMON_SETTER_HPP */

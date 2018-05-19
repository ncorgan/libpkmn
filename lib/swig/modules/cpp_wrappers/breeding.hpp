/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_WRAPPERS_BREEDING_HPP
#define CPP_WRAPPERS_BREEDING_HPP

#include "pokemon.hpp"

#include <pkmn/config.hpp>

#include <pkmn/breeding/child_info.hpp>
#include <pkmn/breeding/compatibility.hpp>

#include <boost/assert.hpp>

#include <string>
#include <vector>

namespace pkmn { namespace swig { namespace breeding {

    inline std::vector<std::string> get_possible_child_species(
        const pkmn::swig::pokemon& pokemon1,
        const pkmn::swig::pokemon& pokemon2
    )
    {
        return pkmn::breeding::get_possible_child_species(
                   pokemon1.get_internal(),
                   pokemon2.get_internal()
               );
    }

    inline bool are_pokemon_compatible(
        const pkmn::swig::pokemon& pokemon1,
        const pkmn::swig::pokemon& pokemon2
    )
    {
        return pkmn::breeding::are_pokemon_compatible(
                   pokemon1.get_internal(),
                   pokemon2.get_internal()
               );
    }

}}}

#endif /* CPP_WRAPPERS_BREEDING_HPP */

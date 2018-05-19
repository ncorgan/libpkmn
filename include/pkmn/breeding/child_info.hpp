/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_BREEDING_CHILD_INFO_HPP
#define PKMN_BREEDING_CHILD_INFO_HPP

#include <pkmn/config.hpp>

#include <pkmn/pokemon.hpp>

#include <string>
#include <vector>

namespace pkmn { namespace breeding {

    PKMN_API std::vector<std::string> get_possible_child_species(
        const pkmn::pokemon::sptr& pokemon1,
        const pkmn::pokemon::sptr& pokemon2
    );

}}

#endif /* PKMN_BREEDING_CHILD_INFO_HPP */

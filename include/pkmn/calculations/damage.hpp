/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_DAMAGE_HPP
#define PKMN_CALCULATIONS_DAMAGE_HPP

#include <pkmn/config.hpp>

#include <string>

namespace pkmn { namespace calculations {

    PKMN_API float type_damage_modifier(
        int generation,
        const std::string &attacking_type,
        const std::string &defending_type1,
        const std::string &defending_type2
    );

}}
#endif /* PKMN_CALCULATIONS_DAMAGE_HPP */

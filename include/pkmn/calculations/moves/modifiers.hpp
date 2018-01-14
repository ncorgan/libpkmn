/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_MOVES_MODIFIERS_HPP
#define PKMN_CALCULATIONS_MOVES_MODIFIERS_HPP

#include <pkmn/config.hpp>

#include <string>

namespace pkmn { namespace calculations {

    PKMN_CONSTEXPR float STAB_MODIFIER = 1.5f;

    PKMN_API float type_damage_modifier(
        int generation,
        const std::string& attacking_type,
        const std::string& defending_type
    );

}}
#endif /* PKMN_CALCULATIONS_MOVES_MODIFIERS_HPP */

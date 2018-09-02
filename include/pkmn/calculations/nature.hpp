/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_NATURE_HPP
#define PKMN_CALCULATIONS_NATURE_HPP

#include <pkmn/config.hpp>

#include <pkmn/enums/nature.hpp>

#include <cstdint>

namespace pkmn { namespace calculations {

    //! Returns the nature of a Pokémon with the given personality.
    PKMN_API pkmn::e_nature nature(
        uint32_t personality
    );

}}
#endif /* PKMN_CALCULATIONS_NATURE_HPP */

/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_NATURE_HPP
#define PKMN_CALCULATIONS_NATURE_HPP

#include <pkmn/config.hpp>

#include <cstdint>
#include <string>

namespace pkmn { namespace calculations {

    PKMN_API std::string nature(
        uint32_t personality
    );

}}
#endif /* PKMN_CALCULATIONS_NATURE_HPP */

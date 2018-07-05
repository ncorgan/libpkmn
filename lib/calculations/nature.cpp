/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/calculations/nature.hpp>

#include <boost/config.hpp>

namespace pkmn { namespace calculations {

    pkmn::e_nature nature(
        uint32_t personality
    )
    {
        static const size_t NUM_NATURES = 25;

        return static_cast<pkmn::e_nature>((personality % NUM_NATURES) + 1);
    }

}}

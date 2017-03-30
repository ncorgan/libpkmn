/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/calculations/personality.hpp>

#include "types/rng.hpp"

namespace pkmn { namespace calculations {

    uint32_t personality_with_shininess(
        uint32_t trainer_id,
        bool shiny
    ) {
        uint32_t ret = pkmn::rng<uint32_t>().rand();

        uint16_t* p = reinterpret_cast<uint16_t*>(&ret);
        const uint16_t* t = reinterpret_cast<const uint16_t*>(&trainer_id);

        if(shiny) {
            for(size_t i = 3; i < 16; ++i) {
                size_t num_ones = 0;
                if(p[0] & (1 << i)) ++num_ones;
                if(p[1] & (1 << i)) ++num_ones;
                if(t[0] & (1 << i)) ++num_ones;
                if(t[1] & (1 << i)) ++num_ones;
                if(num_ones % 2) {
                    p[0] ^= (1 << i);
                }
            }
        } else {
            size_t num_ones = 0;
            if(p[0] & (1 << 3)) ++num_ones;
            if(p[1] & (1 << 3)) ++num_ones;
            if(t[0] & (1 << 3)) ++num_ones;
            if(t[1] & (1 << 3)) ++num_ones;
            if(!(num_ones % 2)) {
                p[0] ^= (1 << 3);
            }
        }

        return ret;
    }

}}

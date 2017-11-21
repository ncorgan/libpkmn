/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "../exception_internal.hpp"
#include "../misc_common.hpp"

#include <pkmn/calculations/shininess.hpp>

#include <boost/config.hpp>

#include <algorithm>

namespace pkmn { namespace calculations {

    bool gen2_shiny(
        int IV_attack,
        int IV_defense,
        int IV_speed,
        int IV_special
    ) {
        pkmn::enforce_IV_bounds("Attack",  IV_attack,  false);
        pkmn::enforce_IV_bounds("Defense", IV_defense, false);
        pkmn::enforce_IV_bounds("Speed",   IV_speed,   false);
        pkmn::enforce_IV_bounds("Special", IV_special, false);

        BOOST_STATIC_CONSTEXPR int valid_IV_attack[] = {2,3,6,7,10,11,14,15};

        bool ret = true;

        if((IV_defense != 10) or (IV_speed != 10) or (IV_special) != 10) {
            ret = false;
        } else if(std::find(valid_IV_attack, valid_IV_attack+8, IV_attack) == (valid_IV_attack+8)) {
            ret = false;
        }

        return ret;
    }

    bool modern_shiny(
        uint32_t personality,
        uint32_t trainer_id
    ) {
        const uint16_t* p = reinterpret_cast<const uint16_t*>(&personality);
        const uint16_t* t = reinterpret_cast<const uint16_t*>(&trainer_id);

        bool ret = true;

        for(size_t i = 3; i < 16; ++i) {
            size_t num_ones = 0;
            if(p[0] & (1 << i)) ++num_ones;
            if(p[1] & (1 << i)) ++num_ones;
            if(t[0] & (1 << i)) ++num_ones;
            if(t[1] & (1 << i)) ++num_ones;
            if(num_ones % 2) {
                ret = false;
                break;
            }
        }

        return ret;
    }

}}

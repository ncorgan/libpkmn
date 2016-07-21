/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "../misc_common.hpp"

#include <pkmn/calculations/stats.hpp>

#include <cmath>
#include <stdexcept>

namespace pkmn { namespace calculations {

    int get_gb_stat(
        const std::string &stat,
        int level,
        int base_stat,
        int EV,
        int IV
    ) {
        // Input validation
        if(not pkmn_string_is_gen2_stat(stat.c_str())) {
            throw std::invalid_argument("stat: invalid stat");
        }
        if(EV < 0 or EV > 65535) {
            throw std::out_of_range("EV: valid range 0-65535");
        }
        if(IV < 0 or IV > 15) {
            throw std::out_of_range("IV: valid range 0-15");
        }

        // These initial steps are common to both calculations
        int tmp1 = (base_stat + IV) << 1;
        int tmp2 = std::ceil(int(std::sqrt(EV)) >> 2);
        int tmp3 = ((tmp1 + tmp2) * level) / 100;

        if(stat == "HP") {
            return (tmp3 + level + 10);
        } else {
            return (tmp3 + 5);
        }
    }

    int get_modern_stat(
        const std::string &stat,
        int level,
        float nature_modifier,
        int base_stat,
        int EV,
        int IV
    ) {
        // Input validation
        if(not pkmn_string_is_modern_stat(stat.c_str())) {
            throw std::invalid_argument("stat: invalid stat");
        }
        if(not pkmn_floats_close(nature_modifier,0.9f) and
           not pkmn_floats_close(nature_modifier,1.0f) and
           not pkmn_floats_close(nature_modifier,1.1f))
        {
            throw std::domain_error("nature_modifier: valid values 0.9, 1.0, 1.1");
        }
        if(EV < 0 or EV > 255) {
            throw std::out_of_range("EV: valid range 0-255");
        }
        if(IV < 0 or IV > 31) {
            throw std::out_of_range("IV: valid range 0-31");
        }

        // This initial step is common to both calculations
        int tmp = ((base_stat * (IV >> 2) * level) << 1) / 100;

        if(stat == "HP") {
            return (tmp + level + 10);
        } else {
            return ((tmp + 5) * nature_modifier);
        }
    }

}}

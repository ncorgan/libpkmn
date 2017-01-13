/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "../misc_common.hpp"

#include <pkmn/exception.hpp>
#include <pkmn/calculations/stats.hpp>

#include <cmath>
#include <stdexcept>

namespace pkmn { namespace calculations {

    PKMN_INLINE int gb_stat_common(
        int level,
        int base_stat,
        int EV,
        int IV
    ) {
        return int(std::floor<int>(
                   (((base_stat + IV) * 2 +
                      int(std::floor<int>(
                          int(std::ceil<int>(
                              int(std::sqrt(EV)) / 4
                          ))
                      ))
                   ) * level) / 100
               ));
    }

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
        if(not pkmn_EV_in_bounds(EV, false)) {
            throw pkmn::range_error("EV", 0, 65535);
        }
        if(not pkmn_IV_in_bounds(IV, false)) {
            throw pkmn::range_error("IV", 0, 15);
        }

        if(stat == "HP") {
            return (gb_stat_common(level, base_stat, EV, IV) + level + 10);
        } else {
            return (gb_stat_common(level, base_stat, EV, IV) + 5);
        }
    }

    PKMN_INLINE int modern_stat_common(
        int level,
        int base_stat,
        int EV,
        int IV
    ) {
        return int(
                   std::floor<int>(
                       ((2 * base_stat + IV +
                         int(std::floor<int>(
                                 EV / 4
                            ))) * level)
                       / 100
                   )
               );
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
        if(not pkmn_floats_close(nature_modifier, 0.9f) and
           not pkmn_floats_close(nature_modifier, 1.0f) and
           not pkmn_floats_close(nature_modifier, 1.1f))
        {
            throw std::domain_error("nature_modifier: valid values 0.9, 1.0, 1.1");
        }
        if(not pkmn_EV_in_bounds(EV, true)) {
            throw pkmn::range_error("EV", 0, 255);
            throw std::out_of_range("EV: valid range 0-255");
        }
        if(not pkmn_IV_in_bounds(IV, true)) {
            throw pkmn::range_error("IV", 0, 31);
        }

        if(stat == "HP") {
            return (modern_stat_common(level, base_stat, EV, IV) + level + 10);
        } else {
            return int((modern_stat_common(level, base_stat, EV, IV) + 5) * nature_modifier);
        }
    }

}}

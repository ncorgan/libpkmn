/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
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
        if(not pkmn::string_is_gen2_stat(stat)) {
            pkmn::throw_invalid_argument("stat", pkmn::GEN2_STATS);
        }
        if(not pkmn::EV_in_bounds(EV, false)) {
            pkmn::throw_out_of_range("EV", 0, 65535);
        }
        if(not pkmn::IV_in_bounds(IV, false)) {
            pkmn::throw_out_of_range("IV", 0, 15);
        }

        int ret = 0;

        if(stat == "HP") {
            ret = gb_stat_common(level, base_stat, EV, IV) + level + 10;
        } else {
            ret = gb_stat_common(level, base_stat, EV, IV) + 5;
        }

        return ret;
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
        if(not pkmn::string_is_modern_stat(stat)) {
            pkmn::throw_invalid_argument("stat", pkmn::MODERN_STATS);
        }
        if(not pkmn::floats_close(nature_modifier, 0.9f) and
           not pkmn::floats_close(nature_modifier, 1.0f) and
           not pkmn::floats_close(nature_modifier, 1.1f))
        {
            throw std::domain_error("nature_modifier: valid values 0.9, 1.0, 1.1");
        }
        if(not pkmn::EV_in_bounds(EV, true)) {
            pkmn::throw_out_of_range("EV", 0, 255);
        }
        if(not pkmn::IV_in_bounds(IV, true)) {
            pkmn::throw_out_of_range("IV", 0, 31);
        }

        int ret = 0;

        if(stat == "HP") {
            ret = modern_stat_common(level, base_stat, EV, IV) + level + 10;
        } else {
            ret = int((modern_stat_common(level, base_stat, EV, IV) + 5) * nature_modifier);
        }

        return ret;
    }

}}

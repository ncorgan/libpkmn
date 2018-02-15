/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "utils/misc.hpp"
#include "utils/floating_point_comparison.hpp"

#include <pkmn/config.hpp>
#include <pkmn/calculations/stats.hpp>

#include <cmath>
#include <stdexcept>

namespace pkmn { namespace calculations {

    inline int gb_stat_common(
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
        pkmn::enforce_value_in_vector("Stat", stat, pkmn::GEN2_STATS);
        pkmn::enforce_EV_bounds(stat, EV, false);
        pkmn::enforce_IV_bounds(stat, IV, false);

        int ret = 0;

        if(stat == "HP") {
            ret = gb_stat_common(level, base_stat, EV, IV) + level + 10;
        } else {
            ret = gb_stat_common(level, base_stat, EV, IV) + 5;
        }

        return ret;
    }

    inline int modern_stat_common(
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
        if(pkmn::fp_compare_not_equal(nature_modifier, 0.9f) and
           pkmn::fp_compare_not_equal(nature_modifier, 1.0f) and
           pkmn::fp_compare_not_equal(nature_modifier, 1.1f))
        {
            throw std::domain_error("nature_modifier: valid values 0.9, 1.0, 1.1");
        }

        pkmn::enforce_value_in_vector("Stat", stat, pkmn::MODERN_STATS);
        pkmn::enforce_EV_bounds(stat, EV, true);
        pkmn::enforce_IV_bounds(stat, IV, true);

        int ret = 0;

        if(stat == "HP") {
            ret = modern_stat_common(level, base_stat, EV, IV) + level + 10;
        } else {
            ret = int((modern_stat_common(level, base_stat, EV, IV) + 5) * nature_modifier);
        }

        return ret;
    }

}}

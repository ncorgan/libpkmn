/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_STATS_HPP
#define PKMN_CALCULATIONS_STATS_HPP

#include <pkmn/config.hpp>

#include <string>
#include <utility>

namespace pkmn { namespace calculations {

    PKMN_API int get_gb_stat(
        const std::string &stat,
        int level,
        int base_stat,
        int EV,
        int IV
    );

    PKMN_INLINE std::pair<int, int> get_gb_stat_range(
        const std::string &stat,
        int level,
        int base_stat
    ) {
        return std::make_pair(
                   get_gb_stat(stat, level, base_stat, 0, 0),
                   get_gb_stat(stat, level, base_stat, 65535, 15)
               );
    }

    PKMN_API int get_modern_stat(
        const std::string &stat,
        int level,
        float nature_modifier,
        int base_stat,
        int EV,
        int IV
    );

    PKMN_INLINE std::pair<int, int> get_modern_stat_range(
        const std::string &stat,
        int level,
        int base_stat
    ) {
        return std::make_pair(
                   get_modern_stat(stat, level, 0.9f, base_stat, 0, 0),
                   get_modern_stat(stat, level, 1.1f, base_stat, 255, 31)
               );
    }
}}
#endif /* PKMN_CALCULATIONS_STATS_HPP */

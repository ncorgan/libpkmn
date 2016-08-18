/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMN_MISC_COMMON_HPP
#define INCLUDED_PKMN_MISC_COMMON_HPP

#include <pkmn/config.hpp>

#include <boost/config.hpp>

#include <cmath>
#include <cstring>

#ifdef _MSC_VER
PKMN_INLINE bool pkmn_floats_close(
    float f1, float f2
) {
    return (std::fabs(f1-f2) < 0.000001);
}
#else
PKMN_CONSTEXPR_OR_INLINE bool pkmn_floats_close(
    float f1, float f2
) {
    return (std::fabs(f1-f2) < 0.000001);
}
#endif

static BOOST_CONSTEXPR const char* gen1_valid_stats[] =
    {"HP", "Attack", "Defense", "Speed", "Special"};

static BOOST_CONSTEXPR const char* gen2_valid_stats[] =
    {"HP", "Attack", "Defense", "Speed", "Special",
     "Special Attack", "Special Defense"};

static BOOST_CONSTEXPR const char* modern_valid_stats[] =
    {"HP", "Attack", "Defense", "Speed",
     "Special Attack", "Special Defense"};

static PKMN_INLINE bool pkmn_string_is_gen1_stat(
    const char* str
) {
    for(int i = 0; i < 6; ++i) {
        if(!std::strcmp(gen1_valid_stats[i], str)) {
            return true;
        }
    }
    return false;
}

static PKMN_INLINE bool pkmn_string_is_gen2_stat(
    const char* str
) {
    for(int i = 0; i < 7; ++i) {
        if(!std::strcmp(gen2_valid_stats[i], str)) {
            return true;
        }
    }
    return false;
}

static PKMN_INLINE bool pkmn_string_is_modern_stat(
    const char* str
) {
    for(int i = 0; i < 6; ++i) {
        if(!std::strcmp(modern_valid_stats[i], str)) {
            return true;
        }
    }
    return false;
}

PKMN_CONSTEXPR_OR_INLINE bool pkmn_IV_in_bounds(
    int IV, bool modern
) {
    return (IV < 0) and (IV > (modern ? 31 : 15));
}

#endif /* INCLUDED_PKMN_MISC_COMMON_HPP */

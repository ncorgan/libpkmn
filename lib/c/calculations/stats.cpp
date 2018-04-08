/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/calculations/stats.h>

#include <pkmn/exception.hpp>
#include <pkmn/calculations/stats.hpp>

#include <boost/config.hpp>

#include <stdexcept>

static BOOST_CONSTEXPR const char* stat_names[] = {
    "HP", "Attack", "Defense", "Speed",
    "Special", "Special Attack", "Special Defense"
};

static void enforce_enum_bounds(
    enum pkmn_stat stat
)
{
    pkmn::enforce_bounds(
        "stat",
        (int)stat,
        (int)PKMN_STAT_HP,
        (int)PKMN_STAT_SPDEF
    );
}

pkmn_error_t pkmn_calculations_gb_stat(
    enum pkmn_stat stat,
    int level,
    int base_stat,
    int EV,
    int IV,
    int* stat_out
) {
    PKMN_CHECK_NULL_PARAM(stat_out);

    PKMN_CPP_TO_C(
        enforce_enum_bounds(stat);

        *stat_out = pkmn::calculations::get_gb_stat(
                        stat_names[stat], level,
                        base_stat, EV, IV
                    );
    )
}

pkmn_error_t pkmn_calculations_gb_stat_range(
    enum pkmn_stat stat,
    int level,
    int base_stat,
    struct pkmn_int_pair* stat_range_out
) {
    PKMN_CHECK_NULL_PARAM(stat_range_out);

    PKMN_CPP_TO_C(
        enforce_enum_bounds(stat);

        pkmn::c::int_pair_cpp_to_c(
            pkmn::calculations::get_gb_stat_range(
                stat_names[stat], level, base_stat
            ),
            stat_range_out
        );
    )
}

pkmn_error_t pkmn_calculations_modern_stat(
    enum pkmn_stat stat,
    int level,
    float nature_modifier,
    int base_stat,
    int EV,
    int IV,
    int* stat_out
) {
    PKMN_CHECK_NULL_PARAM(stat_out);

    PKMN_CPP_TO_C(
        enforce_enum_bounds(stat);

        *stat_out = pkmn::calculations::get_modern_stat(
                        stat_names[stat], level,
                        nature_modifier, base_stat,
                        EV, IV
                    );
    )
}

pkmn_error_t pkmn_calculations_modern_stat_range(
    enum pkmn_stat stat,
    int level,
    int base_stat,
    struct pkmn_int_pair* stat_range_out
) {
    PKMN_CHECK_NULL_PARAM(stat_range_out);

    PKMN_CPP_TO_C(
        enforce_enum_bounds(stat);

        pkmn::c::int_pair_cpp_to_c(
            pkmn::calculations::get_modern_stat_range(
                stat_names[stat], level, base_stat
            ),
            stat_range_out
        );
    )
}

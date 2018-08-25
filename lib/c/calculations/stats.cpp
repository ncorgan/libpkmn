/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"

#include "cpp_to_c.hpp"
#include "enum_maps.hpp"
#include "error_internal.hpp"

#include <pkmn-c/calculations/stats.h>

#include <pkmn/exception.hpp>
#include <pkmn/calculations/stats.hpp>

enum pkmn_error pkmn_calculations_gb_stat(
    enum pkmn_stat stat,
    int level,
    int base_stat,
    int EV,
    int IV,
    int* p_stat_out
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_stat_out,
                "p_stat_out"
            );
    if(!error)
    {
        auto impl = [&]()
        {
            const pkmn::c::stat_bimap_t& stat_bimap = pkmn::c::get_stat_bimap();
            pkmn::enforce_value_in_map_keys(
                "Stat",
                stat,
                stat_bimap.right
            );

            *p_stat_out = pkmn::calculations::get_gb_stat(
                              stat_bimap.right.at(stat), level,
                              base_stat, EV, IV
                          );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

enum pkmn_error pkmn_calculations_gb_stat_range(
    enum pkmn_stat stat,
    int level,
    int base_stat,
    struct pkmn_int_pair* p_stat_range_out
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_stat_range_out,
                "p_stat_range_out"
            );
    if(!error)
    {
        auto impl = [&]()
        {
            const pkmn::c::stat_bimap_t& stat_bimap = pkmn::c::get_stat_bimap();
            pkmn::enforce_value_in_map_keys(
                "Stat",
                stat,
                stat_bimap.right
            );

            pkmn::c::int_pair_cpp_to_c(
                pkmn::calculations::get_gb_stat_range(
                    stat_bimap.right.at(stat), level, base_stat
                ),
                p_stat_range_out
            );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

enum pkmn_error pkmn_calculations_modern_stat(
    enum pkmn_stat stat,
    int level,
    float nature_modifier,
    int base_stat,
    int EV,
    int IV,
    int* p_stat_out
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_stat_out,
                "p_stat_out"
            );
    if(!error)
    {
        auto impl = [&]()
        {
            const pkmn::c::stat_bimap_t& stat_bimap = pkmn::c::get_stat_bimap();
            pkmn::enforce_value_in_map_keys(
                "Stat",
                stat,
                stat_bimap.right
            );

            *p_stat_out = pkmn::calculations::get_modern_stat(
                              stat_bimap.right.at(stat), level,
                              nature_modifier, base_stat,
                              EV, IV
                          );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

enum pkmn_error pkmn_calculations_modern_stat_range(
    enum pkmn_stat stat,
    int level,
    int base_stat,
    struct pkmn_int_pair* p_stat_range_out
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_stat_range_out,
                "p_stat_range_out"
            );
    if(!error)
    {
        auto impl = [&]()
        {
            const pkmn::c::stat_bimap_t& stat_bimap = pkmn::c::get_stat_bimap();
            pkmn::enforce_value_in_map_keys(
                "Stat",
                stat,
                stat_bimap.right
            );

            pkmn::c::int_pair_cpp_to_c(
                pkmn::calculations::get_modern_stat_range(
                    stat_bimap.right.at(stat), level, base_stat
                ),
                p_stat_range_out
            );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

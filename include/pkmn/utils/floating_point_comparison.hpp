/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_UTILS_FLOATING_POINT_COMPARISON_HPP
#define PKMN_UTILS_FLOATING_POINT_COMPARISON_HPP

#include <pkmn/config.hpp>

#include <cmath>
#include <limits>
#include <type_traits>

namespace pkmn
{
    template <class T>
    bool fp_compare_equal(
        T value1,
        T value2,
        T epsilon = 0.000000001f
    )
    {
        return (std::fabs(value1 - value2) < epsilon);
    }

    template <class T>
    bool fp_compare_not_equal(
        T value1,
        T value2,
        T epsilon = 0.000000001f
    )
    {
        return (not fp_compare_equal(value1, value2, epsilon));
    }

    template <class T>
    bool fp_compare_less(
        T value1,
        T value2,
        T epsilon = 0.000000001f
    )
    {
        return (value1 < value2) and fp_compare_not_equal(value1, value2, epsilon);
    }

    template <class T>
    bool fp_compare_greater(
        T value1,
        T value2,
        T epsilon = 0.000000001f
    )
    {
        return (value1 > value2) and fp_compare_not_equal(value1, value2, epsilon);
    }
}
#endif /* PKMN_UTILS_FLOATING_POINT_COMPARISON_HPP */

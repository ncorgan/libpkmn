/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_MOVES_NATURAL_GIFT_HPP
#define PKMN_CALCULATIONS_MOVES_NATURAL_GIFT_HPP

#include <pkmn/config.hpp>

#include <string>

namespace pkmn { namespace calculations {

    struct natural_gift
    {
        natural_gift():
            type(""),
            base_power(0)
        {}

        natural_gift(
            const std::string& move_type,
            int move_power
        ): type(move_type), base_power(move_power)
        {}

        natural_gift(const natural_gift& other) = default;

#ifndef SWIG
        natural_gift(
            std::string&& move_type,
            int move_power
        ): type(std::move(move_type)), base_power(move_power)
        {}

        natural_gift(natural_gift&&) = default;
        natural_gift& operator=(const natural_gift&) = default;
        natural_gift& operator=(natural_gift&&) = default;
#endif

        bool operator==(const natural_gift& rhs) const
        {
            return (type == rhs.type) && (base_power == rhs.base_power);
        }

        bool operator!=(const natural_gift& rhs) const
        {
            return !operator==(rhs);
        }

        std::string type;
        int base_power;
    };

    PKMN_API natural_gift natural_gift_stats(
        const std::string& item_name,
        int generation
    );

}}
#endif /* PKMN_CALCULATIONS_MOVES_NATURAL_GIFT_HPP */

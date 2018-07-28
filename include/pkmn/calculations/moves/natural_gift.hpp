/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_MOVES_NATURAL_GIFT_HPP
#define PKMN_CALCULATIONS_MOVES_NATURAL_GIFT_HPP

#include <pkmn/config.hpp>

#include <pkmn/enums/item.hpp>
#include <pkmn/enums/type.hpp>

#include <string>

namespace pkmn { namespace calculations {

    /*!
     * @brief Natural Gift stats.
     *
     * The type and base power of Natural Gift is determined by the
     * berry held by the Pokémon when it uses the move.
     */
    struct natural_gift
    {
        natural_gift():
            type(pkmn::e_type::NONE),
            base_power(0)
        {}

        natural_gift(
            pkmn::e_type move_type,
            int move_power
        ): type(move_type),
           base_power(move_power)
        {}

        natural_gift(const natural_gift&) = default;
        natural_gift& operator=(const natural_gift&) = default;

#ifndef SWIG
        natural_gift(natural_gift&&) = default;
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

        pkmn::e_type type;
        int base_power;
    };

    /*!
     * @brief Calculate Natural Gift stats.
     *
     * Base powers can vary for the same item between generations.
     *
     * \param item The item held by the Pokémon
     * \param generation The generation of the game
     * \throws std::invalid_argument if the item is not a Berry
     * \throws std::out_of_range if the generation is not in the range [4-6]
     */
    PKMN_API natural_gift natural_gift_stats(
        pkmn::e_item item,
        int generation
    );

}}
#endif /* PKMN_CALCULATIONS_MOVES_NATURAL_GIFT_HPP */

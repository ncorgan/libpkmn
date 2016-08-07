/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_FORM_HPP
#define PKMN_CALCULATIONS_FORM_HPP

#include <pkmn/config.hpp>

#include <cstdint>
#include <string>

namespace pkmn { namespace calculations {

    /*!
     * @brief Returns a Generation II Unown's form based on IV's.
     *
     * \param IV_attack Attack IV (0-15)
     * \param IV_defense Defense IV (0-15)
     * \param IV_speed Speed IV (0-15)
     * \param IV_special Special IV (0-15)
     * \throws std::out_of_range if any IV is outside the range [0,15]
     * \returns Unown form, possible values: "A"-"Z"
     */
    PKMN_API std::string gen2_unown_form(
        int IV_attack,
        int IV_defense,
        int IV_speed,
        int IV_special
    );

    /*!
     * @brief Returns a Generation III Unown's form based on its personality.
     *
     * \param personality Unown's personality value
     * \returns Unown form, possible values: "A"-"Z","?","!"
     */
    PKMN_API std::string gen3_unown_form(
        uint32_t personality
    );

    /*!
     * @brief Returns whether or not a Wurmple with a given personality will
     *        evolve into a Silcoon.
     *
     * \param personality Wurmple's personality
     * \param before_gen5 Whether the Wurmple is in a Generation III-IV game
     * \returns Whether a Wurmple will evolve into a Silcoon
     */
    PKMN_CONSTEXPR_OR_INLINE bool wurmple_becomes_silcoon(
        uint32_t personality,
        bool before_gen5
    ) {
        return before_gen5 ? ((personality % 10) < 5)
                           : (((personality >> 16) % 10) < 5);
    }

}}
#endif /* PKMN_CALCULATIONS_FORM_HPP */

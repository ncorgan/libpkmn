/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_MOVES_HIDDEN_POWER_HPP
#define PKMN_CALCULATIONS_MOVES_HIDDEN_POWER_HPP

#include <pkmn/config.hpp>

#include <string>

namespace pkmn { namespace calculations {

    /*!
     * @brief Hidden Power stats.
     *
     * A Pokémon's IVs determine the type and base power of Hidden
     * Power when it uses it.
     */
    struct hidden_power
    {
        hidden_power():
            type(""),
            base_power(0)
        {}

        hidden_power(
            const std::string& move_type,
            int move_power
        ): type(move_type), base_power(move_power)
        {}

        hidden_power(const hidden_power&) = default;
        hidden_power& operator=(const hidden_power&) = default;

#ifndef SWIG
        hidden_power(
            std::string&& move_type,
            int move_power
        ): type(std::move(move_type)), base_power(move_power)
        {}

        hidden_power(hidden_power&&) = default;
        hidden_power& operator=(hidden_power&&) = default;
#endif

        bool operator==(const hidden_power& rhs) const
        {
            return (type == rhs.type) && (base_power == rhs.base_power);
        }

        bool operator!=(const hidden_power& rhs) const
        {
            return !operator==(rhs);
        }

        std::string type;
        int base_power;
    };

    /*!
     * @brief Calculate Hidden Power information in a Generation II game.
     *
     * \param IV_attack Attack IV (0-15)
     * \param IV_defense Defense IV (0-15)
     * \param IV_speed Speed IV (0-15)
     * \param IV_special Special IV (0-15)
     * \throws std::out_of_range if any parameter is [0-15]
     * \returns calculated Hidden Power info
     */
    PKMN_API hidden_power gen2_hidden_power(
        int IV_attack,
        int IV_defense,
        int IV_speed,
        int IV_special
    );

    /*!
     * @brief Calculate Hidden Power information in a Generation III+ game.
     *
     * \param IV_HP HP IV (0-31)
     * \param IV_attack Attack IV (0-31)
     * \param IV_defense Defense IV (0-31)
     * \param IV_speed Speed IV (0-31)
     * \param IV_spatk Special Attack IV (0-31)
     * \param IV_spdef Special Defense IV (0-31)
     * \throws std::out_of_range if any parameter is [0-31]
     * \returns calculated Hidden Power info
     */
    PKMN_API hidden_power modern_hidden_power(
        int IV_HP,
        int IV_attack,
        int IV_defense,
        int IV_speed,
        int IV_spatk,
        int IV_spdef
    );

}}
#endif /* PKMN_CALCULATIONS_MOVES_HIDDEN_POWER_HPP */

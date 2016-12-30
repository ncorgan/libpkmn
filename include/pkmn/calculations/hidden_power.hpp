/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_HIDDEN_POWER_HPP
#define PKMN_CALCULATIONS_HIDDEN_POWER_HPP

#include <pkmn/config.hpp>

#include <string>

namespace pkmn { namespace calculations {

    /*!
     * @brief Hidden Power stats.
     *
     * A Pokémon's IVs determine the type and base power of Hidden
     * Power when it uses it.
     */
    struct hidden_power {
        //! Default constructor.
        PKMN_INLINE hidden_power():
            type(""),
            base_power(0) {}

        //! Constructor with the type and base power.
        PKMN_INLINE hidden_power(
            const std::string &hidden_power_type,
            int hidden_power_base_power
        ): type(hidden_power_type),
           base_power(hidden_power_base_power) {}

#ifndef SWIG
        //! Move constructor with the type and base power.
        PKMN_INLINE hidden_power(
            std::string&& hidden_power_type,
            int hidden_power_base_power
        ): type(hidden_power_type),
           base_power(hidden_power_base_power) {}
#endif

        //! Equality check with another Hidden Power struct.
        PKMN_INLINE bool operator==(
            const hidden_power &rhs
        ) const {
            return (this->type == rhs.type) and
                   (this->base_power == rhs.base_power);
        }

        //! Inequality check with another Hidden Power struct.
        PKMN_INLINE bool operator!=(
            const hidden_power &rhs
        ) const {
            return (this->type != rhs.type) or
                   (this->base_power != rhs.base_power);
        }

        //! What type Hidden Power has when this Pokémon uses it.
        std::string type;

        //! Base Power
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
#endif /* PKMN_CALCULATIONS_HIDDEN_POWER_HPP */

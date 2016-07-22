/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_SHININESS_HPP
#define PKMN_CALCULATIONS_SHININESS_HPP

#include <pkmn/config.hpp>

#include <cstdint>

namespace pkmn { namespace calculations {

    /*!
     * @brief Calculates whether a Generation II Pokémon with the given
     *        IVs is shiny.
     *
     * \param IV_attack Attack IV (0-15)
     * \param IV_defense Defense IV (0-15)
     * \param IV_speed Speed IV (0-15)
     * \param IV_special Special IV (0-15)
     * \throws std::out_of_range if any parameter is not in the range [0-15]
     * \returns whether Pokémon is shiny
     */
    PKMN_API bool gen2_shiny(
        int IV_attack,
        int IV_defense,
        int IV_speed,
        int IV_special
    );

    /*!
     * @brief Calculates whether a Generation III+ Pokémon with the given
     *        personality and original Trainer ID is shiny.
     *
     * \param personality Pokémon's personality
     * \param trainer_id Pokémon's full original trainer ID
     */
    PKMN_API bool modern_shiny(
        uint32_t personality,
        uint32_t trainer_id
    );

}}
#endif /* PKMN_CALCULATIONS_SHININESS_HPP */

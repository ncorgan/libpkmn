/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_STATS_HPP
#define PKMN_CALCULATIONS_STATS_HPP

#include <pkmn/config.hpp>

#include <string>
#include <utility>

namespace pkmn { namespace calculations {

    /*!
     * @brief Calculates a Generation I-II Pokémon's stat based on the
     *        given information.
     *
     * \param stat Stat name (HP, Attack, Defense, Speed, Special, Special
     *             Attack, Special Defense)
     * \param level Pokémon's level (0-100)
     * \param base_stat The base stat value
     * \param EV The Pokémon's EV for this stat (0-65535)
     * \param IV The Pokémon's IV for this stat (0-15)
     * \throws std::runtime_error If the given stat string is invalid
     * \throws std::out_of_range If any numeric parameter is out of the given range
     * \returns Calculated stat
     */
    PKMN_API int get_gb_stat(
        const std::string& stat,
        int level,
        int base_stat,
        int EV,
        int IV
    );

    /*!
     * @brief Calculates the minimum and maximum values of the given stat for a
     *        Generation I-II Pokémon.
     *
     * \param stat Stat name (HP, Attack, Defense, Speed, Special, Special
     *             Attack, Special Defense)
     * \param level Pokémon's level (0-100)
     * \param base_stat The base stat value
     * \returns Calculated stats
     */
    inline std::pair<int, int> get_gb_stat_range(
        const std::string& stat,
        int level,
        int base_stat
    ) {
        return std::make_pair(
                   get_gb_stat(stat, level, base_stat, 0, 0),
                   get_gb_stat(stat, level, base_stat, 65535, 15)
               );
    }

    /*!
     * @brief Calculates a Generation I-II Pokémon's stat based on the
     *        given information.
     *
     * \param stat Stat name (HP, Attack, Defense, Speed, Special Attack,
     *             Special Defense)
     * \param level Pokémon's level (0-100)
     * \param nature_modifier The effect of the Pokémon's nature on the stat (0.9, 1.0, 1.1)
     * \param base_stat The base stat value
     * \param EV The Pokémon's EV for this stat (0-255)
     * \param IV The Pokémon's IV for this stat (0-31)
     * \throws std::runtime_error If the given stat string is invalid
     * \throws std::domain_error If the given nature modifier is not in (0.9, 1.0, 1.1)
     * \throws std::out_of_range If any numeric parameter is out of the given range
     * \returns Calculated stat
     */
    PKMN_API int get_modern_stat(
        const std::string& stat,
        int level,
        float nature_modifier,
        int base_stat,
        int EV,
        int IV
    );

    /*!
     * @brief Calculates the minimum and maximum values of the given stat for a
     *        Generation III+ Pokémon.
     *
     * \param stat Stat name (HP, Attack, Defense, Speed, Special Attack,
     *             Special Defense)
     * \param level Pokémon's level (0-100)
     * \param base_stat The base stat value
     * \returns Calculated stats
     */
    inline std::pair<int, int> get_modern_stat_range(
        const std::string& stat,
        int level,
        int base_stat
    ) {
        return std::make_pair(
                   get_modern_stat(stat, level, 0.9f, base_stat, 0, 0),
                   get_modern_stat(stat, level, 1.1f, base_stat, 255, 31)
               );
    }
}}
#endif /* PKMN_CALCULATIONS_STATS_HPP */

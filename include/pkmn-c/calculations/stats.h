/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_CALCULATIONS_STATS_H
#define PKMN_C_CALCULATIONS_STATS_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/types/int_pair.h>
#include <pkmn-c/types/stats.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Calculates a Generation I-II Pokémon's stat based on the
 *        given information.
 *
 * \param stat Stat
 * \param level Pokémon's level (0-100)
 * \param base_stat The base stat value
 * \param EV The Pokémon's EV for this stat (0-65535)
 * \param IV The Pokémon's IV for this stat (0-15)
 * \param stat_out Where to return the stat
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if stat_out is NULL
 * \returns ::PKMN_ERROR_RUNTIME_ERROR If the given stat string is invalid
 * \returns ::PKMN_ERROR_OUT_OF_RANGE If any numeric parameter is out of the given range
 */
PKMN_C_API pkmn_error_t pkmn_calculations_gb_stat(
    enum pkmn_stat stat,
    int level,
    int base_stat,
    int EV,
    int IV,
    int* stat_out
);

/*!
 * @brief Calculates the minimum and maximum values of the given stat for a
 *        Generation I-II Pokémon.
 *
 * \param stat Stat name (HP, Attack, Defense, Speed, Special, Special
 *             Attack, Special Defense)
 * \param level Pokémon's level (0-100)
 * \param base_stat The base stat value
 * \param stat_range_out Where to return the stat range
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if stat_rangeout is NULL
 */
PKMN_C_API pkmn_error_t pkmn_calculations_gb_stat_range(
    enum pkmn_stat stat,
    int level,
    int base_stat,
    struct pkmn_int_pair* stat_range_out
);

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
 * \param stat_out Where to return the stat
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if stat_out is NULL
 * \returns ::PKMN_ERROR_RUNTIME_ERROR If the given stat string is invalid
 * \returns ::PKMN_ERROR_DOMAIN_ERROR If the given nature modifier is not in (0.9, 1.0, 1.1)
 * \returns ::PKMN_ERROR_OUT_OF_RANGE If any numeric parameter is out of the given range
 */
PKMN_C_API pkmn_error_t pkmn_calculations_modern_stat(
    enum pkmn_stat stat,
    int level,
    float nature_modifier,
    int base_stat,
    int EV,
    int IV,
    int* stat_out
);

/*!
 * @brief Calculates the minimum and maximum values of the given stat for a
 *        Generation III+ Pokémon.
 *
 * \param stat Stat name (HP, Attack, Defense, Speed, Special Attack,
 *             Special Defense)
 * \param level Pokémon's level (0-100)
 * \param base_stat The base stat value
 * \param stat_range_out Where to return the stat range
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if stat_range_out is NULL
 */
PKMN_C_API pkmn_error_t pkmn_calculations_modern_stat_range(
    enum pkmn_stat stat,
    int level,
    int base_stat,
    struct pkmn_int_pair* stat_range_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_STATS_H */

/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_STATS_H
#define PKMN_C_TYPES_STATS_H

#define PKMN_NUM_STATS 7

typedef enum
{
    PKMN_STAT_HP,
    PKMN_STAT_ATTACK,
    PKMN_STAT_DEFENSE,
    PKMN_STAT_SPEED,
    PKMN_STAT_SPECIAL,
    PKMN_STAT_SPATK,
    PKMN_STAT_SPDEF
} pkmn_stat_t;

#define PKMN_NUM_CONTEST_STATS 7

typedef enum
{
    PKMN_CONTEST_STAT_COOL,
    PKMN_CONTEST_STAT_BEAUTY,
    PKMN_CONTEST_STAT_CUTE,
    PKMN_CONTEST_STAT_SMART,
    PKMN_CONTEST_STAT_TOUGH,
    PKMN_CONTEST_STAT_FEEL,
    PKMN_CONTEST_STAT_SHEEN
} pkmn_contest_stat_t;

#endif /* PKMN_C_TYPES_STATS_H */

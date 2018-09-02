/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "enum_maps.hpp"

namespace pkmn { namespace c {

    const condition_bimap_t& get_condition_bimap()
    {
        static const condition_bimap_t CONDITION_BIMAP =
        boost::assign::list_of<condition_bimap_t::relation>
            (pkmn::e_condition::NONE,       PKMN_CONDITION_NONE)
            (pkmn::e_condition::ASLEEP,     PKMN_CONDITION_ASLEEP)
            (pkmn::e_condition::POISON,     PKMN_CONDITION_POISON)
            (pkmn::e_condition::BURN,       PKMN_CONDITION_BURN)
            (pkmn::e_condition::FROZEN,     PKMN_CONDITION_FROZEN)
            (pkmn::e_condition::PARALYSIS,  PKMN_CONDITION_PARALYSIS)
            (pkmn::e_condition::BAD_POISON, PKMN_CONDITION_BAD_POISON)
        ;

        return CONDITION_BIMAP;
    }

    const contest_stat_bimap_t& get_contest_stat_bimap()
    {
        static const contest_stat_bimap_t CONTEST_STAT_BIMAP =
        boost::assign::list_of<contest_stat_bimap_t::relation>
            (pkmn::e_contest_stat::COOL,   PKMN_CONTEST_STAT_COOL)
            (pkmn::e_contest_stat::BEAUTY, PKMN_CONTEST_STAT_BEAUTY)
            (pkmn::e_contest_stat::CUTE,   PKMN_CONTEST_STAT_CUTE)
            (pkmn::e_contest_stat::SMART,  PKMN_CONTEST_STAT_SMART)
            (pkmn::e_contest_stat::TOUGH,  PKMN_CONTEST_STAT_TOUGH)
            (pkmn::e_contest_stat::FEEL,   PKMN_CONTEST_STAT_FEEL)
            (pkmn::e_contest_stat::SHEEN,  PKMN_CONTEST_STAT_SHEEN)
        ;

        return CONTEST_STAT_BIMAP;
    }

    const marking_bimap_t& get_gen3_marking_bimap()
    {
        static const marking_bimap_t GEN3_MARKING_BIMAP =
        boost::assign::list_of<marking_bimap_t::relation>
            (pkmn::e_marking::CIRCLE,   PKMN_MARKING_CIRCLE)
            (pkmn::e_marking::TRIANGLE, PKMN_MARKING_TRIANGLE)
            (pkmn::e_marking::SQUARE,   PKMN_MARKING_SQUARE)
            (pkmn::e_marking::HEART,    PKMN_MARKING_HEART)
        ;

        return GEN3_MARKING_BIMAP;
    }

    const marking_bimap_t& get_marking_bimap()
    {
        static const marking_bimap_t MARKING_BIMAP =
        boost::assign::list_of<marking_bimap_t::relation>
            (pkmn::e_marking::CIRCLE,   PKMN_MARKING_CIRCLE)
            (pkmn::e_marking::TRIANGLE, PKMN_MARKING_TRIANGLE)
            (pkmn::e_marking::SQUARE,   PKMN_MARKING_SQUARE)
            (pkmn::e_marking::HEART,    PKMN_MARKING_HEART)
            (pkmn::e_marking::STAR,     PKMN_MARKING_STAR)
            (pkmn::e_marking::DIAMOND,  PKMN_MARKING_DIAMOND)
        ;

        return MARKING_BIMAP;
    }

    const stat_bimap_t& get_stat_bimap()
    {
        static const stat_bimap_t STAT_BIMAP =
        boost::assign::list_of<stat_bimap_t::relation>
            (pkmn::e_stat::HP,              PKMN_STAT_HP)
            (pkmn::e_stat::ATTACK,          PKMN_STAT_ATTACK)
            (pkmn::e_stat::DEFENSE,         PKMN_STAT_DEFENSE)
            (pkmn::e_stat::SPEED,           PKMN_STAT_SPEED)
            (pkmn::e_stat::SPECIAL,         PKMN_STAT_SPECIAL)
            (pkmn::e_stat::SPECIAL_ATTACK,  PKMN_STAT_SPECIAL_ATTACK)
            (pkmn::e_stat::SPECIAL_DEFENSE, PKMN_STAT_SPECIAL_DEFENSE)
        ;

        return STAT_BIMAP;
    }
}}

/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "enum_maps.hpp"

namespace pkmn { namespace c {

    const stat_bimap_t& get_stat_bimap()
    {
        static const stat_bimap_t STAT_BIMAP =
        boost::assign::list_of<stat_bimap_t::relation>
            (pkmn::e_stat::HP,              PKMN_STAT_HP)
            (pkmn::e_stat::ATTACK,          PKMN_STAT_ATTACK)
            (pkmn::e_stat::DEFENSE,         PKMN_STAT_DEFENSE)
            (pkmn::e_stat::SPEED,           PKMN_STAT_SPEED)
            (pkmn::e_stat::SPECIAL,         PKMN_STAT_SPECIAL)
            (pkmn::e_stat::SPECIAL_ATTACK,  PKMN_STAT_SPATK)
            (pkmn::e_stat::SPECIAL_DEFENSE, PKMN_STAT_SPDEF)
        ;

        return STAT_BIMAP;
    }

    const contest_stat_bimap_t& get_contest_stat_bimap()
    {
        static const contest_stat_bimap_t CONTEST_STAT_BIMAP =
        boost::assign::list_of<contest_stat_bimap_t::relation>
            ("Cool",   PKMN_CONTEST_STAT_COOL)
            ("Beauty", PKMN_CONTEST_STAT_BEAUTY)
            ("Cute",   PKMN_CONTEST_STAT_CUTE)
            ("Smart",  PKMN_CONTEST_STAT_SMART)
            ("Tough",  PKMN_CONTEST_STAT_TOUGH)
            ("Feel",   PKMN_CONTEST_STAT_FEEL)
            ("Sheen",  PKMN_CONTEST_STAT_SHEEN)
        ;

        return CONTEST_STAT_BIMAP;
    }

    const condition_bimap_t& get_condition_bimap()
    {
        static const condition_bimap_t CONDITION_BIMAP =
        boost::assign::list_of<condition_bimap_t::relation>
            ("None",       PKMN_CONDITION_NONE)
            ("Asleep",     PKMN_CONDITION_ASLEEP)
            ("Poison",     PKMN_CONDITION_POISON)
            ("Burn",       PKMN_CONDITION_BURN)
            ("Frozen",     PKMN_CONDITION_FROZEN)
            ("Paralysis",  PKMN_CONDITION_PARALYSIS)
            ("Bad Poison", PKMN_CONDITION_BAD_POISON)
        ;

        return CONDITION_BIMAP;
    }

    const marking_bimap_t& get_gen3_marking_bimap()
    {
        static const marking_bimap_t GEN3_MARKING_BIMAP =
        boost::assign::list_of<marking_bimap_t::relation>
            ("Circle",   PKMN_MARKING_CIRCLE)
            ("Triangle", PKMN_MARKING_TRIANGLE)
            ("Square",   PKMN_MARKING_SQUARE)
            ("Heart",    PKMN_MARKING_HEART)
        ;

        return GEN3_MARKING_BIMAP;
    }

    const marking_bimap_t& get_marking_bimap()
    {
        static const marking_bimap_t MARKING_BIMAP =
        boost::assign::list_of<marking_bimap_t::relation>
            ("Circle",   PKMN_MARKING_CIRCLE)
            ("Triangle", PKMN_MARKING_TRIANGLE)
            ("Square",   PKMN_MARKING_SQUARE)
            ("Heart",    PKMN_MARKING_HEART)
            ("Star",     PKMN_MARKING_STAR)
            ("Diamond",  PKMN_MARKING_DIAMOND)
        ;

        return MARKING_BIMAP;
    }
}
}

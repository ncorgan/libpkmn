/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "enum_maps.hpp"

namespace pkmn { namespace c {

    const gender_bimap_t& get_gender_bimap()
    {
        static const gender_bimap_t GENDER_BIMAP =
        boost::assign::list_of<gender_bimap_t::relation>
            ("Male",       PKMN_GENDER_MALE)
            ("Female",     PKMN_GENDER_FEMALE)
            ("Genderless", PKMN_GENDER_GENDERLESS)
        ;

        return GENDER_BIMAP;
    }

    const stat_bimap_t& get_stat_bimap()
    {
        static const stat_bimap_t STAT_BIMAP =
        boost::assign::list_of<stat_bimap_t::relation>
            ("HP",              PKMN_STAT_HP)
            ("Attack",          PKMN_STAT_ATTACK)
            ("Defense",         PKMN_STAT_DEFENSE)
            ("Speed",           PKMN_STAT_SPEED)
            ("Special",         PKMN_STAT_SPECIAL)
            ("Special Attack",  PKMN_STAT_SPATK)
            ("Special Defense", PKMN_STAT_SPDEF)
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

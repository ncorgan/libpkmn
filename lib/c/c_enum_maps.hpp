/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_C_ENUM_MAPS_HPP
#define PKMN_C_ENUM_MAPS_HPP

#include <pkmn-c/types/gender.h>
#include <pkmn-c/types/stats.h>

#include <boost/assign.hpp>
#include <boost/bimap.hpp>

#include <string>

namespace pkmn { namespace c {

    typedef boost::bimap<std::string, pkmn_gender_t> gender_bimap_t;
    static const gender_bimap_t GENDER_BIMAP =
    boost::assign::list_of<gender_bimap_t::relation>
        ("Male",       PKMN_MALE)
        ("Female",     PKMN_FEMALE)
        ("Genderless", PKMN_GENDERLESS)
    ;

    typedef boost::bimap<std::string, pkmn_stat_t> stat_bimap_t;
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

}
}

#endif /* PKMN_C_ENUM_MAPS_HPP */

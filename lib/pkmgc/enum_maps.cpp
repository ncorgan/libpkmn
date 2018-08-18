/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pkmgc/enum_maps.hpp"

#include <boost/assign.hpp>
#include <boost/bimap.hpp>

namespace pkmgc
{

const condition_bimap_t& get_condition_bimap()
{
    static const condition_bimap_t CONDITION_BIMAP =
    boost::assign::list_of<condition_bimap_t::relation>
        ("None",       LibPkmGC::NoStatus)
        ("Asleep",     LibPkmGC::Asleep)
        ("Poison",     LibPkmGC::Poisoned)
        ("Burn",       LibPkmGC::Burnt)
        ("Frozen",     LibPkmGC::Frozen)
        ("Paralysis",  LibPkmGC::Paralyzed)
        ("Bad Poison", LibPkmGC::BadlyPoisoned)
    ;

    return CONDITION_BIMAP;
}

const contest_stat_bimap_t& get_contest_stat_bimap()
{
    static const contest_stat_bimap_t CONTEST_STAT_BIMAP =
    boost::assign::list_of<contest_stat_bimap_t::relation>
        (pkmn::e_contest_stat::COOL,   pkmgc::e_contest_stat::COOL)
        (pkmn::e_contest_stat::BEAUTY, pkmgc::e_contest_stat::BEAUTY)
        (pkmn::e_contest_stat::CUTE,   pkmgc::e_contest_stat::CUTE)
        (pkmn::e_contest_stat::SMART,  pkmgc::e_contest_stat::SMART)
        (pkmn::e_contest_stat::TOUGH,  pkmgc::e_contest_stat::TOUGH)
    ;

    return CONTEST_STAT_BIMAP;
}

const gender_bimap_t& get_gender_bimap()
{
    static const gender_bimap_t GENDER_BIMAP =
    boost::assign::list_of<gender_bimap_t::relation>
        (pkmn::e_gender::MALE,       LibPkmGC::Male)
        (pkmn::e_gender::FEMALE,     LibPkmGC::Female)
        (pkmn::e_gender::GENDERLESS, LibPkmGC::Genderless)
    ;

    return GENDER_BIMAP;
}

const language_bimap_t& get_language_bimap()
{
    static const language_bimap_t LANGUAGE_BIMAP =
    boost::assign::list_of<language_bimap_t::relation>
        (pkmn::e_language::JAPANESE, LibPkmGC::Japanese)
        (pkmn::e_language::ENGLISH,  LibPkmGC::English)
        (pkmn::e_language::GERMAN,   LibPkmGC::German)
        (pkmn::e_language::FRENCH,   LibPkmGC::French)
        (pkmn::e_language::ITALIAN,  LibPkmGC::Italian)
        (pkmn::e_language::SPANISH,  LibPkmGC::Spanish)
    ;

    return LANGUAGE_BIMAP;
}

const ribbon_bimap_t& get_ribbon_bimap()
{
    static const ribbon_bimap_t RIBBON_BIMAP =
    boost::assign::list_of<ribbon_bimap_t::relation>
        ("Champion", pkmgc::e_ribbon::CHAMPION)
        ("Winning",  pkmgc::e_ribbon::WINNING)
        ("Victory",  pkmgc::e_ribbon::VICTORY)
        ("Artist",   pkmgc::e_ribbon::ARTIST)
        ("Effort",   pkmgc::e_ribbon::EFFORT)
        ("Marine",   pkmgc::e_ribbon::MARINE)
        ("Land",     pkmgc::e_ribbon::LAND)
        ("Sky",      pkmgc::e_ribbon::SKY)
        ("Country",  pkmgc::e_ribbon::COUNTRY)
        ("National", pkmgc::e_ribbon::NATIONAL)
        ("Earth",    pkmgc::e_ribbon::EARTH)
        ("World",    pkmgc::e_ribbon::WORLD)
    ;

    return RIBBON_BIMAP;
}

const stat_bimap_t& get_stat_bimap()
{
    static const stat_bimap_t STAT_BIMAP =
    boost::assign::list_of<stat_bimap_t::relation>
        (pkmn::e_stat::HP,              pkmgc::e_stat::HP)
        (pkmn::e_stat::ATTACK,          pkmgc::e_stat::ATTACK)
        (pkmn::e_stat::DEFENSE,         pkmgc::e_stat::DEFENSE)
        (pkmn::e_stat::SPECIAL_ATTACK,  pkmgc::e_stat::SPECIAL_ATTACK)
        (pkmn::e_stat::SPECIAL_DEFENSE, pkmgc::e_stat::SPECIAL_DEFENSE)
        (pkmn::e_stat::SPEED,           pkmgc::e_stat::SPEED)
    ;

    return STAT_BIMAP;
}

}

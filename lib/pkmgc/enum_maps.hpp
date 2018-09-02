/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMN_PKMGC_ENUM_MAPS_HPP
#define INCLUDED_PKMN_PKMGC_ENUM_MAPS_HPP

#include "pkmgc/includes.hpp"

#include <pkmn/enums/condition.hpp>
#include <pkmn/enums/contest_stat.hpp>
#include <pkmn/enums/gender.hpp>
#include <pkmn/enums/language.hpp>
#include <pkmn/enums/stat.hpp>

#include <boost/bimap.hpp>

#include <string>

namespace pkmgc
{

    /*
     * LibPkmGC stores some values in arrays, with little indication as to what each
     * index actually corresponds to, so these enums make things easier.
     */
    enum class e_stat
    {
        HP = 0,
        ATTACK,
        DEFENSE,
        SPECIAL_ATTACK,
        SPECIAL_DEFENSE,
        SPEED
    };

    enum class e_contest_stat
    {
        COOL = 0,
        BEAUTY,
        CUTE,
        SMART,
        TOUGH
    };

    enum class e_ribbon
    {
        CHAMPION = 0,
        WINNING,
        VICTORY,
        ARTIST,
        EFFORT,
        MARINE,
        LAND,
        SKY,
        COUNTRY,
        NATIONAL,
        EARTH,
        WORLD
    };

    using condition_bimap_t = boost::bimap<pkmn::e_condition, LibPkmGC::PokemonStatus>;
    using contest_stat_bimap_t = boost::bimap<pkmn::e_contest_stat, pkmgc::e_contest_stat>;
    using gender_bimap_t = boost::bimap<pkmn::e_gender, LibPkmGC::Gender>;
    using language_bimap_t = boost::bimap<pkmn::e_language, LibPkmGC::LanguageIndex>;
    using ribbon_bimap_t = boost::bimap<std::string, pkmgc::e_ribbon>;
    using stat_bimap_t = boost::bimap<pkmn::e_stat, pkmgc::e_stat>;

    const condition_bimap_t& get_condition_bimap();
    const contest_stat_bimap_t& get_contest_stat_bimap();
    const gender_bimap_t& get_gender_bimap();
    const language_bimap_t& get_language_bimap();
    const ribbon_bimap_t& get_ribbon_bimap();
    const stat_bimap_t& get_stat_bimap();
}

#endif /* INCLUDED_PKMN_PKMGC_ENUM_MAPS_HPP */

/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_C_ENUM_MAPS_HPP
#define PKMN_C_ENUM_MAPS_HPP

#include <pkmn-c/game_save.h>

#include <pkmn-c/enums/condition.h>
#include <pkmn-c/enums/game.h>
#include <pkmn-c/enums/gender.h>
#include <pkmn-c/enums/language.h>
#include <pkmn-c/enums/marking.h>
#include <pkmn-c/enums/stat.h>

#include <pkmn/game_save.hpp>

#include <pkmn/enums/game.hpp>
#include <pkmn/enums/gender.hpp>
#include <pkmn/enums/language.hpp>
#include <pkmn/enums/stat.hpp>

#include <boost/assign.hpp>
#include <boost/bimap.hpp>

#include <string>

namespace pkmn { namespace c {

    typedef boost::bimap<pkmn::e_stat, enum pkmn_stat> stat_bimap_t;

    typedef boost::bimap<std::string, enum pkmn_contest_stat> contest_stat_bimap_t;
    typedef boost::bimap<std::string, enum pkmn_condition> condition_bimap_t;
    typedef boost::bimap<std::string, enum pkmn_marking> marking_bimap_t;

    const stat_bimap_t& get_stat_bimap();
    const contest_stat_bimap_t& get_contest_stat_bimap();
    const condition_bimap_t& get_condition_bimap();
    const marking_bimap_t& get_gen3_marking_bimap();
    const marking_bimap_t& get_marking_bimap();
}
}

#endif /* PKMN_C_ENUM_MAPS_HPP */

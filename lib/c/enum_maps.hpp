/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_C_ENUM_MAPS_HPP
#define PKMN_C_ENUM_MAPS_HPP

#include <pkmn-c/types/condition.h>
#include <pkmn-c/types/gender.h>
#include <pkmn-c/types/marking.h>
#include <pkmn-c/types/stats.h>

#include <boost/assign.hpp>
#include <boost/bimap.hpp>

#include <string>

namespace pkmn { namespace c {

    typedef boost::bimap<std::string, enum pkmn_gender> gender_bimap_t;
    typedef boost::bimap<std::string, enum pkmn_stat> stat_bimap_t;
    typedef boost::bimap<std::string, enum pkmn_contest_stat> contest_stat_bimap_t;
    typedef boost::bimap<std::string, enum pkmn_condition> condition_bimap_t;
    typedef boost::bimap<std::string, enum pkmn_marking> marking_bimap_t;

    const gender_bimap_t& get_gender_bimap();
    const stat_bimap_t& get_stat_bimap();
    const contest_stat_bimap_t& get_contest_stat_bimap();
    const condition_bimap_t& get_condition_bimap();
    const marking_bimap_t& get_gen3_marking_bimap();
    const marking_bimap_t& get_marking_bimap();
}
}

#endif /* PKMN_C_ENUM_MAPS_HPP */

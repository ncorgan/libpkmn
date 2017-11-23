/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMN_PKSAV_ENUM_MAPS_HPP
#define INCLUDED_PKMN_PKSAV_ENUM_MAPS_HPP

#include <pksav/gen1/pokemon.h>
#include <pksav/gen6/ribbons.h>
#include <pksav/common/condition.h>

#include <boost/assign.hpp>
#include <boost/bimap.hpp>

#include <string>

namespace pksav {

// Status conditions

typedef boost::bimap<std::string, pksav_gb_condition_t> gb_condition_bimap_t;
static const gb_condition_bimap_t GB_CONDITION_BIMAP =
boost::assign::list_of<gb_condition_bimap_t::relation>
    ("None",      PKSAV_GB_CONDITION_NONE)
    ("Asleep",    PKSAV_GB_CONDITION_ASLEEP)
    ("Poison",    PKSAV_GB_CONDITION_POISON)
    ("Burn",      PKSAV_GB_CONDITION_BURN)
    ("Frozen",    PKSAV_GB_CONDITION_FROZEN)
    ("Paralysis", PKSAV_GB_CONDITION_PARALYZ)
;

typedef boost::bimap<std::string, pksav_condition_mask_t> condition_mask_bimap_t;
static const condition_mask_bimap_t CONDITION_MASK_BIMAP =
boost::assign::list_of<condition_mask_bimap_t::relation>
    ("None",       PKSAV_CONDITION_NONE)
    ("Asleep",     PKSAV_CONDITION_SLEEP_MASK)
    ("Poison",     PKSAV_CONDITION_POISON)
    ("Burn",       PKSAV_CONDITION_BURN)
    ("Frozen",     PKSAV_CONDITION_FROZEN)
    ("Paralysis",  PKSAV_CONDITION_PARALYZ)
    ("Bad Poison", PKSAV_CONDITION_BAD_POISON)
;

// Generation VI ribbons

typedef boost::bimap<std::string, pksav_gen6_ribbon1_mask_t> gen6_ribbon1_bimap_t;
static const gen6_ribbon1_bimap_t GEN6_RIBBONS1 = boost::assign::list_of<gen6_ribbon1_bimap_t::relation>
    ("Kalos Champion",   PKSAV_GEN6_KALOS_CHAMPION_RIBBON_MASK)
    ("Champion",         PKSAV_GEN6_CHAMPION_RIBBON_MASK)
    ("Sinnoh Champ",     PKSAV_GEN6_SINNOH_CHAMP_RIBBON_MASK)
    ("Best Friends",     PKSAV_GEN6_BEST_FRIENDS_RIBBON_MASK)
    ("Training Ribbon",  PKSAV_GEN6_TRAINING_RIBBON_MASK)
    ("Skillful Battler", PKSAV_GEN6_SKILLFUL_BATTLER_RIBBON_MASK)
    ("Expert Battler",   PKSAV_GEN6_EXPERT_BATTLER_RIBBON_MASK)
    ("Effort",           PKSAV_GEN6_EFFORT_RIBBON_MASK)
;

typedef boost::bimap<std::string, pksav_gen6_ribbon2_mask_t> gen6_ribbon2_bimap_t;
static const gen6_ribbon2_bimap_t GEN6_RIBBONS2 = boost::assign::list_of<gen6_ribbon2_bimap_t::relation>
    ("Alert",    PKSAV_GEN6_ALERT_RIBBON_MASK)
    ("Shock",    PKSAV_GEN6_SHOCK_RIBBON_MASK)
    ("Downcast", PKSAV_GEN6_DOWNCAST_RIBBON_MASK)
    ("Careless", PKSAV_GEN6_CARELESS_RIBBON_MASK)
    ("Relax",    PKSAV_GEN6_RELAX_RIBBON_MASK)
    ("Snooze",   PKSAV_GEN6_SNOOZE_RIBBON_MASK)
    ("Smile",    PKSAV_GEN6_SMILE_RIBBON_MASK)
    ("Gorgeous", PKSAV_GEN6_GORGEOUS_RIBBON_MASK)
;

typedef boost::bimap<std::string, pksav_gen6_ribbon3_mask_t> gen6_ribbon3_bimap_t;
static const gen6_ribbon3_bimap_t GEN6_RIBBONS3 = boost::assign::list_of<gen6_ribbon3_bimap_t::relation>
    ("Royal",          PKSAV_GEN6_ROYAL_RIBBON_MASK)
    ("Gorgeous Royal", PKSAV_GEN6_GORGEOUS_ROYAL_RIBBON_MASK)
    ("Artist",         PKSAV_GEN6_ARTIST_RIBBON_MASK)
    ("Footprint",      PKSAV_GEN6_FOOTPRINT_RIBBON_MASK)
    ("Record",         PKSAV_GEN6_RECORD_RIBBON_MASK)
    ("Legend",         PKSAV_GEN6_LEGEND_RIBBON_MASK)
    ("Country",        PKSAV_GEN6_COUNTRY_RIBBON_MASK)
    ("National",       PKSAV_GEN6_NATIONAL_RIBBON_MASK)
;

typedef boost::bimap<std::string, pksav_gen6_ribbon4_mask_t> gen6_ribbon4_bimap_t;
static const gen6_ribbon4_bimap_t GEN6_RIBBONS4 = boost::assign::list_of<gen6_ribbon4_bimap_t::relation>
    ("Earth",    PKSAV_GEN6_EARTH_RIBBON_MASK)
    ("World",    PKSAV_GEN6_WORLD_RIBBON_MASK)
    ("Classic",  PKSAV_GEN6_CLASSIC_RIBBON_MASK)
    ("Premier",  PKSAV_GEN6_PREMIER_RIBBON_MASK)
    ("Event",    PKSAV_GEN6_EVENT_RIBBON_MASK)
    ("Birthday", PKSAV_GEN6_BIRTHDAY_RIBBON_MASK)
    ("Special",  PKSAV_GEN6_SPECIAL_RIBBON_MASK)
    ("Souvenir", PKSAV_GEN6_SOUVENIR_RIBBON_MASK)
;

typedef boost::bimap<std::string, pksav_gen6_ribbon5_mask_t> gen6_ribbon5_bimap_t;
static const gen6_ribbon5_bimap_t GEN6_RIBBONS5 = boost::assign::list_of<gen6_ribbon5_bimap_t::relation>
    ("Wishing",           PKSAV_GEN6_WISHING_RIBBON_MASK)
    ("Battle Champion",   PKSAV_GEN6_BATTLE_CHAMPION_RIBBON_MASK)
    ("Regional Champion", PKSAV_GEN6_REGIONAL_CHAMPION_RIBBON_MASK)
    ("National Champion", PKSAV_GEN6_NATIONAL_CHAMPION_RIBBON_MASK)
    ("World Champion",    PKSAV_GEN6_WORLD_CHAMPION_RIBBON_MASK)
    ("Hoenn Champion",    PKSAV_GEN6_HOENN_CHAMPION_RIBBON_MASK)
;

typedef boost::bimap<std::string, pksav_gen6_ribbon6_mask_t> gen6_ribbon6_bimap_t;
static const gen6_ribbon6_bimap_t GEN6_RIBBONS6 = boost::assign::list_of<gen6_ribbon6_bimap_t::relation>
    ("Contest Star",      PKSAV_GEN6_CONTEST_STAR_RIBBON_MASK)
    ("Coolness Master",   PKSAV_GEN6_COOLNESS_MASTER_RIBBON_MASK)
    ("Beauty Master",     PKSAV_GEN6_BEAUTY_MASTER_RIBBON_MASK)
    ("Cuteness Master",   PKSAV_GEN6_CUTENESS_MASTER_RIBBON_MASK)
    ("Cleverness Master", PKSAV_GEN6_CLEVERNESS_MASTER_RIBBON_MASK)
    ("Toughness Master",  PKSAV_GEN6_TOUGHNESS_MASTER_RIBBON_MASK)
;

// Misc

typedef boost::bimap<std::string, pksav_gen1_type_t> gen1_type_bimap_t;
static const gen1_type_bimap_t GEN1_TYPE_BIMAP =
boost::assign::list_of<gen1_type_bimap_t::relation>
    ("Normal",   PKSAV_GEN1_TYPE_NORMAL)
    ("Fighting", PKSAV_GEN1_TYPE_FIGHTING)
    ("Flying",   PKSAV_GEN1_TYPE_FLYING)
    ("Poison",   PKSAV_GEN1_TYPE_POISON)
    ("Ground",   PKSAV_GEN1_TYPE_GROUND)
    ("Rock",     PKSAV_GEN1_TYPE_ROCK)
    ("Bug",      PKSAV_GEN1_TYPE_BUG)
    ("Ghost",    PKSAV_GEN1_TYPE_GHOST)
    ("Fire",     PKSAV_GEN1_TYPE_FIRE)
    ("Water",    PKSAV_GEN1_TYPE_WATER)
    ("Grass",    PKSAV_GEN1_TYPE_GRASS)
    ("Electric", PKSAV_GEN1_TYPE_ELECTRIC)
    ("Psychic",  PKSAV_GEN1_TYPE_PSYCHIC)
    ("Ice",      PKSAV_GEN1_TYPE_ICE)
    ("Dragon",   PKSAV_GEN1_TYPE_DRAGON)
;

}

#endif /* INCLUDED_PKMN_PKSAV_ENUM_MAPS_HPP */

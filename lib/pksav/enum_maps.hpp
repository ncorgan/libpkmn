/*
 * Copyright (c) 2017,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMN_PKSAV_ENUM_MAPS_HPP
#define INCLUDED_PKMN_PKSAV_ENUM_MAPS_HPP

#include <pksav/gen1/pokemon.h>
#include <pksav/gba/box_wallpaper.h>
#include <pksav/common/condition.h>

#include <boost/assign.hpp>
#include <boost/bimap.hpp>

#include <string>

namespace pksav {

// Status conditions

typedef boost::bimap<std::string, enum pksav_gb_condition> gb_condition_bimap_t;
static const gb_condition_bimap_t GB_CONDITION_BIMAP =
boost::assign::list_of<gb_condition_bimap_t::relation>
    ("None",      PKSAV_GB_CONDITION_NONE)
    ("Asleep",    PKSAV_GB_CONDITION_ASLEEP)
    ("Poison",    PKSAV_GB_CONDITION_POISON)
    ("Burn",      PKSAV_GB_CONDITION_BURN)
    ("Frozen",    PKSAV_GB_CONDITION_FROZEN)
    ("Paralysis", PKSAV_GB_CONDITION_PARALYZ)
;

typedef boost::bimap<std::string, enum pksav_condition_mask> condition_mask_bimap_t;
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

// Box wallpapers

typedef boost::bimap<std::string, enum pksav_gba_box_wallpaper> gba_box_wallpaper_bimap_t;
static const gba_box_wallpaper_bimap_t GBA_BOX_WALLPAPER_BIMAP =
boost::assign::list_of<gba_box_wallpaper_bimap_t::relation>
    ("Forest",  PKSAV_GBA_BOX_WALLPAPER_FOREST)
    ("City",     PKSAV_GBA_BOX_WALLPAPER_CITY)
    ("Desert",   PKSAV_GBA_BOX_WALLPAPER_DESERT)
    ("Savanna",  PKSAV_GBA_BOX_WALLPAPER_SAVANNA)
    ("Crag",     PKSAV_GBA_BOX_WALLPAPER_CRAG)
    ("Volcano",  PKSAV_GBA_BOX_WALLPAPER_VOLCANO)
    ("Snow",     PKSAV_GBA_BOX_WALLPAPER_SNOW)
    ("Cave",     PKSAV_GBA_BOX_WALLPAPER_CAVE)
    ("Beach",    PKSAV_GBA_BOX_WALLPAPER_BEACH)
    ("Seafloor", PKSAV_GBA_BOX_WALLPAPER_SEAFLOOR)
    ("River",    PKSAV_GBA_BOX_WALLPAPER_RIVER)
    ("Sky",      PKSAV_GBA_BOX_WALLPAPER_SKY)
;

typedef boost::bimap<std::string, enum pksav_gba_rse_box_wallpaper> gba_rse_box_wallpaper_bimap_t;
static const gba_rse_box_wallpaper_bimap_t GBA_RSE_BOX_WALLPAPER_BIMAP =
boost::assign::list_of<gba_rse_box_wallpaper_bimap_t::relation>
    ("Polka-Dot",  PKSAV_GBA_RSE_BOX_WALLPAPER_POLKADOT)
    ("PokéCenter", PKSAV_GBA_RSE_BOX_WALLPAPER_POKECENTER)
    ("Machine",    PKSAV_GBA_RSE_BOX_WALLPAPER_MACHINE)
    ("Plain",      PKSAV_GBA_RSE_BOX_WALLPAPER_PLAIN)
;

typedef boost::bimap<std::string, enum pksav_gba_frlg_box_wallpaper> gba_frlg_box_wallpaper_bimap_t;
static const gba_frlg_box_wallpaper_bimap_t GBA_FRLG_BOX_WALLPAPER_BIMAP =
boost::assign::list_of<gba_frlg_box_wallpaper_bimap_t::relation>
    ("Stars",      PKSAV_GBA_FRLG_BOX_WALLPAPER_STARS)
    ("PokéCenter", PKSAV_GBA_FRLG_BOX_WALLPAPER_POKECENTER)
    ("Tiles",      PKSAV_GBA_FRLG_BOX_WALLPAPER_TILES)
    ("Simple",     PKSAV_GBA_FRLG_BOX_WALLPAPER_SIMPLE)
;

// Misc

typedef boost::bimap<std::string, enum pksav_gen1_type> gen1_type_bimap_t;
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

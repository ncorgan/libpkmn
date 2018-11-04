/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pksav/enum_maps.hpp"

#include <boost/bimap.hpp>

namespace pksav
{

// Box wallpapers

const gba_box_wallpaper_bimap_t& get_gba_box_wallpaper_bimap()
{
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

    return GBA_BOX_WALLPAPER_BIMAP;
}

const gba_rse_box_wallpaper_bimap_t& get_gba_rse_box_wallpaper_bimap()
{
    static const gba_rse_box_wallpaper_bimap_t GBA_RSE_BOX_WALLPAPER_BIMAP =
    boost::assign::list_of<gba_rse_box_wallpaper_bimap_t::relation>
        ("Polka-Dot",  PKSAV_GBA_RSE_BOX_WALLPAPER_POLKADOT)
        ("PokéCenter", PKSAV_GBA_RSE_BOX_WALLPAPER_POKECENTER)
        ("Machine",    PKSAV_GBA_RSE_BOX_WALLPAPER_MACHINE)
        ("Plain",      PKSAV_GBA_RSE_BOX_WALLPAPER_PLAIN)
    ;

    return GBA_RSE_BOX_WALLPAPER_BIMAP;
}

const gba_frlg_box_wallpaper_bimap_t& get_gba_frlg_box_wallpaper_bimap()
{
    static const gba_frlg_box_wallpaper_bimap_t GBA_FRLG_BOX_WALLPAPER_BIMAP =
    boost::assign::list_of<gba_frlg_box_wallpaper_bimap_t::relation>
        ("Stars",      PKSAV_GBA_FRLG_BOX_WALLPAPER_STARS)
        ("PokéCenter", PKSAV_GBA_FRLG_BOX_WALLPAPER_POKECENTER)
        ("Tiles",      PKSAV_GBA_FRLG_BOX_WALLPAPER_TILES)
        ("Simple",     PKSAV_GBA_FRLG_BOX_WALLPAPER_SIMPLE)
    ;

    return GBA_FRLG_BOX_WALLPAPER_BIMAP;
}

// Ribbons

const gba_ribbon_mask_bimap_t& get_gba_ribbon_mask_bimap()
{
    static const gba_ribbon_mask_bimap_t GBA_RIBBON_MASK_BIMAP =
    boost::assign::list_of<gba_ribbon_mask_bimap_t::relation>
        ("Champion", PKSAV_GBA_CHAMPION_RIBBON_MASK)
        ("Winning",  PKSAV_GBA_WINNING_RIBBON_MASK)
        ("Victory",  PKSAV_GBA_VICTORY_RIBBON_MASK)
        ("Artist",   PKSAV_GBA_ARTIST_RIBBON_MASK)
        ("Effort",   PKSAV_GBA_EFFORT_RIBBON_MASK)
        ("Marine",   PKSAV_GBA_MARINE_RIBBON_MASK)
        ("Land",     PKSAV_GBA_LAND_RIBBON_MASK)
        ("Sky",      PKSAV_GBA_SKY_RIBBON_MASK)
        ("Country",  PKSAV_GBA_COUNTRY_RIBBON_MASK)
        ("National", PKSAV_GBA_NATIONAL_RIBBON_MASK)
        ("Earth",    PKSAV_GBA_EARTH_RIBBON_MASK)
        ("World",    PKSAV_GBA_WORLD_RIBBON_MASK)
    ;

    return GBA_RIBBON_MASK_BIMAP;
}

const gba_contest_ribbon_level_bimap_t& get_gba_contest_ribbon_level_bimap()
{
    static const gba_contest_ribbon_level_bimap_t& GBA_CONTEST_RIBBON_LEVEL_BIMAP =
    boost::assign::list_of<gba_contest_ribbon_level_bimap_t::relation>
        ("",       PKSAV_GBA_CONTEST_RIBBON_NONE)
        ("Normal", PKSAV_GBA_CONTEST_RIBBON_NORMAL)
        ("Super",  PKSAV_GBA_CONTEST_RIBBON_SUPER)
        ("Hyper",  PKSAV_GBA_CONTEST_RIBBON_HYPER)
        ("Master", PKSAV_GBA_CONTEST_RIBBON_MASTER)
    ;

    return GBA_CONTEST_RIBBON_LEVEL_BIMAP;
}

const gba_contest_ribbon_mask_bimap_t& get_gba_contest_ribbon_mask_bimap()
{
    static const gba_contest_ribbon_mask_bimap_t& GBA_CONTEST_RIBBON_MASK_BIMAP =
    boost::assign::list_of<gba_contest_ribbon_mask_bimap_t::relation>
        ("Cool",   PKSAV_GBA_COOL_RIBBONS_MASK)
        ("Beauty", PKSAV_GBA_BEAUTY_RIBBONS_MASK)
        ("Cute",   PKSAV_GBA_CUTE_RIBBONS_MASK)
        ("Smart",  PKSAV_GBA_SMART_RIBBONS_MASK)
        ("Tough",  PKSAV_GBA_TOUGH_RIBBONS_MASK)
    ;

    return GBA_CONTEST_RIBBON_MASK_BIMAP;
}

const gba_contest_ribbon_offset_bimap_t& get_gba_contest_ribbon_offset_bimap()
{
    static const gba_contest_ribbon_offset_bimap_t& GBA_CONTEST_RIBBON_OFFSET_BIMAP =
    boost::assign::list_of<gba_contest_ribbon_offset_bimap_t::relation>
        ("Cool",   PKSAV_GBA_COOL_RIBBONS_OFFSET)
        ("Beauty", PKSAV_GBA_BEAUTY_RIBBONS_OFFSET)
        ("Cute",   PKSAV_GBA_CUTE_RIBBONS_OFFSET)
        ("Smart",  PKSAV_GBA_SMART_RIBBONS_OFFSET)
        ("Tough",  PKSAV_GBA_TOUGH_RIBBONS_OFFSET)
    ;

    return GBA_CONTEST_RIBBON_OFFSET_BIMAP;
}

// Misc

const gen1_type_bimap_t& get_gen1_type_bimap()
{
    static const gen1_type_bimap_t GEN1_TYPE_BIMAP =
    boost::assign::list_of<gen1_type_bimap_t::relation>
        (pkmn::e_type::NORMAL,   PKSAV_GEN1_TYPE_NORMAL)
        (pkmn::e_type::FIGHTING, PKSAV_GEN1_TYPE_FIGHTING)
        (pkmn::e_type::FLYING,   PKSAV_GEN1_TYPE_FLYING)
        (pkmn::e_type::POISON,   PKSAV_GEN1_TYPE_POISON)
        (pkmn::e_type::GROUND,   PKSAV_GEN1_TYPE_GROUND)
        (pkmn::e_type::ROCK,     PKSAV_GEN1_TYPE_ROCK)
        (pkmn::e_type::BUG,      PKSAV_GEN1_TYPE_BUG)
        (pkmn::e_type::GHOST,    PKSAV_GEN1_TYPE_GHOST)
        (pkmn::e_type::FIRE,     PKSAV_GEN1_TYPE_FIRE)
        (pkmn::e_type::WATER,    PKSAV_GEN1_TYPE_WATER)
        (pkmn::e_type::GRASS,    PKSAV_GEN1_TYPE_GRASS)
        (pkmn::e_type::ELECTRIC, PKSAV_GEN1_TYPE_ELECTRIC)
        (pkmn::e_type::PSYCHIC,  PKSAV_GEN1_TYPE_PSYCHIC)
        (pkmn::e_type::ICE,      PKSAV_GEN1_TYPE_ICE)
        (pkmn::e_type::DRAGON,   PKSAV_GEN1_TYPE_DRAGON)
    ;

    return GEN1_TYPE_BIMAP;
}

const gen2_mom_money_policy_bimap_t& get_gen2_mom_money_policy_bimap()
{
    static const gen2_mom_money_policy_bimap_t GEN2_MOM_MONEY_POLICY_BIMAP =
    boost::assign::list_of<gen2_mom_money_policy_bimap_t::relation>
        ("Not saving money",  PKSAV_GEN2_MOM_MONEY_POLICY_NOT_SAVING)
        ("Default",           PKSAV_GEN2_MOM_MONEY_POLICY_SAVING_SOME_MONEY)
        ("Saving half money", PKSAV_GEN2_MOM_MONEY_POLICY_SAVING_HALF_MONEY)
        ("Saving all money",  PKSAV_GEN2_MOM_MONEY_POLICY_SAVING_ALL_MONEY)
    ;

    return GEN2_MOM_MONEY_POLICY_BIMAP;
}

const gba_language_bimap_t& get_gba_language_bimap()
{
    static const gba_language_bimap_t GBA_LANGUAGE_BIMAP =
    boost::assign::list_of<gba_language_bimap_t::relation>
        (pkmn::e_language::JAPANESE, PKSAV_GBA_LANGUAGE_JAPANESE)
        (pkmn::e_language::ENGLISH,  PKSAV_GBA_LANGUAGE_ENGLISH)
        (pkmn::e_language::FRENCH,   PKSAV_GBA_LANGUAGE_FRENCH)
        (pkmn::e_language::ITALIAN,  PKSAV_GBA_LANGUAGE_ITALIAN)
        (pkmn::e_language::GERMAN,   PKSAV_GBA_LANGUAGE_GERMAN)
        (pkmn::e_language::KOREAN,   PKSAV_GBA_LANGUAGE_KOREAN) // Unused, but the value's there
        (pkmn::e_language::SPANISH,  PKSAV_GBA_LANGUAGE_SPANISH)
    ;

    return GBA_LANGUAGE_BIMAP;
}

const nature_bimap_t& get_nature_bimap()
{
    static const nature_bimap_t NATURE_BIMAP =
    boost::assign::list_of<nature_bimap_t::relation>
        (pkmn::e_nature::HARDY,   PKSAV_NATURE_HARDY)
        (pkmn::e_nature::LONELY,  PKSAV_NATURE_LONELY)
        (pkmn::e_nature::BRAVE,   PKSAV_NATURE_BRAVE)
        (pkmn::e_nature::ADAMANT, PKSAV_NATURE_ADAMANT)
        (pkmn::e_nature::NAUGHTY, PKSAV_NATURE_NAUGHTY)
        (pkmn::e_nature::BOLD,    PKSAV_NATURE_BOLD)
        (pkmn::e_nature::DOCILE,  PKSAV_NATURE_DOCILE)
        (pkmn::e_nature::RELAXED, PKSAV_NATURE_RELAXED)
        (pkmn::e_nature::IMPISH,  PKSAV_NATURE_IMPISH)
        (pkmn::e_nature::LAX,     PKSAV_NATURE_LAX)
        (pkmn::e_nature::TIMID,   PKSAV_NATURE_TIMID)
        (pkmn::e_nature::HASTY,   PKSAV_NATURE_HASTY)
        (pkmn::e_nature::SERIOUS, PKSAV_NATURE_SERIOUS)
        (pkmn::e_nature::JOLLY,   PKSAV_NATURE_JOLLY)
        (pkmn::e_nature::NAIVE,   PKSAV_NATURE_NAIVE)
        (pkmn::e_nature::MODEST,  PKSAV_NATURE_MODEST)
        (pkmn::e_nature::MILD,    PKSAV_NATURE_MILD)
        (pkmn::e_nature::QUIET,   PKSAV_NATURE_QUIET)
        (pkmn::e_nature::BASHFUL, PKSAV_NATURE_BASHFUL)
        (pkmn::e_nature::RASH,    PKSAV_NATURE_RASH)
        (pkmn::e_nature::CALM,    PKSAV_NATURE_CALM)
        (pkmn::e_nature::GENTLE,  PKSAV_NATURE_GENTLE)
        (pkmn::e_nature::SASSY,   PKSAV_NATURE_SASSY)
        (pkmn::e_nature::CAREFUL, PKSAV_NATURE_CAREFUL)
        (pkmn::e_nature::QUIRKY,  PKSAV_NATURE_QUIRKY)
    ;

    return NATURE_BIMAP;
}

}

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

// Stats

const gb_IV_bimap_t& get_gb_IV_bimap()
{
    static const gb_IV_bimap_t GB_IV_BIMAP =
    boost::assign::list_of<gb_IV_bimap_t::relation>
        (pkmn::e_stat::HP,      PKSAV_GB_IV_HP)
        (pkmn::e_stat::ATTACK,  PKSAV_GB_IV_ATTACK)
        (pkmn::e_stat::DEFENSE, PKSAV_GB_IV_DEFENSE)
        (pkmn::e_stat::SPEED,   PKSAV_GB_IV_SPEED)
        (pkmn::e_stat::SPECIAL, PKSAV_GB_IV_SPECIAL)
    ;

    return GB_IV_BIMAP;
}

const IV_bimap_t& get_IV_bimap()
{
    static const IV_bimap_t IV_BIMAP =
    boost::assign::list_of<IV_bimap_t::relation>
        (pkmn::e_stat::HP,              PKSAV_IV_HP)
        (pkmn::e_stat::ATTACK,          PKSAV_IV_ATTACK)
        (pkmn::e_stat::DEFENSE,         PKSAV_IV_DEFENSE)
        (pkmn::e_stat::SPEED,           PKSAV_IV_SPEED)
        (pkmn::e_stat::SPECIAL_ATTACK,  PKSAV_IV_SPATK)
        (pkmn::e_stat::SPECIAL_DEFENSE, PKSAV_IV_SPDEF)
    ;

    return IV_BIMAP;
}

// Genders

const gen2_gender_bimap_t& get_gen2_gender_bimap()
{
    static const gen2_gender_bimap_t GEN2_GENDER_BIMAP =
    boost::assign::list_of<gen2_gender_bimap_t::relation>
        (pkmn::e_gender::MALE,   PKSAV_GEN2_GENDER_MALE)
        (pkmn::e_gender::FEMALE, PKSAV_GEN2_GENDER_FEMALE)
    ;

    return GEN2_GENDER_BIMAP;
}

// Options

const gen1_rb_text_speed_bimap_t& get_gen1_rb_text_speed_bimap()
{
    static const gen1_rb_text_speed_bimap_t GEN1_RB_TEXT_SPEED_BIMAP =
    boost::assign::list_of<gen1_rb_text_speed_bimap_t::relation>
        ("Fast",   PKSAV_GEN1_RB_TEXT_SPEED_FAST)
        ("Normal", PKSAV_GEN1_RB_TEXT_SPEED_NORMAL)
        ("Slow",   PKSAV_GEN1_RB_TEXT_SPEED_SLOW)
    ;

    return GEN1_RB_TEXT_SPEED_BIMAP;
}

const gen1_yellow_sound_option_bimap_t& get_gen1_yellow_sound_option_bimap()
{
    static const gen1_yellow_sound_option_bimap_t GEN1_YELLOW_SOUND_OPTION_BIMAP =
    boost::assign::list_of<gen1_yellow_sound_option_bimap_t::relation>
        ("Mono",      PKSAV_GEN1_YELLOW_SOUND_MONO)
        ("Earphone1", PKSAV_GEN1_YELLOW_SOUND_EARPHONE1)
        ("Earphone2", PKSAV_GEN1_YELLOW_SOUND_EARPHONE2)
        ("Earphone3", PKSAV_GEN1_YELLOW_SOUND_EARPHONE3)
    ;

    return GEN1_YELLOW_SOUND_OPTION_BIMAP;
}

const gen2_text_speed_bimap_t& get_gen2_text_speed_bimap()
{
    static const gen2_text_speed_bimap_t GEN2_TEXT_SPEED_BIMAP =
    boost::assign::list_of<gen2_text_speed_bimap_t::relation>
        ("Fast",   PKSAV_GEN2_TEXT_SPEED_FAST)
        ("Normal", PKSAV_GEN2_TEXT_SPEED_NORMAL)
        ("Slow",   PKSAV_GEN2_TEXT_SPEED_SLOW)
    ;

    return GEN2_TEXT_SPEED_BIMAP;
}

const gen2_gbprinter_brightness_bimap_t& get_gen2_gbprinter_brightness_bimap()
{
    static const gen2_gbprinter_brightness_bimap_t GEN2_GBPRINTER_BRIGHTNESS_BIMAP =
    boost::assign::list_of<gen2_gbprinter_brightness_bimap_t::relation>
        ("Lightest", PKSAV_GEN2_GBPRINTER_BRIGHTNESS_LIGHTEST)
        ("Lighter",  PKSAV_GEN2_GBPRINTER_BRIGHTNESS_LIGHTER)
        ("Normal",   PKSAV_GEN2_GBPRINTER_BRIGHTNESS_NORMAL)
        ("Darker",   PKSAV_GEN2_GBPRINTER_BRIGHTNESS_DARKER)
        ("Darkest",  PKSAV_GEN2_GBPRINTER_BRIGHTNESS_DARKEST)
    ;

    return GEN2_GBPRINTER_BRIGHTNESS_BIMAP;
}

const gba_rse_button_mode_bimap_t& get_gba_rse_button_mode_bimap()
{
    static const gba_rse_button_mode_bimap_t GBA_RSE_BUTTON_MODE_BIMAP =
    boost::assign::list_of<gba_rse_button_mode_bimap_t::relation>
        ("Normal", PKSAV_GBA_RSE_BUTTON_MODE_NORMAL)
        ("LR",     PKSAV_GBA_RSE_BUTTON_MODE_LR)
        ("L=A",    PKSAV_GBA_RSE_BUTTON_MODE_L_EQUALS_A)
    ;

    return GBA_RSE_BUTTON_MODE_BIMAP;
}

const gba_frlg_button_mode_bimap_t& get_gba_frlg_button_mode_bimap()
{
    static const gba_frlg_button_mode_bimap_t GBA_FRLG_BUTTON_MODE_BIMAP =
    boost::assign::list_of<gba_frlg_button_mode_bimap_t::relation>
        ("Help", PKSAV_GBA_FRLG_BUTTON_MODE_HELP)
        ("LR",   PKSAV_GBA_FRLG_BUTTON_MODE_LR)
        ("L=A",  PKSAV_GBA_FRLG_BUTTON_MODE_L_EQUALS_A)
    ;

    return GBA_FRLG_BUTTON_MODE_BIMAP;
}

// Status conditions

const gb_condition_bimap_t& get_gb_condition_bimap()
{
    static const gb_condition_bimap_t GB_CONDITION_BIMAP =
    boost::assign::list_of<gb_condition_bimap_t::relation>
        ("None",      PKSAV_GB_CONDITION_NONE)
        ("Asleep",    PKSAV_GB_CONDITION_ASLEEP)
        ("Poison",    PKSAV_GB_CONDITION_POISON)
        ("Burn",      PKSAV_GB_CONDITION_BURN)
        ("Frozen",    PKSAV_GB_CONDITION_FROZEN)
        ("Paralysis", PKSAV_GB_CONDITION_PARALYZ)
    ;

    return GB_CONDITION_BIMAP;
}

const condition_mask_bimap_t& get_condition_mask_bimap()
{
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

    return CONDITION_MASK_BIMAP;
}

const gba_text_speed_bimap_t& get_gba_text_speed_bimap()
{
    static const gba_text_speed_bimap_t GBA_TEXT_SPEED_BIMAP =
    boost::assign::list_of<gba_text_speed_bimap_t::relation>
        ("Slow",   PKSAV_GBA_TEXT_SPEED_SLOW)
        ("Medium", PKSAV_GBA_TEXT_SPEED_SLOW)
        ("Fast",   PKSAV_GBA_TEXT_SPEED_SLOW)
    ;

    return GBA_TEXT_SPEED_BIMAP;
}

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

// Misc

const gen1_type_bimap_t& get_gen1_type_bimap()
{
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

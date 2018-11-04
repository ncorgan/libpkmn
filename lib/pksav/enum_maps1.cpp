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
        (pkmn::e_condition::NONE,      PKSAV_GB_CONDITION_NONE)
        (pkmn::e_condition::ASLEEP,    PKSAV_GB_CONDITION_ASLEEP)
        (pkmn::e_condition::POISON,    PKSAV_GB_CONDITION_POISON)
        (pkmn::e_condition::BURN,      PKSAV_GB_CONDITION_BURN)
        (pkmn::e_condition::FROZEN,    PKSAV_GB_CONDITION_FROZEN)
        (pkmn::e_condition::PARALYSIS, PKSAV_GB_CONDITION_PARALYZ)
    ;

    return GB_CONDITION_BIMAP;
}

const condition_mask_bimap_t& get_condition_mask_bimap()
{
    static const condition_mask_bimap_t CONDITION_MASK_BIMAP =
    boost::assign::list_of<condition_mask_bimap_t::relation>
        (pkmn::e_condition::NONE,       PKSAV_CONDITION_NONE)
        (pkmn::e_condition::ASLEEP,     PKSAV_CONDITION_SLEEP_MASK)
        (pkmn::e_condition::POISON,     PKSAV_CONDITION_POISON)
        (pkmn::e_condition::BURN,       PKSAV_CONDITION_BURN)
        (pkmn::e_condition::FROZEN,     PKSAV_CONDITION_FROZEN)
        (pkmn::e_condition::PARALYSIS,  PKSAV_CONDITION_PARALYZ)
        (pkmn::e_condition::BAD_POISON, PKSAV_CONDITION_BAD_POISON)
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

}

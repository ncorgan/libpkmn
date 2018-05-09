/*
 * Copyright (c) 2017,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMN_PKSAV_ENUM_MAPS_HPP
#define INCLUDED_PKMN_PKSAV_ENUM_MAPS_HPP

#include <pksav/gen1/options.h>
#include <pksav/gen1/pokemon.h>
#include <pksav/gen1/type.h>

#include <pksav/gen2/mom_money_policy.h>
#include <pksav/gen2/options.h>

#include <pksav/gba/box_wallpaper.h>
#include <pksav/gba/language.h>
#include <pksav/gba/options.h>

#include <pksav/common/condition.h>
#include <pksav/common/nature.h>

#include <boost/assign.hpp>
#include <boost/bimap.hpp>

#include <string>

namespace pksav
{
    // Options
    typedef boost::bimap<std::string, enum pksav_gen1_rb_text_speed> gen1_rb_text_speed_bimap_t;
    typedef boost::bimap<std::string, enum pksav_gen1_yellow_sound_option> gen1_yellow_sound_option_bimap_t;
    typedef boost::bimap<std::string, enum pksav_gen2_text_speed> gen2_text_speed_bimap_t;
    typedef boost::bimap<std::string, enum pksav_gen2_gbprinter_brightness> gen2_gbprinter_brightness_bimap_t;
    typedef boost::bimap<std::string, enum pksav_gba_rse_button_mode> gba_rse_button_mode_bimap_t;
    typedef boost::bimap<std::string, enum pksav_gba_frlg_button_mode> gba_frlg_button_mode_bimap_t;
    typedef boost::bimap<std::string, enum pksav_gba_text_speed> gba_text_speed_bimap_t;

    const gen1_rb_text_speed_bimap_t& get_gen1_rb_text_speed_bimap();
    const gen1_yellow_sound_option_bimap_t& get_gen1_yellow_sound_option_bimap();

    const gen2_text_speed_bimap_t& get_gen2_text_speed_bimap();
    const gen2_gbprinter_brightness_bimap_t& get_gen2_gbprinter_brightness_bimap();

    const gba_rse_button_mode_bimap_t& get_gba_rse_button_mode_bimap();
    const gba_frlg_button_mode_bimap_t& get_gba_frlg_button_mode_bimap();
    const gba_text_speed_bimap_t& get_gba_text_speed_bimap();

    // Status conditions
    typedef boost::bimap<std::string, enum pksav_gb_condition> gb_condition_bimap_t;
    typedef boost::bimap<std::string, enum pksav_condition_mask> condition_mask_bimap_t;

    const gb_condition_bimap_t& get_gb_condition_bimap();
    const condition_mask_bimap_t& get_condition_mask_bimap();

    // Box wallpapers
    typedef boost::bimap<std::string, enum pksav_gba_box_wallpaper> gba_box_wallpaper_bimap_t;
    typedef boost::bimap<std::string, enum pksav_gba_rse_box_wallpaper> gba_rse_box_wallpaper_bimap_t;
    typedef boost::bimap<std::string, enum pksav_gba_frlg_box_wallpaper> gba_frlg_box_wallpaper_bimap_t;

    const gba_box_wallpaper_bimap_t& get_gba_box_wallpaper_bimap();
    const gba_rse_box_wallpaper_bimap_t& get_gba_rse_box_wallpaper_bimap();
    const gba_frlg_box_wallpaper_bimap_t& get_gba_frlg_box_wallpaper_bimap();

    // Misc
    typedef boost::bimap<std::string, enum pksav_gen1_type> gen1_type_bimap_t;
    typedef boost::bimap<std::string, enum pksav_gen2_mom_money_policy> gen2_mom_money_policy_bimap_t;
    typedef boost::bimap<std::string, enum pksav_gba_language> gba_language_bimap_t;
    typedef boost::bimap<std::string, enum pksav_nature> nature_bimap_t;

    const gen1_type_bimap_t& get_gen1_type_bimap();
    const gen2_mom_money_policy_bimap_t& get_gen2_mom_money_policy_bimap();
    const gba_language_bimap_t& get_gba_language_bimap();
    const nature_bimap_t& get_nature_bimap();
}

#endif /* INCLUDED_PKMN_PKSAV_ENUM_MAPS_HPP */

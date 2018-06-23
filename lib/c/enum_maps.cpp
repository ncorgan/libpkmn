/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "enum_maps.hpp"

namespace pkmn { namespace c {

    const game_bimap_t& get_game_bimap()
    {
        static const game_bimap_t GAME_BIMAP =
        boost::assign::list_of<game_bimap_t::relation>
            (pkmn::e_game::NONE,           PKMN_GAME_NONE)
            (pkmn::e_game::RED,            PKMN_GAME_RED)
            (pkmn::e_game::BLUE,           PKMN_GAME_BLUE)
            (pkmn::e_game::YELLOW,         PKMN_GAME_YELLOW)
            (pkmn::e_game::GOLD,           PKMN_GAME_GOLD)
            (pkmn::e_game::SILVER,         PKMN_GAME_SILVER)
            (pkmn::e_game::CRYSTAL,        PKMN_GAME_CRYSTAL)
            (pkmn::e_game::RUBY,           PKMN_GAME_RUBY)
            (pkmn::e_game::SAPPHIRE,       PKMN_GAME_SAPPHIRE)
            (pkmn::e_game::EMERALD,        PKMN_GAME_EMERALD)
            (pkmn::e_game::FIRERED,        PKMN_GAME_FIRERED)
            (pkmn::e_game::LEAFGREEN,      PKMN_GAME_LEAFGREEN)
            (pkmn::e_game::COLOSSEUM,      PKMN_GAME_COLOSSEUM)
            (pkmn::e_game::XD,             PKMN_GAME_XD)
            (pkmn::e_game::DIAMOND,        PKMN_GAME_DIAMOND)
            (pkmn::e_game::PEARL,          PKMN_GAME_PEARL)
            (pkmn::e_game::PLATINUM,       PKMN_GAME_PLATINUM)
            (pkmn::e_game::HEARTGOLD,      PKMN_GAME_HEARTGOLD)
            (pkmn::e_game::SOULSILVER,     PKMN_GAME_SOULSILVER)
            (pkmn::e_game::BLACK,          PKMN_GAME_BLACK)
            (pkmn::e_game::WHITE,          PKMN_GAME_WHITE)
            (pkmn::e_game::BLACK2,         PKMN_GAME_BLACK2)
            (pkmn::e_game::WHITE2,         PKMN_GAME_WHITE2)
            (pkmn::e_game::X,              PKMN_GAME_X)
            (pkmn::e_game::Y,              PKMN_GAME_Y)
            (pkmn::e_game::OMEGA_RUBY,     PKMN_GAME_OMEGA_RUBY)
            (pkmn::e_game::ALPHA_SAPPHIRE, PKMN_GAME_ALPHA_SAPPHIRE)
        ;

        return GAME_BIMAP;
    }

    const game_save_type_bimap_t& get_game_save_type_bimap()
    {
        static const game_save_type_bimap_t GAME_SAVE_TYPE_BIMAP =
        boost::assign::list_of<game_save_type_bimap_t::relation>
            (pkmn::e_game_save_type::NONE, PKMN_GAME_SAVE_TYPE_NONE)
            (pkmn::e_game_save_type::RED_BLUE, PKMN_GAME_SAVE_TYPE_RED_BLUE)
            (pkmn::e_game_save_type::YELLOW, PKMN_GAME_SAVE_TYPE_YELLOW)
            (pkmn::e_game_save_type::GOLD_SILVER, PKMN_GAME_SAVE_TYPE_GOLD_SILVER)
            (pkmn::e_game_save_type::CRYSTAL, PKMN_GAME_SAVE_TYPE_CRYSTAL)
            (pkmn::e_game_save_type::RUBY_SAPPHIRE, PKMN_GAME_SAVE_TYPE_RUBY_SAPPHIRE)
            (pkmn::e_game_save_type::EMERALD, PKMN_GAME_SAVE_TYPE_EMERALD)
            (pkmn::e_game_save_type::FIRERED_LEAFGREEN, PKMN_GAME_SAVE_TYPE_FIRERED_LEAFGREEN)
            (pkmn::e_game_save_type::COLOSSEUM_XD, PKMN_GAME_SAVE_TYPE_COLOSSEUM_XD)
        ;

        return GAME_SAVE_TYPE_BIMAP;
    }

    const gender_bimap_t& get_gender_bimap()
    {
        static const gender_bimap_t GENDER_BIMAP =
        boost::assign::list_of<gender_bimap_t::relation>
            (pkmn::e_gender::MALE,       PKMN_GENDER_MALE)
            (pkmn::e_gender::FEMALE,     PKMN_GENDER_FEMALE)
            (pkmn::e_gender::GENDERLESS, PKMN_GENDER_GENDERLESS)
        ;

        return GENDER_BIMAP;
    }

    const language_bimap_t& get_language_bimap()
    {
        static const language_bimap_t LANGUAGE_BIMAP =
        boost::assign::list_of<language_bimap_t::relation>
            (pkmn::e_language::JAPANESE, PKMN_LANGUAGE_JAPANESE)
            (pkmn::e_language::ENGLISH,  PKMN_LANGUAGE_ENGLISH)
            (pkmn::e_language::GERMAN,   PKMN_LANGUAGE_GERMAN)
            (pkmn::e_language::FRENCH,   PKMN_LANGUAGE_FRENCH)
            (pkmn::e_language::ITALIAN,  PKMN_LANGUAGE_ITALIAN)
            (pkmn::e_language::SPANISH,  PKMN_LANGUAGE_SPANISH)
            (pkmn::e_language::KOREAN,   PKMN_LANGUAGE_KOREAN)
        ;

        return LANGUAGE_BIMAP;
    }

    const stat_bimap_t& get_stat_bimap()
    {
        static const stat_bimap_t STAT_BIMAP =
        boost::assign::list_of<stat_bimap_t::relation>
            (pkmn::e_stat::HP,              PKMN_STAT_HP)
            (pkmn::e_stat::ATTACK,          PKMN_STAT_ATTACK)
            (pkmn::e_stat::DEFENSE,         PKMN_STAT_DEFENSE)
            (pkmn::e_stat::SPEED,           PKMN_STAT_SPEED)
            (pkmn::e_stat::SPECIAL,         PKMN_STAT_SPECIAL)
            (pkmn::e_stat::SPECIAL_ATTACK,  PKMN_STAT_SPATK)
            (pkmn::e_stat::SPECIAL_DEFENSE, PKMN_STAT_SPDEF)
        ;

        return STAT_BIMAP;
    }

    const contest_stat_bimap_t& get_contest_stat_bimap()
    {
        static const contest_stat_bimap_t CONTEST_STAT_BIMAP =
        boost::assign::list_of<contest_stat_bimap_t::relation>
            ("Cool",   PKMN_CONTEST_STAT_COOL)
            ("Beauty", PKMN_CONTEST_STAT_BEAUTY)
            ("Cute",   PKMN_CONTEST_STAT_CUTE)
            ("Smart",  PKMN_CONTEST_STAT_SMART)
            ("Tough",  PKMN_CONTEST_STAT_TOUGH)
            ("Feel",   PKMN_CONTEST_STAT_FEEL)
            ("Sheen",  PKMN_CONTEST_STAT_SHEEN)
        ;

        return CONTEST_STAT_BIMAP;
    }

    const condition_bimap_t& get_condition_bimap()
    {
        static const condition_bimap_t CONDITION_BIMAP =
        boost::assign::list_of<condition_bimap_t::relation>
            ("None",       PKMN_CONDITION_NONE)
            ("Asleep",     PKMN_CONDITION_ASLEEP)
            ("Poison",     PKMN_CONDITION_POISON)
            ("Burn",       PKMN_CONDITION_BURN)
            ("Frozen",     PKMN_CONDITION_FROZEN)
            ("Paralysis",  PKMN_CONDITION_PARALYSIS)
            ("Bad Poison", PKMN_CONDITION_BAD_POISON)
        ;

        return CONDITION_BIMAP;
    }

    const marking_bimap_t& get_gen3_marking_bimap()
    {
        static const marking_bimap_t GEN3_MARKING_BIMAP =
        boost::assign::list_of<marking_bimap_t::relation>
            ("Circle",   PKMN_MARKING_CIRCLE)
            ("Triangle", PKMN_MARKING_TRIANGLE)
            ("Square",   PKMN_MARKING_SQUARE)
            ("Heart",    PKMN_MARKING_HEART)
        ;

        return GEN3_MARKING_BIMAP;
    }

    const marking_bimap_t& get_marking_bimap()
    {
        static const marking_bimap_t MARKING_BIMAP =
        boost::assign::list_of<marking_bimap_t::relation>
            ("Circle",   PKMN_MARKING_CIRCLE)
            ("Triangle", PKMN_MARKING_TRIANGLE)
            ("Square",   PKMN_MARKING_SQUARE)
            ("Heart",    PKMN_MARKING_HEART)
            ("Star",     PKMN_MARKING_STAR)
            ("Diamond",  PKMN_MARKING_DIAMOND)
        ;

        return MARKING_BIMAP;
    }
}
}

/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "database/id_to_string.hpp"

#include <pkmn/enums/enum_to_string.hpp>

#include <boost/assign.hpp>
#include <boost/bimap.hpp>

#include <exception>

namespace pkmn {

    // Internal bimaps for what's not stored in the database

    typedef boost::bimap<std::string, pkmn::e_condition> condition_bimap_t;
    static const condition_bimap_t& get_condition_bimap()
    {
        static const condition_bimap_t CONDITION_BIMAP =
        boost::assign::list_of<condition_bimap_t::relation>
            ("None",       pkmn::e_condition::NONE)
            ("Asleep",     pkmn::e_condition::ASLEEP)
            ("Poison",     pkmn::e_condition::POISON)
            ("Burn",       pkmn::e_condition::BURN)
            ("Frozen",     pkmn::e_condition::FROZEN)
            ("Paralysis",  pkmn::e_condition::PARALYSIS)
            ("Bad Poison", pkmn::e_condition::BAD_POISON)
        ;

        return CONDITION_BIMAP;
    }

    typedef boost::bimap<std::string, pkmn::e_gender> gender_bimap_t;
    static const gender_bimap_t& get_gender_bimap()
    {
        static const gender_bimap_t GENDER_BIMAP =
        boost::assign::list_of<gender_bimap_t::relation>
            ("None",       pkmn::e_gender::NONE)
            ("Male",       pkmn::e_gender::MALE)
            ("Female",     pkmn::e_gender::FEMALE)
            ("Genderless", pkmn::e_gender::GENDERLESS)
        ;

        return GENDER_BIMAP;
    }

    typedef boost::bimap<std::string, pkmn::e_language> language_bimap_t;
    static const language_bimap_t& get_language_bimap()
    {
        static const language_bimap_t LANGUAGE_BIMAP =
        boost::assign::list_of<language_bimap_t::relation>
            ("None",     pkmn::e_language::NONE)
            ("Japanese", pkmn::e_language::JAPANESE)
            ("English",  pkmn::e_language::ENGLISH)
            ("German",   pkmn::e_language::GERMAN)
            ("French",   pkmn::e_language::FRENCH)
            ("Italian",  pkmn::e_language::ITALIAN)
            ("Spanish",  pkmn::e_language::SPANISH)
            ("Korean",   pkmn::e_language::KOREAN)
        ;

        return LANGUAGE_BIMAP;
    }

    typedef boost::bimap<std::string, pkmn::e_move_damage_class> move_damage_class_bimap_t;
    static const move_damage_class_bimap_t& get_move_damage_class_bimap()
    {
        static const move_damage_class_bimap_t MOVE_DAMAGE_CLASS_BIMAP =
        boost::assign::list_of<move_damage_class_bimap_t::relation>
            ("None",     pkmn::e_move_damage_class::NONE)
            ("Status",   pkmn::e_move_damage_class::STATUS)
            ("Physical", pkmn::e_move_damage_class::PHYSICAL)
            ("Special",  pkmn::e_move_damage_class::SPECIAL)
        ;

        return MOVE_DAMAGE_CLASS_BIMAP;
    }

    typedef boost::bimap<std::string, pkmn::e_stat> stat_bimap_t;
    static const stat_bimap_t& get_stat_bimap()
    {
        static const stat_bimap_t STAT_BIMAP =
        boost::assign::list_of<stat_bimap_t::relation>
            ("None",            pkmn::e_stat::NONE)
            ("HP",              pkmn::e_stat::HP)
            ("Attack",          pkmn::e_stat::ATTACK)
            ("Defense",         pkmn::e_stat::DEFENSE)
            ("Speed",           pkmn::e_stat::SPEED)
            ("Special",         pkmn::e_stat::SPECIAL)
            ("Special Attack",  pkmn::e_stat::SPECIAL_ATTACK)
            ("Special Defense", pkmn::e_stat::SPECIAL_DEFENSE)
        ;

        return STAT_BIMAP;
    }

    // User-facing functions

    std::string ability_to_string(pkmn::e_ability ability)
    {
        return pkmn::database::ability_id_to_name(
                   static_cast<int>(ability)
               );
    }

    std::string ball_to_string(pkmn::e_ball ball)
    {
        std::string ball_name;

        if(ball == pkmn::e_ball::INVALID)
        {
            ball_name = "Invalid";
        }
        else
        {
            ball_name = pkmn::database::ball_id_to_name(
                            static_cast<int>(ball)
                        );
        }

        return ball_name;
    }

    std::string condition_to_string(pkmn::e_condition condition)
    {
        static const condition_bimap_t& CONDITION_BIMAP = get_condition_bimap();

        auto condition_iter = CONDITION_BIMAP.right.find(condition);
        if(condition_iter != CONDITION_BIMAP.right.end())
        {
            return condition_iter->second;
        }
        else
        {
            const std::string error_message = "Invalid condition: "
                                            + std::to_string(static_cast<int>(condition));
            throw std::invalid_argument(error_message);
        }
    }

    std::string egg_group_to_string(pkmn::e_egg_group egg_group)
    {
        return pkmn::database::egg_group_id_to_name(
                   static_cast<int>(egg_group)
               );
    }

    std::string game_to_string(pkmn::e_game game)
    {
        return pkmn::database::game_id_to_name(
                   static_cast<int>(game)
               );
    }

    std::string gender_to_string(pkmn::e_gender gender)
    {
        static const gender_bimap_t& GENDER_BIMAP = get_gender_bimap();

        auto gender_iter = GENDER_BIMAP.right.find(gender);
        if(gender_iter != GENDER_BIMAP.right.end())
        {
            return gender_iter->second;
        }
        else
        {
            const std::string error_message = "Invalid gender: "
                                            + std::to_string(static_cast<int>(gender));
            throw std::invalid_argument(error_message);
        }
    }

    std::string item_to_string(pkmn::e_item item)
    {
        std::string item_name;

        if(item == pkmn::e_item::INVALID)
        {
            item_name = "Invalid";
        }
        else
        {
            // So we get the latest spelling.
            static const int ORAS_VERSION_GROUP = 26;

            item_name = pkmn::database::item_id_to_name(
                            static_cast<int>(item),
                            ORAS_VERSION_GROUP
                        );
        }

        return item_name;
    }

    std::string language_to_string(pkmn::e_language language)
    {
        static const language_bimap_t& LANGUAGE_BIMAP = get_language_bimap();

        auto language_iter = LANGUAGE_BIMAP.right.find(language);
        if(language_iter != LANGUAGE_BIMAP.right.end())
        {
            return language_iter->second;
        }
        else
        {
            const std::string error_message = "Invalid language: "
                                            + std::to_string(static_cast<int>(language));
            throw std::invalid_argument(error_message);
        }
    }

    std::string move_damage_class_to_string(pkmn::e_move_damage_class move_damage_class)
    {
        static const move_damage_class_bimap_t& MOVE_DAMAGE_CLASS_BIMAP = get_move_damage_class_bimap();

        auto move_damage_class_iter = MOVE_DAMAGE_CLASS_BIMAP.right.find(move_damage_class);
        if(move_damage_class_iter != MOVE_DAMAGE_CLASS_BIMAP.right.end())
        {
            return move_damage_class_iter->second;
        }
        else
        {
            const std::string error_message = "Invalid move damage class: "
                                            + std::to_string(static_cast<int>(move_damage_class));
            throw std::invalid_argument(error_message);
        }
    }

    std::string move_to_string(pkmn::e_move move)
    {
        // So we get the latest spelling.
        static const int GENERATION = 6;

        return pkmn::database::move_id_to_name(
                   static_cast<int>(move),
                   GENERATION
               );
    }

    std::string nature_to_string(pkmn::e_nature nature)
    {
        return pkmn::database::nature_id_to_name(
                   static_cast<int>(nature)
               );
    }

    std::string species_to_string(pkmn::e_species species)
    {
        return pkmn::database::species_id_to_name(
                   static_cast<int>(species)
               );
    }

    std::string stat_to_string(pkmn::e_stat stat)
    {
        static const stat_bimap_t& STAT_BIMAP = get_stat_bimap();

        auto stat_iter = STAT_BIMAP.right.find(stat);
        if(stat_iter != STAT_BIMAP.right.end())
        {
            return stat_iter->second;
        }
        else
        {
            const std::string error_message = "Invalid stat: "
                                            + std::to_string(static_cast<int>(stat));
            throw std::invalid_argument(error_message);
        }
    }

    std::string type_to_string(pkmn::e_type type)
    {
        return pkmn::database::type_id_to_name(
                   static_cast<int>(type)
               );
    }

}

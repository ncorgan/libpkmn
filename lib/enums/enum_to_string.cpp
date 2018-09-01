/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "database/database_common.hpp"
#include "database/enum_conversions.hpp"
#include "database/id_to_string.hpp"
#include "database/index_to_string.hpp"

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

    typedef boost::bimap<std::string, pkmn::e_contest_stat> contest_stat_bimap_t;
    static const contest_stat_bimap_t& get_contest_stat_bimap()
    {
        static const contest_stat_bimap_t CONTEST_STAT_BIMAP =
        boost::assign::list_of<contest_stat_bimap_t::relation>
            ("None",   pkmn::e_contest_stat::NONE)
            ("Cool",   pkmn::e_contest_stat::COOL)
            ("Cute",   pkmn::e_contest_stat::CUTE)
            ("Beauty", pkmn::e_contest_stat::BEAUTY)
            ("Smart",  pkmn::e_contest_stat::SMART)
            ("Tough",  pkmn::e_contest_stat::TOUGH)
            ("Feel",   pkmn::e_contest_stat::FEEL)
            ("Sheen",  pkmn::e_contest_stat::SHEEN)
        ;

        return CONTEST_STAT_BIMAP;
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

    pkmn::e_ability string_to_ability(const std::string& ability_name)
    {
        return static_cast<pkmn::e_ability>(
                   pkmn::database::ability_name_to_id(ability_name)
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

    pkmn::e_ball string_to_ball(const std::string& ball_name)
    {
        return static_cast<pkmn::e_ball>(
                   pkmn::database::ball_name_to_id(ball_name)
               );
    }

    std::string condition_to_string(pkmn::e_condition condition)
    {
        std::string ret;

        static const condition_bimap_t& CONDITION_BIMAP = get_condition_bimap();

        auto condition_iter = CONDITION_BIMAP.right.find(condition);
        if(condition_iter != CONDITION_BIMAP.right.end())
        {
            ret = condition_iter->second;
        }
        else
        {
            const std::string error_message = "Invalid condition: "
                                            + std::to_string(static_cast<int>(condition));
            throw std::invalid_argument(error_message);
        }

        return ret;
    }

    pkmn::e_condition string_to_condition(const std::string& condition_name)
    {
        pkmn::e_condition ret = pkmn::e_condition::NONE;

        static const condition_bimap_t& CONDITION_BIMAP = get_condition_bimap();

        auto condition_name_iter = CONDITION_BIMAP.left.find(condition_name);
        if(condition_name_iter != CONDITION_BIMAP.left.end())
        {
            ret = condition_name_iter->second;
        }
        else
        {
            const std::string error_message = "Invalid condition: " + condition_name;
            throw std::invalid_argument(error_message);
        }

        return ret;
    }

    std::string contest_stat_to_string(pkmn::e_contest_stat contest_stat)
    {
        std::string ret;

        static const contest_stat_bimap_t& CONTEST_STAT_BIMAP = get_contest_stat_bimap();

        auto contest_stat_iter = CONTEST_STAT_BIMAP.right.find(contest_stat);
        if(contest_stat_iter != CONTEST_STAT_BIMAP.right.end())
        {
            ret = contest_stat_iter->second;
        }
        else
        {
            const std::string error_message = "Invalid contest_stat: "
                                            + std::to_string(static_cast<int>(contest_stat));
            throw std::invalid_argument(error_message);
        }

        return ret;
    }

    pkmn::e_contest_stat string_to_contest_stat(const std::string& contest_stat_name)
    {
        pkmn::e_contest_stat ret = pkmn::e_contest_stat::NONE;

        static const contest_stat_bimap_t& CONTEST_STAT_BIMAP = get_contest_stat_bimap();

        auto contest_stat_name_iter = CONTEST_STAT_BIMAP.left.find(contest_stat_name);
        if(contest_stat_name_iter != CONTEST_STAT_BIMAP.left.end())
        {
            ret = contest_stat_name_iter->second;
        }
        else
        {
            const std::string error_message = "Invalid contest_stat: " + contest_stat_name;
            throw std::invalid_argument(error_message);
        }

        return ret;
    }

    std::string egg_group_to_string(pkmn::e_egg_group egg_group)
    {
        return pkmn::database::egg_group_id_to_name(
                   static_cast<int>(egg_group)
               );
    }

    pkmn::e_egg_group string_to_egg_group(const std::string& egg_group_name)
    {
        return static_cast<pkmn::e_egg_group>(
                   pkmn::database::egg_group_name_to_id(egg_group_name)
               );
    }

    std::string game_to_string(pkmn::e_game game)
    {
        return pkmn::database::game_enum_to_name(game);
    }

    pkmn::e_game string_to_game(const std::string& game_name)
    {
        return pkmn::database::game_id_to_enum(
                   pkmn::database::game_name_to_id(game_name)
               );
    }

    std::string gender_to_string(pkmn::e_gender gender)
    {
        std::string ret;

        static const gender_bimap_t& GENDER_BIMAP = get_gender_bimap();

        auto gender_iter = GENDER_BIMAP.right.find(gender);
        if(gender_iter != GENDER_BIMAP.right.end())
        {
            ret = gender_iter->second;
        }
        else
        {
            const std::string error_message = "Invalid gender: "
                                            + std::to_string(static_cast<int>(gender));
            throw std::invalid_argument(error_message);
        }

        return ret;
    }

    pkmn::e_gender string_to_gender(const std::string& gender_name)
    {
        pkmn::e_gender ret = pkmn::e_gender::NONE;

        static const gender_bimap_t& GENDER_BIMAP = get_gender_bimap();

        auto gender_name_iter = GENDER_BIMAP.left.find(gender_name);
        if(gender_name_iter != GENDER_BIMAP.left.end())
        {
            ret = gender_name_iter->second;
        }
        else
        {
            const std::string error_message = "Invalid gender: " + gender_name;
            throw std::invalid_argument(error_message);
        }

        return ret;
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

    pkmn::e_item string_to_item(const std::string& item_name)
    {
        return static_cast<pkmn::e_item>(
                   pkmn::database::item_name_to_id(item_name)
               );
    }

    std::string language_to_string(pkmn::e_language language)
    {
        std::string ret;

        static const language_bimap_t& LANGUAGE_BIMAP = get_language_bimap();

        auto language_iter = LANGUAGE_BIMAP.right.find(language);
        if(language_iter != LANGUAGE_BIMAP.right.end())
        {
            ret = language_iter->second;
        }
        else
        {
            const std::string error_message = "Invalid language: "
                                            + std::to_string(static_cast<int>(language));
            throw std::invalid_argument(error_message);
        }

        return ret;
    }

    pkmn::e_language string_to_language(const std::string& language_name)
    {
        pkmn::e_language ret = pkmn::e_language::NONE;

        static const language_bimap_t& LANGUAGE_BIMAP = get_language_bimap();

        auto language_name_iter = LANGUAGE_BIMAP.left.find(language_name);
        if(language_name_iter != LANGUAGE_BIMAP.left.end())
        {
            ret = language_name_iter->second;
        }
        else
        {
            const std::string error_message = "Invalid language: " + language_name;
            throw std::invalid_argument(error_message);
        }

        return ret;
    }

    std::string move_damage_class_to_string(
        pkmn::e_move_damage_class move_damage_class
    )
    {
        std::string ret;

        static const move_damage_class_bimap_t& MOVE_DAMAGE_CLASS_BIMAP = get_move_damage_class_bimap();

        auto move_damage_class_iter = MOVE_DAMAGE_CLASS_BIMAP.right.find(move_damage_class);
        if(move_damage_class_iter != MOVE_DAMAGE_CLASS_BIMAP.right.end())
        {
            ret = move_damage_class_iter->second;
        }
        else
        {
            const std::string error_message = "Invalid move damage class: "
                                            + std::to_string(static_cast<int>(move_damage_class));
            throw std::invalid_argument(error_message);
        }

        return ret;
    }

    pkmn::e_move_damage_class string_to_move_damage_class(
        const std::string& move_damage_class_name
    )
    {
        pkmn::e_move_damage_class ret = pkmn::e_move_damage_class::NONE;

        static const move_damage_class_bimap_t& MOVE_DAMAGE_CLASS_BIMAP = get_move_damage_class_bimap();

        auto move_damage_class_name_iter = MOVE_DAMAGE_CLASS_BIMAP.left.find(move_damage_class_name);
        if(move_damage_class_name_iter != MOVE_DAMAGE_CLASS_BIMAP.left.end())
        {
            ret = move_damage_class_name_iter->second;
        }
        else
        {
            const std::string error_message = "Invalid move damage class: " + move_damage_class_name;
            throw std::invalid_argument(error_message);
        }

        return ret;
    }

    std::string move_target_to_string(pkmn::e_move_target move_target)
    {
        std::string ret = "None";

        if(move_target != pkmn::e_move_target::NONE)
        {
            static const std::string query =
                "SELECT name FROM move_target_prose WHERE move_target_id=? AND "
                "local_language_id=9";

            std::string error_message = "Invalid move target: ";
            error_message += std::to_string(static_cast<int>(move_target));

            ret = pkmn::database::query_db_bind1<std::string, int>(
                      query.c_str(),
                      static_cast<int>(move_target),
                      error_message
                  );
        }

        return ret;
    }

    pkmn::e_move_target string_to_move_target(const std::string& move_target_name)
    {
        pkmn::e_move_target ret = pkmn::e_move_target::NONE;

        if(move_target_name != "None")
        {
            static const std::string query =
                "SELECT move_target_id FROM move_target_prose WHERE name=?";

            std::string error_message = "Invalid move target: " + move_target_name;

            ret = static_cast<pkmn::e_move_target>(
                      pkmn::database::query_db_bind1<int, const std::string&>(
                          query.c_str(),
                          move_target_name,
                          error_message
                      )
                  );
        }

        return ret;
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

    pkmn::e_move string_to_move(const std::string& move_name)
    {
        return static_cast<pkmn::e_move>(
                   pkmn::database::move_name_to_id(move_name)
               );
    }

    /*
     * The nature enum is offset by 1 from the in-game index because we use
     * 0 for a "None" nature.
     */

    std::string nature_to_string(pkmn::e_nature nature)
    {
        std::string ret = "None";

        if(nature != pkmn::e_nature::NONE)
        {
            ret = pkmn::database::nature_index_to_name(
                      static_cast<int>(nature)-1,
                      true /*increment_error_message*/
                  );
        }

        return ret;
    }

    pkmn::e_nature string_to_nature(const std::string& nature_name)
    {
        pkmn::e_nature ret = pkmn::e_nature::NONE;

        if(nature_name != "None")
        {
            ret = static_cast<pkmn::e_nature>(
                      pkmn::database::nature_name_to_index(nature_name)+1
                  );
        }

        return ret;
    }

    std::string species_to_string(pkmn::e_species species)
    {
        return pkmn::database::species_id_to_name(
                   static_cast<int>(species)
               );
    }

    pkmn::e_species string_to_species(const std::string& species_name)
    {
        return static_cast<pkmn::e_species>(
                   pkmn::database::species_name_to_id(species_name)
               );
    }

    std::string stat_to_string(pkmn::e_stat stat)
    {
        std::string ret;

        static const stat_bimap_t& STAT_BIMAP = get_stat_bimap();

        auto stat_iter = STAT_BIMAP.right.find(stat);
        if(stat_iter != STAT_BIMAP.right.end())
        {
            ret = stat_iter->second;
        }
        else
        {
            const std::string error_message = "Invalid stat: "
                                            + std::to_string(static_cast<int>(stat));
            throw std::invalid_argument(error_message);
        }

        return ret;
    }

    pkmn::e_stat string_to_stat(const std::string& stat_name)
    {
        pkmn::e_stat ret = pkmn::e_stat::NONE;

        static const stat_bimap_t& STAT_BIMAP = get_stat_bimap();

        auto stat_name_iter = STAT_BIMAP.left.find(stat_name);
        if(stat_name_iter != STAT_BIMAP.left.end())
        {
            ret = stat_name_iter->second;
        }
        else
        {
            const std::string error_message = "Invalid stat: " + stat_name;
            throw std::invalid_argument(error_message);
        }

        return ret;
    }

    std::string type_to_string(pkmn::e_type type)
    {
        return pkmn::database::type_id_to_name(
                   static_cast<int>(type)
               );
    }

    pkmn::e_type string_to_type(const std::string& type_name)
    {
        return static_cast<pkmn::e_type>(
                   pkmn::database::type_name_to_id(type_name)
               );
    }

}

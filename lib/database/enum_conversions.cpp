/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "enum_conversions.hpp"

#include <boost/assign.hpp>
#include <boost/bimap.hpp>

#include <exception>

namespace pkmn { namespace database {

    typedef boost::bimap<pkmn::e_game, int> game_enum_bimap_t;

    const game_enum_bimap_t& get_game_enum_bimap()
    {
        static const game_enum_bimap_t GAME_ENUM_BIMAP =
        boost::assign::list_of<game_enum_bimap_t::relation>
            (pkmn::e_game::NONE,           0)
            (pkmn::e_game::RED,            1)
            (pkmn::e_game::BLUE,           2)
            (pkmn::e_game::YELLOW,         3)
            (pkmn::e_game::GOLD,           4)
            (pkmn::e_game::SILVER,         5)
            (pkmn::e_game::CRYSTAL,        6)
            (pkmn::e_game::RUBY,           7)
            (pkmn::e_game::SAPPHIRE,       8)
            (pkmn::e_game::EMERALD,        9)
            (pkmn::e_game::FIRERED,        10)
            (pkmn::e_game::LEAFGREEN,      11)
            (pkmn::e_game::COLOSSEUM,      19)
            (pkmn::e_game::XD,             20)
            (pkmn::e_game::DIAMOND,        12)
            (pkmn::e_game::PEARL,          13)
            (pkmn::e_game::PLATINUM,       14)
            (pkmn::e_game::HEARTGOLD,      15)
            (pkmn::e_game::SOULSILVER,     16)
            (pkmn::e_game::BLACK,          17)
            (pkmn::e_game::WHITE,          18)
            (pkmn::e_game::BLACK2,         21)
            (pkmn::e_game::WHITE2,         22)
            (pkmn::e_game::X,              23)
            (pkmn::e_game::Y,              24)
            (pkmn::e_game::OMEGA_RUBY,     25)
            (pkmn::e_game::ALPHA_SAPPHIRE, 26)
        ;

        return GAME_ENUM_BIMAP;
    };

    int game_enum_to_id(pkmn::e_game game)
    {
        int game_id = 0;

        const game_enum_bimap_t& GAME_ENUM_BIMAP = get_game_enum_bimap();

        auto enum_to_id_iter = GAME_ENUM_BIMAP.left.find(game);
        if(enum_to_id_iter != GAME_ENUM_BIMAP.left.end())
        {
            game_id = enum_to_id_iter->second;
        }
        else
        {
            std::string error_message = "Invalid game: ";
            error_message += std::to_string(static_cast<int>(game));

            throw std::invalid_argument(error_message);
        }

        return game_id;
    }

    pkmn::e_game game_id_to_enum(int game_id)
    {
        pkmn::e_game game_enum = pkmn::e_game::NONE;

        const game_enum_bimap_t& GAME_ENUM_BIMAP = get_game_enum_bimap();

        // Allow it to be none since it could be invalid.
        auto id_to_enum_iter = GAME_ENUM_BIMAP.right.find(game_id);
        if(id_to_enum_iter != GAME_ENUM_BIMAP.right.end())
        {
            game_enum = id_to_enum_iter->second;
        }

        return game_enum;
    }

    typedef boost::bimap<pkmn::e_ball, pkmn::e_item> ball_item_enum_bimap_t;

    const ball_item_enum_bimap_t& get_ball_item_enum_bimap()
    {
        static const ball_item_enum_bimap_t BALL_ITEM_ENUM_BIMAP =
        boost::assign::list_of<ball_item_enum_bimap_t::relation>
            (pkmn::e_ball::NONE,         pkmn::e_item::NONE)
            (pkmn::e_ball::MASTER_BALL,  pkmn::e_item::MASTER_BALL)
            (pkmn::e_ball::ULTRA_BALL,   pkmn::e_item::ULTRA_BALL)
            (pkmn::e_ball::GREAT_BALL,   pkmn::e_item::GREAT_BALL)
            (pkmn::e_ball::POKE_BALL,    pkmn::e_item::POKE_BALL)
            (pkmn::e_ball::SAFARI_BALL,  pkmn::e_item::SAFARI_BALL)
            (pkmn::e_ball::NET_BALL,     pkmn::e_item::NET_BALL)
            (pkmn::e_ball::DIVE_BALL,    pkmn::e_item::DIVE_BALL)
            (pkmn::e_ball::NEST_BALL,    pkmn::e_item::NEST_BALL)
            (pkmn::e_ball::REPEAT_BALL,  pkmn::e_item::REPEAT_BALL)
            (pkmn::e_ball::TIMER_BALL,   pkmn::e_item::TIMER_BALL)
            (pkmn::e_ball::LUXURY_BALL,  pkmn::e_item::LUXURY_BALL)
            (pkmn::e_ball::PREMIER_BALL, pkmn::e_item::PREMIER_BALL)
            (pkmn::e_ball::DUSK_BALL,    pkmn::e_item::DUSK_BALL)
            (pkmn::e_ball::HEAL_BALL,    pkmn::e_item::HEAL_BALL)
            (pkmn::e_ball::QUICK_BALL,   pkmn::e_item::QUICK_BALL)
            (pkmn::e_ball::CHERISH_BALL, pkmn::e_item::CHERISH_BALL)
            (pkmn::e_ball::FAST_BALL,    pkmn::e_item::FAST_BALL)
            (pkmn::e_ball::LEVEL_BALL,   pkmn::e_item::LEVEL_BALL)
            (pkmn::e_ball::LURE_BALL,    pkmn::e_item::LURE_BALL)
            (pkmn::e_ball::HEAVY_BALL,   pkmn::e_item::HEAVY_BALL)
            (pkmn::e_ball::LOVE_BALL,    pkmn::e_item::LOVE_BALL)
            (pkmn::e_ball::FRIEND_BALL,  pkmn::e_item::FRIEND_BALL)
            (pkmn::e_ball::MOON_BALL,    pkmn::e_item::MOON_BALL)
            (pkmn::e_ball::SPORT_BALL,   pkmn::e_item::SPORT_BALL)
            (pkmn::e_ball::PARK_BALL,    pkmn::e_item::PARK_BALL)
            (pkmn::e_ball::DREAM_BALL,   pkmn::e_item::DREAM_BALL)
            (pkmn::e_ball::INVALID,      pkmn::e_item::INVALID)
        ;

        return BALL_ITEM_ENUM_BIMAP;
    };

    pkmn::e_item ball_to_item(pkmn::e_ball ball)
    {
        static const ball_item_enum_bimap_t& BALL_ITEM_ENUM_BIMAP =
            get_ball_item_enum_bimap();

        pkmn::e_item ret = pkmn::e_item::NONE;

        auto ball_to_item_iter = BALL_ITEM_ENUM_BIMAP.left.find(ball);
        if(ball_to_item_iter != BALL_ITEM_ENUM_BIMAP.left.end())
        {
            ret = ball_to_item_iter->second;
        }
        else
        {
            throw std::invalid_argument("Invalid ball.");
        }

        return ret;
    }

    pkmn::e_ball item_to_ball(pkmn::e_item item)
    {
        static const ball_item_enum_bimap_t& BALL_ITEM_ENUM_BIMAP =
            get_ball_item_enum_bimap();

        pkmn::e_ball ret = pkmn::e_ball::NONE;

        auto item_to_ball_iter = BALL_ITEM_ENUM_BIMAP.right.find(item);
        if(item_to_ball_iter != BALL_ITEM_ENUM_BIMAP.right.end())
        {
            ret = item_to_ball_iter->second;
        }
        else
        {
            throw std::invalid_argument("Invalid item.");
        }

        return ret;
    }

}}

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
            throw std::invalid_argument("Invalid game.");
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

}}

/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_DATABASE_ENUM_CONVERSIONS_HPP
#define PKMN_DATABASE_ENUM_CONVERSIONS_HPP

#include "database_common.hpp"
#include "id_to_string.hpp"
#include "id_to_index.hpp"

#include <pkmn/enums/ball.hpp>
#include <pkmn/enums/game.hpp>
#include <pkmn/enums/item.hpp>

namespace pkmn { namespace database {

    int game_enum_to_id(pkmn::e_game game_enum);

    pkmn::e_game game_id_to_enum(int game_id);

    inline int game_enum_to_generation(pkmn::e_game game)
    {
        return game_id_to_generation(game_enum_to_id(game));
    }

    inline int game_enum_to_version_group(pkmn::e_game game)
    {
        return game_id_to_version_group(game_enum_to_id(game));
    }

    inline std::string game_enum_to_name(pkmn::e_game game)
    {
        return game_id_to_name(game_enum_to_id(game));
    }

    inline int game_enum_to_index(pkmn::e_game game)
    {
        return (game == pkmn::e_game::COLOSSEUM || game == pkmn::e_game::XD) ?
                   15 : game_id_to_index(game_enum_to_id(game));
    }

    inline pkmn::e_game game_index_to_enum(int index)
    {
        return (index == 15) ? pkmn::e_game::COLOSSEUM
                             : game_id_to_enum(game_index_to_id(index));
    }

    // TODO: these are both enums, should this move into the enums directory?

    pkmn::e_item ball_to_item(pkmn::e_ball ball);

    pkmn::e_ball item_to_ball(pkmn::e_item item);

}}

#endif /* PKMN_DATABASE_ENUM_CONVERSIONS_HPP */

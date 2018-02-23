/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "private_exports.hpp"

#include "database/database_common.hpp"

namespace pkmn { namespace priv {

    int game_name_to_generation(
        const std::string& game
    )
    {
        return pkmn::database::game_name_to_generation(game);
    }

    void initialize_database_connection()
    {
        pkmn::database::initialize_connection();
    }

}}

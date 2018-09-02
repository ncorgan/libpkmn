/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMN_PRIVATE_FUNCTIONS_HPP
#define INCLUDED_PKMN_PRIVATE_FUNCTIONS_HPP

#include <pkmn/config.hpp>

#include <pkmn/enums/ball.hpp>
#include <pkmn/enums/game.hpp>
#include <pkmn/enums/item.hpp>

#include <string>

/*
 * Helper functions for other LibPKMN DLL's to use that require internal
 * functionality.
 */

namespace pkmn { namespace priv {

    PKMN_API int game_name_to_generation(
        const std::string& game
    );

    PKMN_API int game_enum_to_generation(pkmn::e_game game);

    PKMN_API void initialize_database_connection();

    PKMN_API std::string convert_image_for_mono(
        const std::string& filepath
    );

    // TODO: move public
    PKMN_API pkmn::e_item ball_to_item(pkmn::e_ball ball);
    PKMN_API pkmn::e_ball item_to_ball(pkmn::e_item item);
}}

#endif /* INCLUDED_PKMN_PRIVATE_FUNCTIONS_HPP */

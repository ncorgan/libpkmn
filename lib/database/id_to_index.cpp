/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "id_to_index.hpp"

// TODO: database sptr

namespace pkmn { namespace database {

    int get_game_id(
        int game_index
    ) {
        (void)game_index;
        return 0;
    }

    int get_game_index(
        int game_id
    ) {
        (void)game_id;
        return 0;
    }

    int get_item_id(
        int item_index,
        int game_id
    ) {
        (void)item_index;
        (void)game_id;
        return 0;
    }

    int get_item_index(
        int item_id,
        int game_id
    ) {
        (void)item_id;
        (void)game_id;
        return 0;
    }

    int get_location_id(
        int location_index,
        int game_id
    ) {
        (void)location_index;
        (void)game_id;
        return 0;
    }

    int get_location_index(
        int location_id,
        int game_id
    ) {
        (void)location_id;
        (void)game_id;
        return 0;
    }

    int get_nature_id(
        int nature_index
    ) {
        (void)nature_index;
        return 0;
    }

    int get_nature_index(
        int nature_id
    ) {
        (void)nature_id;
        return 0;
    }

    int get_pokemon_id(
        int pokemon_index,
        int game_id
    ) {
        (void)pokemon_index;
        (void)game_id;
        return 0;
    }

    int get_pokemon_index(
        int pokemon_id,
        int game_id
    ) {
        (void)pokemon_id;
        (void)game_id;
        return 0;
    }

}}

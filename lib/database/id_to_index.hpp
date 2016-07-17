/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_DATABASE_ID_TO_INDEX_HPP
#define PKMN_DATABASE_ID_TO_INDEX_HPP

#include <string>

namespace pkmn { namespace database {

    /*
     * Games (Generation III+)
     */

    int get_game_id(
        int game_index
    );

    int get_game_index(
        int game_id
    );

    /*
     * Items
     */

    int get_item_id(
        int item_index,
        int game_id
    );

    int get_item_index(
        int item_id,
        int game_id
    );

    /*
     * Locations
     */

    int get_location_id(
        int location_index,
        int game_id
    );

    int get_location_index(
        int location_id,
        int game_id
    );

    /*
     * Natures
     */

    int get_nature_id(
        int nature_index
    );

    int get_nature_index(
        int nature_id
    );

    /*
     * Pokémon
     */

    int get_pokemon_id(
        int pokemon_index,
        int game_id
    );

    int get_pokemon_index(
        int pokemon_id,
        int game_id
    );

}}

#endif /* PKMN_DATABASE_ID_TO_INDEX_HPP */

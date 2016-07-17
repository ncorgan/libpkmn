/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_DATABASE_ID_TO_INDEX_HPP
#define PKMN_DATABASE_ID_TO_INDEX_HPP

#include <string>

/*
 * These functions convert between database ID's and their corresponding
 * in-game indices.
 */

namespace pkmn { namespace database {

    /*
     * Games (Generation III+)
     */

    int game_id_to_index(
        int game_id
    );

    int game_index_to_id(
        int game_index
    );

    /*
     * Items
     */

    int item_id_to_index(
        int item_id,
        int game_id
    );

    int item_index_to_id(
        int item_index,
        int game_id
    );

    /*
     * Locations
     */

    int location_id_to_index(
        int location_id,
        int game_id
    );

    int location_index_to_id(
        int location_index,
        int game_id
    );

    /*
     * Natures
     */

    int nature_id_to_index(
        int nature_id
    );

    int nature_index_to_id(
        int nature_index
    );

    /*
     * Pokémon
     */

    int pokemon_id_to_index(
        int pokemon_id,
        int game_id
    );

    int pokemon_index_to_id(
        int pokemon_index,
        int game_id
    );

}}

#endif /* PKMN_DATABASE_ID_TO_INDEX_HPP */

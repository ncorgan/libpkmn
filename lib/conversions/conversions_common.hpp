/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

namespace pkmn { namespace conversions {

    int convert_pokemon_game_index(
        int pokemon_index,
        int from_game_id,
        int to_game_id
    );

    int pokemon_index_to_base_friendship(
        int pokemon_index,
        int game_id
    );

}}

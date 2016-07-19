/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_DATABASE_INDEX_TO_STRING_HPP
#define PKMN_DATABASE_INDEX_TO_STRING_HPP

#include <string>

/*
 * These functions convert between in-game indices and their corresponding
 * strings.
 */

namespace pkmn { namespace database {

    /*
     * Games
     */

    std::string game_index_to_name(
        int game_index,
        int language_id = 9
    );

    int game_name_to_index(
        const std::string &game_name
    );

    /*
     * Items
     */

    std::string item_index_to_name(
        int item_index,
        int game_id,
        int language_id = 9
    );

    int item_name_to_index(
        const std::string &item_name,
        int game_id
    );

    /*
     * Locations
     */

    std::string location_index_to_name(
        int location_index,
        int game_id,
        int language_id = 9
    );

    int location_name_to_index(
        const std::string &location_name,
        int game_id
    );

    /*
     * Natures
     */

    std::string nature_index_to_name(
        int nature_index,
        int language_id = 9
    );

    int nature_name_to_index(
        const std::string &nature_name
    );

    /*
     * Pokémon
     */

    std::string pokemon_index_to_name(
        int pokemon_index,
        int game_id,
        int language_id = 9
    );

    int pokemon_name_to_index(
        const std::string &pokemon_name,
        const std::string &form_name,
        int game_id
    );

}}

#endif /* PKMN_DATABASE_INDEX_TO_STRING_HPP */

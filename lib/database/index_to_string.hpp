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
 *
 * Doing this in a single transaction is significantly faster than passing
 * *_index_to_id into *_id_to_name, and vice versa.
 */

namespace pkmn { namespace database {

    /*
     * Games
     */

    std::string game_index_to_name(
        int game_index
    );

    int game_name_to_index(
        const std::string &game_name
    );

    /*
     * Locations
     */

    std::string location_index_to_name(
        int location_index,
        int game_id
    );

    int location_name_to_index(
        const std::string &location_name,
        int game_id
    );

    /*
     * Natures
     */

    std::string nature_index_to_name(
        int nature_index
    );

    int nature_name_to_index(
        const std::string &nature_name
    );

}}

#endif /* PKMN_DATABASE_INDEX_TO_STRING_HPP */

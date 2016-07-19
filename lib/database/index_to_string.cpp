/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "database_common.hpp"
#include "id_to_string.hpp"

#include <boost/config.hpp>

// TODO: database sptr

namespace pkmn { namespace database {

    std::string game_index_to_name(
        int game_index,
        int language_index
    ) {
        (void)game_index;
        (void)language_index;
        return "";
    }

    int game_name_to_index(
        const std::string &game_name
    ) {
        static BOOST_CONSTEXPR const char* query = \
            "SELECT game_index FROM version_game_indices WHERE version_id="
            "(SELECT version_id FROM version_names WHERE name=?)";

        return pkmn_db_query_bind1<int, const std::string&>(
                   query, game_name
               );
    }

    std::string item_index_to_name(
        int item_index,
        int language_index
    ) {
        (void)item_index;
        (void)language_index;
        return "";
    }

    int item_name_to_index(
        const std::string &item_name
    ) {
        (void)item_name;
        return 0;
    }

    std::string location_index_to_name(
        int location_index,
        int language_index
    ) {
        (void)location_index;
        (void)language_index;
        return "";
    }

    int location_name_to_index(
        const std::string &location_name
    ) {
        (void)location_name;
        return 0;
    }

    std::string nature_index_to_name(
        int nature_index,
        int language_index
    ) {
        (void)nature_index;
        (void)language_index;
        return "";
    }

    int nature_name_to_index(
        const std::string &nature_name
    ) {
        static BOOST_CONSTEXPR const char* query = \
            "SELECT game_index FROM natures WHERE id="
            "(SELECT nature_id FROM nature_names WHERE name=?)";

        return pkmn_db_query_bind1<int, const std::string&>(
                   query, nature_name
               );
    }

    std::string pokemon_index_to_name(
        int pokemon_index,
        int language_index
    ) {
        (void)pokemon_index;
        (void)language_index;
        return "";
    }

    int pokemon_name_to_index(
        const std::string &pokemon_name
    ) {
        (void)pokemon_name;
        return 0;
    }

}}

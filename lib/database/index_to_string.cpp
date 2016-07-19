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
        int language_id
    ) {
        (void)game_index;
        (void)language_id;
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
        int game_id,
        int language_id
    ) {
        (void)item_index;
        (void)language_id;
        return "";
    }

    int item_name_to_index(
        const std::string &item_name,
        int game_id
    ) {
        (void)item_name;
        (void)game_id;
        return 0;
    }

    std::string location_index_to_name(
        int location_index,
        int game_id,
        int language_id
    ) {
        (void)location_index;
        (void)game_id;
        (void)language_id;
        return "";
    }

    int location_name_to_index(
        const std::string &location_name,
        int game_id
    ) {
        (void)location_name;
        (void)game_id;
        return 0;
    }

    std::string nature_index_to_name(
        int nature_index,
        int language_id
    ) {
        (void)nature_index;
        (void)language_id;
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
        int game_id,
        int language_id
    ) {
        (void)pokemon_index;
        (void)game_id;
        (void)language_id;
        return "";
    }

    int pokemon_name_to_index(
        const std::string &pokemon_name,
        const std::string &form_name,
        int game_id
    ) {
        (void)pokemon_name;
        (void)form_name;
        (void)game_id;
        return 0;
    }

}}

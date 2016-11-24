/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "database_common.hpp"
#include "id_to_string.hpp"

#include <boost/config.hpp>

namespace pkmn { namespace database {

    static pkmn::database::sptr _db;

    std::string game_index_to_name(
        int game_index
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM version_names WHERE local_language_id=9 AND "
            "version_id=(SELECT version_id IN version_game_indices WHERE "
            "game_index=?)";

        return pkmn::database::query_db_bind1<std::string, int>(
                   _db, query, game_index
               );
    }

    int game_name_to_index(
        const std::string &game_name
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT game_index FROM version_game_indices WHERE version_id="
            "(SELECT version_id FROM version_names WHERE name=?)";

        return pkmn::database::query_db_bind1<int, const std::string&>(
                   _db, query, game_name
               );
    }

    std::string item_index_to_name(
        int item_index,
        int game_id
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        (void)item_index;
        (void)game_id;
        return "";
    }

    int item_name_to_index(
        const std::string &item_name,
        int game_id
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        (void)item_name;
        (void)game_id;
        return 0;
    }

    std::string location_index_to_name(
        int location_index,
        int game_id
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        (void)location_index;
        (void)game_id;
        return "";
    }

    int location_name_to_index(
        const std::string &location_name,
        int game_id
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        (void)location_name;
        (void)game_id;
        return 0;
    }

    std::string nature_index_to_name(
        int nature_index
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM nature_names WHERE local_language_id=9 "
            "AND nature_id=(SELECT id FROM natures WHERE game_index=?)";

        return pkmn::database::query_db_bind1<std::string, int>(
                   _db, query, nature_index
               );
    }

    int nature_name_to_index(
        const std::string &nature_name
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT game_index FROM natures WHERE id="
            "(SELECT nature_id FROM nature_names WHERE name=?)";

        return pkmn::database::query_db_bind1<int, const std::string&>(
                   _db, query, nature_name
               );
    }

    std::string pokemon_index_to_name(
        int pokemon_index,
        int game_id
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM pokemon_species_names WHERE pokemon_species_id="
            "(SELECT species_id FROM pokemon WHERE id=(SELECT pokemon_id FROM "
            "pokemon_game_indices WHERE game_index=? AND version_id=?))";

        return pkmn::database::query_db_bind2<std::string, int, int>(
                   _db, query, pokemon_index, game_id
               );
    }

    int pokemon_name_to_index(
        const std::string &pokemon_name,
        const std::string &form_name,
        int game_id
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        if(form_name == "" or form_name == "Standard") {
            static BOOST_CONSTEXPR const char* query = \
                "SELECT game_index FROM pokemon_game_indices WHERE version_id=? "
                "AND pokemon_id=(SELECT pokemon_species_id FROM pokemon_species_names "
                "WHERE name=?)";

            return pkmn::database::query_db_bind2<int, int, const std::string&>(
                       _db, query, game_id, pokemon_name
                   );
        } else {
            static BOOST_CONSTEXPR const char* query = \
                "SELECT game_index FROM pokemon_game_indices WHERE version_id=? "
                "AND pokemon_id=(SELECT pokemon_id FROM pokemon_forms INNER JOIN "
                "pokemon ON (pokemon_forms.pokemon_id=pokemon.id) INNER JOIN "
                "libpkmn_pokemon_form_names ON "
                "(pokemon_forms.id=libpkmn_pokemon_form_names.form_id) WHERE "
                "libpkmn_pokemon_form_names.name=? AND pokemon.id IN (SELECT id "
                "FROM pokemon WHERE pokemon_species_id=(SELECT pokemon_species_id "
                "FROM pokemon_species_names WHERE name=?)))";

            return pkmn::database::query_db_bind2<int, int, const std::string&>(
                       _db, query, game_id, pokemon_name
                   );
        }
    }

}}

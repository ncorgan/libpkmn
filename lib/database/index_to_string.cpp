/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "../misc_common.hpp"

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

    static PKMN_CONSTEXPR_OR_INLINE bool GAME_IS_SAPPHIRE(int game_id) {return (game_id == 8);}
    static PKMN_CONSTEXPR_OR_INLINE bool GAME_IS_RSE     (int game_id) {return (game_id >= 7 and game_id <= 9);}
    static PKMN_CONSTEXPR_OR_INLINE bool GAME_IS_E       (int game_id) {return (game_id == 9);}
    static PKMN_CONSTEXPR_OR_INLINE bool GAME_IS_DP      (int game_id) {return (game_id == 12 or game_id == 13);}
    static PKMN_CONSTEXPR_OR_INLINE bool GAME_IS_B2W2    (int game_id) {return (game_id == 21 or game_id == 22);}

    BOOST_STATIC_CONSTEXPR int COLOSSEUM = 19;

    std::string location_index_to_name(
        int location_index,
        int game_id
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        if(game_is_gamecube(game_id)) {
            static BOOST_CONSTEXPR const char* query = \
                "SELECT name FROM location_names WHERE local_language_id=9 AND location_id=(SELECT "
                "location_id FROM gamecube_location_index_ranges WHERE range_start<=? AND "
                "colosseum=? ORDER BY range_start DESC)";

            return pkmn::database::query_db_bind2<std::string, int, int>(
                       _db, query, location_index, ((game_id == COLOSSEUM) ? 1 : 0)
                   );
        } else {
            /*
             * Veekun's database stores location indices by generation, but some version
             * groups within a generation overlap indices. This solution isn't ideal, but
             * it's needed to work around how the database works.
             */
            if(GAME_IS_DP(game_id) and location_index == 108) {
                return "Cafe";
            } else if(GAME_IS_B2W2(game_id) and location_index == 36) {
                return "PWT";
            } else if((GAME_IS_SAPPHIRE(game_id) and location_index == 486) or
                      (GAME_IS_E(game_id) and location_index == 197)) {
                return "Aqua Hideout";
            } else if(GAME_IS_RSE(game_id) and location_index == 87) {
                return "Ferry";
            } else {
                int generation = pkmn::database::game_id_to_generation(game_id);

                static BOOST_CONSTEXPR const char* query = \
                    "SELECT name FROM location_names WHERE local_language_id=9 AND location_id="
                    "(SELECT location_id FROM location_game_indices WHERE game_index=? AND "
                    "generation_id=?)";

                return pkmn::database::query_db_bind2<std::string, int, int>(
                           _db, query, location_index, generation
                       );
            }
        }
    }

    int location_name_to_index(
        const std::string &location_name,
        int game_id
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        if(game_is_gamecube(game_id)) {
            static BOOST_CONSTEXPR const char* query = \
                "SELECT max(range_start) FROM gamecube_location_index_ranges WHERE colosseum=? AND "
                "location_id=(SELECT location_id FROM location_names WHERE name=?)";

            return pkmn::database::query_db_bind2<int, int, const std::string&>(
                       _db, query, ((game_id == COLOSSEUM) ? 1 : 0), location_name
                   );
        } else {
            /*
             * Veekun's database stores location indices by generation, but some version
             * groups within a generation overlap indices. This solution isn't ideal, but
             * it's needed to work around how the database works.
             */
            if(GAME_IS_DP(game_id) and location_name == "Cafe") {
                return 108;
            } else if(GAME_IS_B2W2(game_id) and location_name == "PWT") {
                return 36;
            } else if(GAME_IS_SAPPHIRE(game_id) and location_name == "Aqua Hideout") {
                return 486;
            } else if(GAME_IS_E(game_id) and location_name == "Aqua Hideout") {
                return 197;
            } else if(GAME_IS_RSE(game_id) and location_name == "Ferry") {
                return 87;
            } else {
                int generation = pkmn::database::game_id_to_generation(game_id);

                static BOOST_CONSTEXPR const char* query = \
                    "SELECT game_index FROM location_game_indices WHERE generation_id=? AND "
                    "location_id=(SELECT location_id FROM location_names WHERE name=?)";

                return pkmn::database::query_db_bind2<int, int, const std::string&>(
                           _db, query, generation, location_name
                       );
            }
        }
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
                "FROM pokemon WHERE species_id=(SELECT pokemon_species_id FROM "
                "pokemon_species_names WHERE name=?)))";

            return pkmn::database::query_db_bind3<int, int, const std::string&, const std::string&>(
                       _db, query, game_id, form_name, pokemon_name
                   );
        }
    }

}}

/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "../misc_common.hpp"

#include "database_common.hpp"
#include "id_to_index.hpp"
#include "id_to_string.hpp"

#include <pkmn/config.hpp>

#include <boost/config.hpp>

#include <stdexcept>

namespace pkmn { namespace database {

    static pkmn::database::sptr _db;

    std::string game_index_to_name(
        int game_index
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM version_names WHERE local_language_id=9 AND "
            "version_id=(SELECT version_id FROM version_game_indices WHERE "
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

    /*
     * Calling the id_to_string and id_to_index functions is not ideal, but the logic in
     * those functions is complicated enough to warrant not duplicating code.
     */

    std::string item_index_to_name(
        int item_index,
        int game_id
    )
    {
        return pkmn::database::item_id_to_name(
                   pkmn::database::item_index_to_id(item_index, game_id),
                   pkmn::database::game_id_to_version_group(game_id)
               );
    }

    int item_name_to_index(
        const std::string& item_name,
        int game_id
    )
    {
        return pkmn::database::item_id_to_index(
                   pkmn::database::item_name_to_id(item_name),
                   game_id
               );
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

        std::string ret;

        if(game_is_gamecube(game_id)) {
            static BOOST_CONSTEXPR const char* query = \
                "SELECT name FROM location_names WHERE local_language_id=9 AND location_id=(SELECT "
                "location_id FROM gamecube_location_index_ranges WHERE range_start<=? AND "
                "colosseum=? ORDER BY range_start DESC)";

            ret = pkmn::database::query_db_bind2<std::string, int, int>(
                      _db, query, location_index, ((game_id == COLOSSEUM) ? 1 : 0)
                  );
        } else {
            /*
             * Veekun's database stores location indices by generation, but some version
             * groups within a generation overlap indices. This solution isn't ideal, but
             * it's needed to work around how the database works.
             */
            if(GAME_IS_DP(game_id) and location_index == 108) {
                ret = "Cafe";
            } else if(GAME_IS_B2W2(game_id) and location_index == 36) {
                ret = "PWT";
            } else if((GAME_IS_SAPPHIRE(game_id) and location_index == 486) or
                      (GAME_IS_E(game_id) and location_index == 197)) {
                ret = "Aqua Hideout";
            } else if(GAME_IS_RSE(game_id) and location_index == 87) {
                ret = "Ferry";
            } else {
                int generation = pkmn::database::game_id_to_generation(game_id);

                static BOOST_CONSTEXPR const char* query = \
                    "SELECT name FROM location_names WHERE local_language_id=9 AND location_id="
                    "(SELECT location_id FROM location_game_indices WHERE game_index=? AND "
                    "generation_id=?)";

                ret = pkmn::database::query_db_bind2<std::string, int, int>(
                          _db, query, location_index, generation
                      );
            }
        }

        return ret;
    }

    int location_name_to_index(
        const std::string &location_name,
        int game_id
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        int ret = 0;

        std::string error_message = "Invalid location: ";
        error_message += location_name;

        if(game_is_gamecube(game_id)) {
            static BOOST_CONSTEXPR const char* query = \
                "SELECT max(range_start) FROM gamecube_location_index_ranges WHERE colosseum=? AND "
                "location_id=(SELECT location_id FROM location_names WHERE name=?)";

            ret = pkmn::database::query_db_bind2<int, int, const std::string&>(
                      _db, query, ((game_id == COLOSSEUM) ? 1 : 0), location_name,
                      error_message
                  );

            /*
             * Even if the value is not in the gamecube_location_index_ranges table, it will
             * still return 0.
             */
            if(ret == 0) {
                throw std::invalid_argument(error_message);
            }
        } else {
            /*
             * Veekun's database stores location indices by generation, but some version
             * groups within a generation overlap indices. This solution isn't ideal, but
             * it's needed to work around how the database works.
             */
            if(GAME_IS_DP(game_id) and location_name == "Cafe") {
                ret = 108;
            } else if(GAME_IS_B2W2(game_id) and location_name == "PWT") {
                ret = 36;
            } else if(GAME_IS_SAPPHIRE(game_id) and location_name == "Aqua Hideout") {
                ret = 486;
            } else if(GAME_IS_E(game_id) and location_name == "Aqua Hideout") {
                ret = 197;
            } else if(GAME_IS_RSE(game_id) and location_name == "Ferry") {
                ret = 87;
            } else {
                int generation = pkmn::database::game_id_to_generation(game_id);

                static BOOST_CONSTEXPR const char* query = \
                    "SELECT game_index FROM location_game_indices WHERE generation_id=? AND "
                    "location_id=(SELECT location_id FROM location_names WHERE name=?)";

                ret = pkmn::database::query_db_bind2<int, int, const std::string&>(
                          _db, query, generation, location_name, error_message
                      );
            }
        }

        return ret;
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

        std::string error_message = "Invalid nature: ";
        error_message += nature_name;

        return pkmn::database::query_db_bind1<int, const std::string&>(
                   _db, query, nature_name
               );
    }

}}

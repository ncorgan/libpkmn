/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "../misc_common.hpp"
#include "database_common.hpp"
#include "id_to_string.hpp"

#include <pkmn/exception.hpp>
#include <pkmn/database/lists.hpp>

#include <boost/algorithm/string/compare.hpp>
#include <boost/assign.hpp>
#include <boost/config.hpp>

#include <stdexcept>

namespace pkmn { namespace database {

    static pkmn::database::sptr _db;

    std::vector<std::string> get_ability_list(
        int generation
    ) {
        if(generation < 3 or generation > 6) {
            throw pkmn::range_error("generation", 3, 6);
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM ability_names WHERE local_language_id=9 AND "
            "ability_id IN (SELECT id FROM abilities WHERE generation_id<=? "
            "AND is_main_series=1) ORDER BY name";

        std::vector<std::string> ret;
        pkmn::database::query_db_list_bind1<std::string, int>(
            _db, query, ret, generation
        );

        return ret;
    }

    std::vector<std::string> get_game_list(
        int generation,
        bool include_previous
    ) {
        if(generation < 1 or generation > 6) {
            throw pkmn::range_error("generation", 1, 6);
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* with_previous_query = \
            "SELECT name FROM version_names WHERE local_language_id=9 AND version_id IN "
            "(SELECT id FROM versions WHERE version_group_id IN "
            "(SELECT id FROM version_groups WHERE generation_id<=?))";

        static BOOST_CONSTEXPR const char* no_previous_query = \
            "SELECT name FROM version_names WHERE local_language_id=9 AND version_id IN "
            "(SELECT id FROM versions WHERE version_group_id IN "
            "(SELECT id FROM version_groups WHERE generation_id=?))";

        std::vector<std::string> ret;
        pkmn::database::query_db_list_bind1<std::string, int>(
            _db,
            (include_previous ? with_previous_query : no_previous_query),
            ret, generation
        );

        return ret;
    }

    std::vector<std::string> get_item_list(
        const std::string &game
    ) {
        int game_id = pkmn::database::game_name_to_id(game);

        std::vector<std::string> ret;
        pkmn::database::_get_item_list(
            ret, -1, game_id
        );

        return ret;
    }

    /*
     * There is unfortunately a lot of manual work necessary here because the Veekun database
     * stores location game indices by generation and not by game. This is generally fine because
     * there is almost never any overlap in location index, but some games in the same region have
     * version-exclusive indices, so we need to detect these and filter them out.
     */

    BOOST_STATIC_CONSTEXPR int version_group_region_ids[][2] = {
        {0,0}, // None
        {1,1}, // Red/Blue
        {1,1}, // Yellow
        {2,1}, // Gold/Silver
        {2,1}, // Crystal
        {3,3}, // Ruby/Sapphire
        {3,3}, // Emerald
        {1,1}, // FR/LG
        {4,4}, // D/P
        {4,4}, // Platinum
        {2,1}, // HG/SS
        {5,5}, // B/W
        {0,0}, // Colosseum
        {0,0}, // XD
        {5,5}, // B2/W2
        {6,6}, // X/Y
        {3,3}, // OR/AS
    };

    PKMN_CONSTEXPR_OR_INLINE bool version_group_has_single_region(
        int version_group_id
    ) {
        return (version_group_region_ids[version_group_id][0] == \
                version_group_region_ids[version_group_id][1]);
    }

    BOOST_STATIC_CONSTEXPR int version_group_location_index_bounds[][4][2] = {
        {{0,0},{0,0},{0,0},{0,0}}, // None
        {{0,0},{0,0},{0,0},{0,0}}, // Red/Blue
        {{0,0},{0,0},{0x0},{0,0}}, // Yellow
        {{0x01,0x0C},{0x1E,0x7F},{0,0}}, // Gold/Silver
        {{0x01,0x7F},{0,0},{0,0},{0,0}}, // Crystal
        {{0,87},{254,255},{0,0},{0,0}}, // Ruby/Sapphire
        {{0,87},{196,255},{0,0},{0,0}}, // Emerald
        {{88,196},{254,255},{0,0},{0,0}}, // FR/LG
        {{0,111},{2000,2010},{2012,2012},{3000,3076}}, // D/P
        {{0,125},{2000,2012},{3000,3076},{0,0}}, // Platinum
        {{126,2010},{2012,2012},{3000,3076},{0,0}}, // HG/SS
        {{0,116},{30001,30014},{40000,60003},{0,0}}, // B/W
        {{0,0},{0,0},{0,0},{0,0}}, // Colosseum
        {{0,0},{0,0},{0,0},{0,0}}, // XD
        {{0,60003},{0,0},{0,0},{0,0}}, // B2/W2
        {{0,168},{30001,60003},{0,0},{0,0}}, // X/Y
        {{170,60003},{0,0},{0,0},{0,0}} // OR/AS
    };

    BOOST_STATIC_CONSTEXPR int num_ranges_in_version_group[] = {
        0, // None
        0, // Red/Blue
        0, // Yellow
        3, // Gold/Silver
        1, // Crystal
        2, // Ruby/Sapphire
        2, // Emerald
        2, // FR/LG
        4, // D/P
        3, // Platinum
        3, // HG/SS
        3, // B/W
        0, // Colosseum
        0, // XD
        1, // B2/W2
        2, // X/Y
        2  // OR/AS
    };

    static const std::vector<std::string> location_names_to_fix = boost::assign::list_of
        ("Café")("Cold Storage")("Magma Hideout")
        ("Battle Tower")("S.S. Tidal")("Team Aqua Hideout")
    ;

    static PKMN_INLINE void fix_location_vector(
        std::vector<std::string> &ret,
        int game_id,
        bool whole_generation
    ) {
        std::vector<size_t> to_erase;

        for(size_t i = 0; i < ret.size(); ++i) {
            if(std::find(
                   location_names_to_fix.begin(), location_names_to_fix.end(), ret[i]
               ) != location_names_to_fix.end()
            ) {
                int location_id = pkmn::database::location_name_to_id(ret[i]);
                bool different_found = true;
                bool different_applies = true;
                std::string new_string = pkmn::database::alternate_location_string(
                                             ret[i], location_id, game_id,
                                             whole_generation, &different_found,
                                             &different_applies
                                         );

                if(different_found) {
                    if(whole_generation) {
                        ret.emplace_back(new_string);
                    } else if(different_applies) {
                        if(new_string.empty()) {
                            to_erase.emplace_back(i);
                        } else {
                            ret[i] = new_string;
                        }
                    }
                }
            }
        }

        if(not to_erase.empty()) {
            for(size_t i = (to_erase.size()-1); i < to_erase.size(); --i) {
                ret.erase(ret.begin() + to_erase[i]);
            }
        }
    }

    std::vector<std::string> get_location_list(
        const std::string &game,
        bool whole_generation
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        int game_id          = game_name_to_id(game);
        int version_group_id = game_id_to_version_group(game_id);
        int generation       = game_id_to_generation(game_id);
        std::vector<std::string> ret;

        // TODO: add Generation I locations
        if(generation == 1) {
            return ret;
        }

        if(game_is_gamecube(game_id)) {
            BOOST_STATIC_CONSTEXPR int COLOSSEUM = 19;

            static BOOST_CONSTEXPR const char* query = \
                "SELECT name FROM location_names WHERE local_language_id=9 AND "
                "location_id IN (SELECT DISTINCT location_id FROM gamecube_location_index_ranges "
                "WHERE colosseum IN (?,?))";

            pkmn::database::query_db_list_bind2<std::string, int>(
                _db, query, ret,
                (whole_generation ? 0 : ((game_id == COLOSSEUM) ? 1 : 0)),
                (whole_generation ? 1 : ((game_id == COLOSSEUM) ? 1 : 0))
            );
        } else {
            if(whole_generation) {
                static BOOST_CONSTEXPR const char* query = \
                    "SELECT name FROM location_names WHERE local_language_id=9 "
                    "AND location_id IN (SELECT locations.id FROM locations "
                    "INNER JOIN location_game_indices ON "
                    "(locations.id=location_game_indices.location_id) WHERE "
                    "location_game_indices.generation_id=?)";

                pkmn::database::query_db_list_bind1<std::string, int>(
                    _db, query, ret, generation
                );
            } else {
                /*
                 * Figure out how many ranges of item indices there are and form the query
                 * from there. This looks ugly and verbose, but a constexpr array of strings
                 * is much faster than dynamically forming the query.
                 */
                static BOOST_CONSTEXPR const char* queries[] = {
                    "",

                    "SELECT name FROM location_names WHERE local_language_id=9 "
                    "AND location_id IN (SELECT locations.id FROM locations "
                    "INNER JOIN location_game_indices ON "
                    "(locations.id=location_game_indices.location_id) WHERE "
                    "location_game_indices.generation_id=? AND (locations.region_id=? OR locations.region_id IS NULL) "
                    "AND (location_game_indices.game_index>=? AND location_game_indices.game_index<=?))",

                    "SELECT name FROM location_names WHERE local_language_id=9 "
                    "AND location_id IN (SELECT locations.id FROM locations "
                    "INNER JOIN location_game_indices ON "
                    "(locations.id=location_game_indices.location_id) WHERE "
                    "location_game_indices.generation_id=? AND (locations.region_id=? OR locations.region_id IS NULL) "
                    "AND ((location_game_indices.game_index>=? AND location_game_indices.game_index<=?) OR"
                    "     (location_game_indices.game_index>=? AND location_game_indices.game_index<=?)))",

                    "SELECT name FROM location_names WHERE local_language_id=9 "
                    "AND location_id IN (SELECT locations.id FROM locations "
                    "INNER JOIN location_game_indices ON "
                    "(locations.id=location_game_indices.location_id) WHERE "
                    "location_game_indices.generation_id=? AND (locations.region_id=? OR locations.region_id IS NULL) "
                    "AND ((location_game_indices.game_index>=? AND location_game_indices.game_index<=?) OR"
                    "     (location_game_indices.game_index>=? AND location_game_indices.game_index<=?) OR"
                    "     (location_game_indices.game_index>=? AND location_game_indices.game_index<=?)))",

                    "SELECT name FROM location_names WHERE local_language_id=9 "
                    "AND location_id IN (SELECT locations.id FROM locations "
                    "INNER JOIN location_game_indices ON "
                    "(locations.id=location_game_indices.location_id) WHERE "
                    "location_game_indices.generation_id=? AND (locations.region_id=? OR locations.region_id IS NULL) "
                    "AND ((location_game_indices.game_index>=? AND location_game_indices.game_index<=?) OR"
                    "     (location_game_indices.game_index>=? AND location_game_indices.game_index<=?) OR"
                    "     (location_game_indices.game_index>=? AND location_game_indices.game_index<=?) OR"
                    "     (location_game_indices.game_index>=? AND location_game_indices.game_index<=?)))",
                };

                for(int i = 0; i < (version_group_has_single_region(version_group_id) ? 1 : 2); ++i) {
                    SQLite::Statement stmt((*_db), queries[num_ranges_in_version_group[version_group_id]]);
                    stmt.bind(1, generation);
                    stmt.bind(2, version_group_region_ids[version_group_id][i]);

                    stmt.bind(3, version_group_location_index_bounds[version_group_id][0][0]);
                    stmt.bind(4, version_group_location_index_bounds[version_group_id][0][1]);
                    if(num_ranges_in_version_group[version_group_id] > 1) {
                        stmt.bind(5, version_group_location_index_bounds[version_group_id][1][0]);
                        stmt.bind(6, version_group_location_index_bounds[version_group_id][1][1]);
                    }
                    if(num_ranges_in_version_group[version_group_id] > 2) {
                        stmt.bind(7, version_group_location_index_bounds[version_group_id][2][0]);
                        stmt.bind(8, version_group_location_index_bounds[version_group_id][2][1]);
                    }
                    if(num_ranges_in_version_group[version_group_id] > 3) {
                        stmt.bind(9, version_group_location_index_bounds[version_group_id][3][0]);
                        stmt.bind(10, version_group_location_index_bounds[version_group_id][3][1]);
                    }

                    while(stmt.executeStep()) {
                        ret.push_back((const char*)stmt.getColumn(0));
                    }
                }
            }
        }

        fix_location_vector(ret, game_id, whole_generation);
        std::sort(ret.begin(), ret.end(), boost::algorithm::is_less());
        return ret;
    }

    std::vector<std::string> get_move_list(
        const std::string &game
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* main_query = \
            "SELECT name FROM move_names WHERE local_language_id=9 AND "
            "move_id IN (SELECT id FROM moves WHERE generation_id<=? AND "
            "type_id<100) ORDER BY move_id";

        int generation = game_name_to_generation(game);

        std::vector<std::string> ret;
        pkmn::database::query_db_list_bind1<std::string, int>(
            _db, main_query, ret, generation
        );

        BOOST_STATIC_CONSTEXPR int COLOSSEUM = 19;
        BOOST_STATIC_CONSTEXPR int XD        = 20;
        BOOST_STATIC_CONSTEXPR int X         = 23;
        BOOST_STATIC_CONSTEXPR int Y         = 24;

        /*
         * Shadow Pokémon in Colosseum only know Shadow Rush, and XD brings in
         * other Shadow moves, so only bring in the relevant Shadow moves in
         * these cases.
         *
         * There are also four Generation VI moves that are exclusive to Omega
         * Ruby and Alpha Sapphire, so get rid of those for X/Y.
         */
        int game_id = game_name_to_id(game);
        if(game_id == COLOSSEUM) {
            static BOOST_CONSTEXPR const char* shadow_query = \
                "SELECT name FROM move_names WHERE local_language_id=9 AND "
                "move_id=10001";

            pkmn::database::query_db_list<std::string>(
                _db, shadow_query, ret
            );
        } else if(game_id == XD) {
            static BOOST_CONSTEXPR const char* shadow_query = \
                "SELECT name FROM move_names WHERE local_language_id=9 AND "
                "move_id IN (SELECT id FROM moves WHERE type_id=10002) "
                "ORDER BY move_id";

            pkmn::database::query_db_list<std::string>(
                _db, shadow_query, ret
            );
        } else if(game_id == X or game_id == Y) {
            for(size_t i = 0; i < 4; ++i) {
                ret.pop_back();
            }
        }

        return ret;
    }

    std::vector<std::string> get_nature_list() {
        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT nature_names.name FROM nature_names INNER JOIN natures ON "
            "(nature_names.nature_id=natures.id) WHERE nature_names.local_language_id=9 "
            "ORDER BY natures.game_index";

        std::vector<std::string> ret;
        pkmn::database::query_db_list<std::string>(
            _db, query, ret
        );

        return ret;
    }

    std::vector<std::string> get_pokemon_list(
        int generation,
        bool include_previous
    ) {
        if(generation < 1 or generation > 6) {
            throw pkmn::range_error("generation", 1, 6);
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* with_previous_query = \
            "SELECT name FROM pokemon_species_names WHERE local_language_id=9 AND "
            "pokemon_species_id IN (SELECT id FROM pokemon_species WHERE generation_id<=?) "
            "ORDER BY pokemon_species_id";

        static BOOST_CONSTEXPR const char* no_previous_query = \
            "SELECT name FROM pokemon_species_names WHERE local_language_id=9 AND "
            "pokemon_species_id IN (SELECT id FROM pokemon_species WHERE generation_id=?) "
            "ORDER BY pokemon_species_id";

        std::vector<std::string> ret;
        pkmn::database::query_db_list_bind1<std::string, int>(
            _db,
            (include_previous ? with_previous_query : no_previous_query),
            ret, generation
        );

        return ret;
    }

    std::vector<std::string> get_region_list() {
        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM region_names WHERE local_language_id=9 "
            "AND region_id<=6 ORDER BY region_id";

        std::vector<std::string> ret;
        pkmn::database::query_db_list<std::string>(
            _db, query, ret
        );
        ret.insert(ret.begin()+3, "Orre");

        return ret;
    }

    std::vector<std::string> get_ribbon_list(
        int generation
    ) {
        (void)generation;
        return std::vector<std::string>();
    }

    std::vector<std::string> get_super_training_medal_list() {
        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM ribbons_medals WHERE super_training_medal=1";

        std::vector<std::string> ret;
        pkmn::database::query_db_list<std::string>(
            _db, query, ret
        );

        return ret;
    }

    std::vector<std::string> get_type_list(
        const std::string &game
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        int generation = game_name_to_generation(game);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM type_names WHERE local_language_id=9 AND "
            "type_id IN (SELECT id FROM types WHERE generation_id<=?) "
            "AND type_id<100 ORDER BY type_id";

        std::vector<std::string> ret;
        pkmn::database::query_db_list_bind1<std::string, int>(
            _db, query, ret, generation
        );

        BOOST_STATIC_CONSTEXPR int GOLD   = 4;
        BOOST_STATIC_CONSTEXPR int SS     = 16;

        int game_id = game_name_to_id(game);

        // The Shadow type only exists in the Gamecube games
        if(game_is_gamecube(game_id)) {
            ret.emplace_back("Shadow");
        }

        // The ??? type only exists in Generations II-II
        if((game_id >= GOLD and game_id <= SS) or game_is_gamecube(game_id)) {
            ret.emplace_back("???");
        }

        return ret;
    }
}}

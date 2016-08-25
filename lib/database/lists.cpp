/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "database_common.hpp"
#include "id_to_string.hpp"

#include <pkmn/database/lists.hpp>

#include <boost/config.hpp>

#include <stdexcept>

#ifdef PKMN_SQLITE_DEBUG
#include <iostream>
#endif

namespace pkmn { namespace database {

    static pkmn::database::sptr _db;

    std::vector<std::string> get_ability_list(
        int generation
    ) {
        if(generation < 3 or generation > 6) {
            throw std::out_of_range("generation: valid range 3-6");
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

        return (ret);
    }

    std::vector<std::string> get_game_list(
        int generation,
        bool include_previous
    ) {
        if(generation < 1 or generation > 6) {
            throw std::out_of_range("generation: valid range 1-6");
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

        return (ret);
    }

    std::vector<std::string> get_item_list(
        const std::string &game
    ) {
        int game_id = pkmn::database::game_name_to_id(game);

        std::vector<std::string> ret;
        pkmn::database::_get_item_list(
            ret, -1, game_id
        );

        return (ret);
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

    std::vector<std::string> get_location_list(
        const std::string &game,
        bool whole_generation
    ) {
        (void)game;
        (void)whole_generation;

        return std::vector<std::string>();
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

        /*
         * Shadow Pokémon in Colosseum only know Shadow Rush, and XD brings in
         * other Shadow moves, so only bring in the relevant Shadow moves in
         * these cases.
         */
        int game_id = game_name_to_id(game);
        if(game_id == 19) {
            static BOOST_CONSTEXPR const char* shadow_query = \
                "SELECT name FROM move_names WHERE local_language_id=9 AND "
                "move_id=10001";

            pkmn::database::query_db_list<std::string>(
                _db, shadow_query, ret
            );
        } else if(game_id == 20) {
            static BOOST_CONSTEXPR const char* shadow_query = \
                "SELECT name FROM move_names WHERE local_language_id=9 AND "
                "move_id IN (SELECT id FROM moves WHERE type_id=10002) "
                "ORDER BY move_id";

            pkmn::database::query_db_list<std::string>(
                _db, shadow_query, ret
            );
        }

        return (ret);
    }

    std::vector<std::string> get_nature_list() {
        return std::vector<std::string>();
    }

    std::vector<std::string> get_pokemon_list(
        int generation,
        bool include_previous
    ) {
        if(generation < 1 or generation > 6) {
            throw std::out_of_range("generation: valid range 1-6");
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

        return (ret);
    }

    std::vector<std::string> get_region_list() {
        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM region_names WHERE local_language_id=9 "
            "ORDER BY region_id";

        std::vector<std::string> ret;
        pkmn::database::query_db_list<std::string>(
            _db, query, ret
        );

        return (ret);
    }

    std::vector<std::string> get_ribbon_list(
        int generation
    ) {
        (void)generation;
        return std::vector<std::string>();
    }

    std::vector<std::string> get_super_training_medal_list() {
        return std::vector<std::string>();
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

        // The Shadow type only exists in the Gamecube games
        int game_id = game_name_to_id(game);
        if(game_is_gamecube(game_id)) {
            ret.emplace_back("Shadow");
        }

        return (ret);
    }
}}

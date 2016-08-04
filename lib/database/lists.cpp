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

    template <typename ret_type>
    static void pkmn_db_query_list(
        const char* query,
        std::vector<ret_type> &ret_vec
    ) {
#ifdef PKMN_SQLITE_DEBUG
        std::cout << "Query: \"" << query << "\"" << std::endl
                  << " * Results: " << std::flush;
#endif
        SQLite::Statement stmt((*_db), query);
        while(stmt.executeStep()) {
#ifdef PKMN_SQLITE_DEBUG
            std::cout << (ret_type)stmt.getColumn(0) << ", " << std::flush;
#endif
            ret_vec.push_back((ret_type)stmt.getColumn(0));
        }
#ifdef PKMN_SQLITE_DEBUG
        std::cout << "\b\b " << std::endl;
#endif
    }

    template <typename ret_type, typename bind1_type>
    static void pkmn_db_query_list_bind1(
        const char* query,
        std::vector<ret_type> &ret_vec,
        bind1_type bind1
    ) {
#ifdef PKMN_SQLITE_DEBUG
        std::cout << "Query: \"" << query << "\"" << std::endl
                  << " * Bind " << bind1 << " to 1" << std::endl
                  << " * Results: " << std::flush;
#endif
        SQLite::Statement stmt((*_db), query);
        stmt.bind(1, (bind1_type)bind1);
        while(stmt.executeStep()) {
#ifdef PKMN_SQLITE_DEBUG
            std::cout << (ret_type)stmt.getColumn(0) << ", " << std::flush;
#endif
            ret_vec.push_back((ret_type)stmt.getColumn(0));
        }
#ifdef PKMN_SQLITE_DEBUG
        std::cout << "\b\b " << std::endl;
#endif
    }

    template <typename ret_type, typename bind1_type, typename bind2_type>
    static void pkmn_db_query_list_bind2(
        const char* query,
        std::vector<ret_type> &ret_vec,
        bind1_type bind1,
        bind2_type bind2
    ) {
#ifdef PKMN_SQLITE_DEBUG
        std::cout << "Query: \"" << query << "\"" << std::endl
                  << " * Bind " << bind1 << " to 1" << std::endl
                  << " * Bind " << bind2 << " to 2" << std::endl
                  << " * Results: " << std::flush;
#endif
        SQLite::Statement stmt((*_db), query);
        stmt.bind(1, (bind1_type)bind1);
        stmt.bind(2, (bind2_type)bind2);
        while(stmt.executeStep()) {
#ifdef PKMN_SQLITE_DEBUG
            std::cout << (ret_type)stmt.getColumn(0) << ", " << std::flush;
#endif
            ret_vec.push_back((ret_type)stmt.getColumn(0));
        }
#ifdef PKMN_SQLITE_DEBUG
        std::cout << "\b\b " << std::endl;
#endif
    }

    template <typename ret_type, typename bind1_type, typename bind2_type, typename bind3_type>
    static void pkmn_db_query_list_bind3(
        const char* query,
        std::vector<ret_type> &ret_vec,
        bind1_type bind1,
        bind1_type bind2,
        bind1_type bind3
    ) {
#ifdef PKMN_SQLITE_DEBUG
        std::cout << "Query: \"" << query << "\"" << std::endl
                  << " * Bind " << bind1 << " to 1" << std::endl
                  << " * Bind " << bind2 << " to 2" << std::endl
                  << " * Bind " << bind3 << " to 3" << std::endl
                  << " * Results: " << std::flush;
#endif
        SQLite::Statement stmt((*_db), query);
        stmt.bind(1, (bind1_type)bind1);
        stmt.bind(2, (bind2_type)bind2);
        stmt.bind(3, (bind3_type)bind3);
        while(stmt.executeStep()) {
#ifdef PKMN_SQLITE_DEBUG
            std::cout << (ret_type)stmt.getColumn(0) << ", " << std::flush;
#endif
            ret_vec.push_back((ret_type)stmt.getColumn(0));
        }
#ifdef PKMN_SQLITE_DEBUG
        std::cout << "\b\b " << std::endl;
#endif
    }

    void get_ability_list(
        int generation,
        std::vector<std::string> &abilities_out
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

        abilities_out.clear();
        pkmn_db_query_list_bind1<std::string, int>(
            query, abilities_out, generation
        );
    }

    void get_game_list(
        int generation,
        bool include_previous,
        std::vector<std::string> &games_out
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

        games_out.clear();
        pkmn_db_query_list_bind1<std::string, int>(
            (include_previous ? with_previous_query : no_previous_query),
            games_out, generation
        );
    }

    void get_item_list(
        const std::string &game,
        std::vector<std::string> &items_out
    ) {
        int game_id = pkmn::database::game_name_to_id(game);
        pkmn::database::_get_item_list(
            items_out, -1, game_id
        );
    }

    void get_location_list(
        const std::string &game,
        bool whole_generation,
        std::vector<std::string> &locations_out
    ) {
        (void)game;
        (void)whole_generation;
        (void)locations_out;
    }

    void get_move_list(
        const std::string &game,
        std::vector<std::string> &moves_out
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* main_query = \
            "SELECT name FROM move_names WHERE local_language_id=9 AND "
            "move_id IN (SELECT id FROM moves WHERE generation_id<=? AND "
            "type_id<100) ORDER BY move_id";

        moves_out.clear();
        int generation = game_name_to_generation(game);
        pkmn_db_query_list_bind1<std::string, int>(
            main_query, moves_out, generation
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

            pkmn_db_query_list<std::string>(
                shadow_query, moves_out
            );
        } else if(game_id == 20) {
            static BOOST_CONSTEXPR const char* shadow_query = \
                "SELECT name FROM move_names WHERE local_language_id=9 AND "
                "move_id IN (SELECT id FROM moves WHERE type_id=10002) "
                "ORDER BY move_id";

            pkmn_db_query_list<std::string>(
                shadow_query, moves_out
            );
        }
    }

    void get_nature_list(
        std::vector<std::string> &natures_out
    ) {
        (void)natures_out;
    }

    void get_pokemon_list(
        int generation,
        bool include_previous,
        std::vector<std::string> &pokemon_out
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

        pokemon_out.clear();
        pkmn_db_query_list_bind1<std::string, int>(
            (include_previous ? with_previous_query : no_previous_query),
            pokemon_out, generation
        );
    }

    void get_region_list(
        std::vector<std::string> &regions_out
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM region_names WHERE local_language_id=9 "
            "ORDER BY region_id";

        regions_out.clear();
        pkmn_db_query_list<std::string>(
            query, regions_out
        );
    }

    void get_ribbon_list(
        int generation,
        std::vector<std::string> &ribbons_out
    ) {
        (void)generation;
        (void)ribbons_out;
    }

    void get_super_training_medal_list(
        std::vector<std::string> &super_training_medals_out
    ) {
        super_training_medals_out.clear();
    }

    void get_type_list(
        const std::string &game,
        std::vector<std::string> &types_out
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        int generation = game_name_to_generation(game);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM type_names WHERE local_language_id=9 AND "
            "type_id IN (SELECT id FROM types WHERE generation_id<=?) "
            "AND type_id<100 ORDER BY type_id";

        types_out.clear();
        pkmn_db_query_list_bind1<std::string, int>(
            query, types_out, generation
        );

        // The Shadow type only exists in the Gamecube games
        int game_id = game_name_to_id(game);
        if(game_id == 19 or game_id == 20) {
            types_out.push_back("Shadow");
        }
    }
}}

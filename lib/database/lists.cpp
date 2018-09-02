/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "utils/misc.hpp"

#include "database_common.hpp"
#include "enum_conversions.hpp"
#include "id_to_string.hpp"

#include <pkmn/config.hpp>
#include <pkmn/exception.hpp>
#include <pkmn/database/lists.hpp>

#include <boost/algorithm/string/compare.hpp>
#include <boost/assign.hpp>
#include <boost/config.hpp>

#include <algorithm>
#include <unordered_map>

namespace pkmn { namespace database {

    std::vector<pkmn::e_ability> get_ability_list(
        int generation
    )
    {
        pkmn::enforce_bounds("generation", generation, 3, 6);

        static const std::string query =
            "SELECT id FROM abilities WHERE generation_id<=? AND is_main_series=1";

        std::vector<pkmn::e_ability> abilities;
        pkmn::database::query_db_enum_list_bind1(
            query,
            abilities,
            generation
        );

        return abilities;
    }

    std::vector<std::string> get_ability_name_list(
        int generation
    )
    {
        pkmn::enforce_bounds("generation", generation, 3, 6);

        static BOOST_CONSTEXPR const char* query =
            "SELECT name FROM ability_names WHERE local_language_id=9 AND "
            "ability_id IN (SELECT id FROM abilities WHERE generation_id<=? "
            "AND is_main_series=1) ORDER BY ability_id";

        std::vector<std::string> ret;
        pkmn::database::query_db_list_bind1<std::string, int>(
            query, ret, generation
        );

        return ret;
    }

    std::vector<pkmn::e_game> get_game_list(
        int generation,
        bool include_previous
    )
    {
        pkmn::enforce_bounds("generation", generation, 1, 6);

        static const std::string with_previous_query =
            "SELECT id FROM versions WHERE version_group_id IN "
            "(SELECT id FROM version_groups WHERE generation_id<=?)";

        static const std::string no_previous_query =
            "SELECT id FROM versions WHERE version_group_id IN "
            "(SELECT id FROM version_groups WHERE generation_id=?)";

        std::vector<int> game_ids;
        pkmn::database::query_db_list_bind1(
            include_previous ? with_previous_query.c_str() : no_previous_query.c_str(),
            game_ids,
            generation
        );

        // The games in Veekun's database aren't listed in a sensible order,
        // with the Gamecube games (Generation III) being listed with the
        // Generation V games. So we need to convert it to our order.
        std::vector<pkmn::e_game> games;
        for(int game_id: game_ids)
        {
            games.emplace_back(game_id_to_enum(game_id));
        }

        return games;
    }

    std::vector<std::string> get_game_name_list(
        int generation,
        bool include_previous
    )
    {
        pkmn::enforce_bounds("generation", generation, 1, 6);

        std::vector<pkmn::e_game> games = get_game_list(generation, include_previous);

        std::vector<std::string> game_names;
        for(pkmn::e_game game: games)
        {
            game_names.emplace_back(game_enum_to_name(game));
        }

        return game_names;
    }

    std::vector<pkmn::e_species> get_gamecube_shadow_pokemon_list(
        bool colosseum
    )
    {
        static const std::string query =
            "SELECT DISTINCT species_id FROM shadow_pokemon WHERE colosseum=? ORDER BY species_id";

        std::vector<pkmn::e_species> species;
        pkmn::database::query_db_enum_list_bind1(
            query,
            species,
            (colosseum ? 1 : 0)
        );

        return species;
    }

    std::vector<std::string> get_gamecube_shadow_pokemon_name_list(
        bool colosseum
    )
    {
        static BOOST_CONSTEXPR const char* shadow_pokemon_query =
            "SELECT name FROM pokemon_species_names WHERE local_language_id=9 AND pokemon_species_id IN "
            "(SELECT species_id FROM shadow_pokemon WHERE colosseum=?) ORDER BY pokemon_species_id";

        std::vector<std::string> ret;
        pkmn::database::query_db_list_bind1<std::string, int>(
            shadow_pokemon_query, ret, (colosseum ? 1 : 0)
        );
        return ret;
    }

    std::vector<pkmn::e_move> get_hm_move_list(
        pkmn::e_game game
    )
    {
        int game_id = game_enum_to_id(game);

        if(game_is_gamecube(game_id))
        {
            throw pkmn::feature_not_in_game_error(
                      "HMs",
                      game
                  );
        }

        static const std::string query =
            "SELECT move_names.move_id FROM move_names INNER JOIN machines ON (machines.move_id=move_names.move_id) "
            "WHERE machines.version_group_id=? AND machines.machine_number>=101 "
            "AND move_names.local_language_id=9 ORDER BY machines.machine_number";

        int version_group_id = game_id_to_version_group(game_id);

        std::vector<pkmn::e_move> hm_moves;
        pkmn::database::query_db_enum_list_bind1(
            query,
            hm_moves,
            version_group_id
        );
        return hm_moves;
    }

    std::vector<std::string> get_hm_move_name_list(
        pkmn::e_game game
    )
    {
        int game_id = game_enum_to_id(game);

        if(game_is_gamecube(game_id))
        {
            throw pkmn::feature_not_in_game_error(
                      "HMs",
                      game
                  );
        }

        static BOOST_CONSTEXPR const char* query =
            "SELECT name FROM move_names INNER JOIN machines ON (machines.move_id=move_names.move_id) "
            "WHERE machines.version_group_id=? AND machines.machine_number>=101 "
            "AND move_names.local_language_id=9 ORDER BY machines.machine_number";

        int version_group_id = game_id_to_version_group(game_id);

        std::vector<std::string> ret;
        pkmn::database::query_db_list_bind1<std::string, int>(
            query, ret, version_group_id
        );

        return ret;
    }

    std::vector<pkmn::e_item> get_item_list(
        pkmn::e_game game
    )
    {
        int game_id = pkmn::database::game_enum_to_id(game);

        std::vector<pkmn::e_item> enum_list;
        std::vector<std::string> name_list; // Unused

        pkmn::database::_get_item_lists(
            enum_list,
            name_list,
            -1, // list_id
            game_id,
            true, // should_populate_enum_list
            false // should_populate_string_list
        );

        return enum_list;
    }

    std::vector<std::string> get_item_name_list(
        pkmn::e_game game
    )
    {
        int game_id = pkmn::database::game_enum_to_id(game);

        std::vector<pkmn::e_item> enum_list; // Unused
        std::vector<std::string> name_list;

        pkmn::database::_get_item_lists(
            enum_list,
            name_list,
            -1, // list_id
            game_id,
            false, // should_populate_enum_list
            true   // should_populate_string_list
        );

        return name_list;
    }

    /*
     * There is unfortunately a lot of manual work necessary here because the Veekun database
     * stores location game indices by generation and not by game. This is generally fine because
     * there is almost never any overlap in location index, but some games in the same region have
     * version-exclusive indices, so we need to detect these and filter them out.
     */

    BOOST_STATIC_CONSTEXPR int version_group_region_ids[][2] =
    {
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
    )
    {
        return (version_group_region_ids[version_group_id][0] == \
                version_group_region_ids[version_group_id][1]);
    }

    BOOST_STATIC_CONSTEXPR int version_group_location_index_bounds[][4][2] =
    {
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

    BOOST_STATIC_CONSTEXPR int num_ranges_in_version_group[] =
    {
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

    static inline void fix_location_vector(
        std::vector<std::string> &ret,
        int game_id,
        bool whole_generation
    )
    {
        std::vector<size_t> to_erase;

        for(size_t i = 0; i < ret.size(); ++i)
        {
            if(std::find(
                   location_names_to_fix.begin(), location_names_to_fix.end(), ret[i]
               ) != location_names_to_fix.end()
            )
            {
                int location_id = pkmn::database::location_name_to_id(ret[i]);
                bool different_found = true;
                bool different_applies = true;
                std::string new_string = pkmn::database::alternate_location_string(
                                             ret[i], location_id, game_id,
                                             whole_generation, &different_found,
                                             &different_applies
                                         );

                if(different_found)
                {
                    if(whole_generation)
                    {
                        ret.emplace_back(new_string);
                    }
                    else if(different_applies)
                    {
                        if(new_string.empty())
                        {
                            to_erase.emplace_back(i);
                        }
                        else
                        {
                            ret[i] = new_string;
                        }
                    }
                }
            }
        }

        if(!to_erase.empty())
        {
            for(size_t i = (to_erase.size()-1); i < to_erase.size(); --i)
            {
                ret.erase(ret.begin() + to_erase[i]);
            }
        }
    }

    std::vector<std::string> get_location_name_list(
        pkmn::e_game game,
        bool whole_generation
    )
    {
        int game_id          = game_enum_to_id(game);
        int version_group_id = game_id_to_version_group(game_id);
        int generation       = game_id_to_generation(game_id);
        std::vector<std::string> ret;

        // TODO: add Generation I locations
        if(generation == 1)
        {
            return ret;
        }

        if(game_is_gamecube(game_id))
        {
            BOOST_STATIC_CONSTEXPR int COLOSSEUM = 19;

            static BOOST_CONSTEXPR const char* query =
                "SELECT name FROM location_names WHERE local_language_id=9 AND "
                "location_id IN (SELECT DISTINCT location_id FROM gamecube_location_index_ranges "
                "WHERE colosseum IN (?,?))";

            pkmn::database::query_db_list_bind2<std::string, int>(
                query, ret,
                (whole_generation ? 0 : ((game_id == COLOSSEUM) ? 1 : 0)),
                (whole_generation ? 1 : ((game_id == COLOSSEUM) ? 1 : 0))
            );
        }
        else
        {
            if(whole_generation)
            {
                static BOOST_CONSTEXPR const char* query =
                    "SELECT name FROM location_names WHERE local_language_id=9 "
                    "AND location_id IN (SELECT locations.id FROM locations "
                    "INNER JOIN location_game_indices ON "
                    "(locations.id=location_game_indices.location_id) WHERE "
                    "location_game_indices.generation_id=?)";

                pkmn::database::query_db_list_bind1<std::string, int>(
                    query, ret, generation
                );
            }
            else
            {
                /*
                 * Figure out how many ranges of item indices there are and form the query
                 * from there. This looks ugly and verbose, but a constexpr array of strings
                 * is much faster than dynamically forming the query.
                 */
                static BOOST_CONSTEXPR const char* queries[] =
                {
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

                for(int i = 0; i < (version_group_has_single_region(version_group_id) ? 1 : 2); ++i)
                {
                    SQLite::Statement stmt(
                        get_connection(),
                        queries[num_ranges_in_version_group[version_group_id]]
                    );
                    stmt.bind(1, generation);
                    stmt.bind(2, version_group_region_ids[version_group_id][i]);

                    stmt.bind(3, version_group_location_index_bounds[version_group_id][0][0]);
                    stmt.bind(4, version_group_location_index_bounds[version_group_id][0][1]);
                    if(num_ranges_in_version_group[version_group_id] > 1)
                    {
                        stmt.bind(5, version_group_location_index_bounds[version_group_id][1][0]);
                        stmt.bind(6, version_group_location_index_bounds[version_group_id][1][1]);
                    }
                    if(num_ranges_in_version_group[version_group_id] > 2)
                    {
                        stmt.bind(7, version_group_location_index_bounds[version_group_id][2][0]);
                        stmt.bind(8, version_group_location_index_bounds[version_group_id][2][1]);
                    }
                    if(num_ranges_in_version_group[version_group_id] > 3)
                    {
                        stmt.bind(9, version_group_location_index_bounds[version_group_id][3][0]);
                        stmt.bind(10, version_group_location_index_bounds[version_group_id][3][1]);
                    }

                    while(stmt.executeStep())
                    {
                        ret.push_back(static_cast<const char*>(stmt.getColumn(0)));
                    }
                }
            }
        }

        fix_location_vector(ret, game_id, whole_generation);
        std::sort(ret.begin(), ret.end(), boost::algorithm::is_less());
        return ret;
    }

    std::vector<pkmn::e_move> get_move_list(pkmn::e_game game)
    {
        static const std::string main_query =
            "SELECT id FROM moves WHERE generation_id<=? AND type_id<100";

        int game_id = game_enum_to_id(game);
        int generation = game_enum_to_generation(game);

        std::vector<pkmn::e_move> moves;
        pkmn::database::query_db_enum_list_bind1(
            main_query,
            moves,
            generation
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
        if(game_id == COLOSSEUM)
        {
            moves.emplace_back(pkmn::e_move::SHADOW_RUSH);
        }
        else if(game_id == XD)
        {
            static const std::string shadow_query =
                "SELECT id FROM moves WHERE type_id=10002";

            pkmn::database::query_db_enum_list(
                shadow_query,
                moves
            );
        }
        else if((game_id == X) || (game_id == Y))
        {
            // DTODO: this is likely fragile.
            for(size_t i = 0; i < 4; ++i)
            {
                moves.pop_back();
            }
        }

        return moves;
    }

    std::vector<std::string> get_move_name_list(
        pkmn::e_game game
    )
    {
        static BOOST_CONSTEXPR const char* main_query =
            "SELECT name FROM move_names WHERE local_language_id=9 AND "
            "move_id IN (SELECT id FROM moves WHERE generation_id<=? AND "
            "type_id<100) ORDER BY move_id";

        int game_id = game_enum_to_id(game);
        int generation = game_enum_to_generation(game);

        std::vector<std::string> ret;

        pkmn::database::query_db_list_bind1<std::string, int>(
            main_query, ret, generation
        );

        // DTODO: We have a database table with these. There has to be
        // a way to do this in a single transaction...
        if(generation < 6)
        {
            static const std::unordered_map<std::string, std::string> REPLACEMENT_MOVES =
            {
                {"Ancient Power",  "AncientPower"},
                {"Bubble Beam",    "BubbleBeam"},
                {"Double Slap",    "DoubleSlap"},
                {"Dragon Breath",  "DragonBreath"},
                {"Dynamic Punch",  "DynamicPunch"},
                {"Extreme Speed",  "ExtremeSpeed"},
                {"Feint Attack",   "Faint Attack"},
                {"Feather Dance",  "FeatherDance"},
                {"Grass Whistle",  "GrassWhistle"},
                {"High Jump Kick", "Hi Jump Kick"},
                {"Poison Powder",  "PoisonPowder"},
                {"Sand Attack",    "Sand-Attack"},
                {"Self-Destruct",  "Selfdestruct"},
                {"Smelling Salts", "SmellingSalt"},
                {"Smokescreen",    "SmokeScreen"},
                {"Soft-Boiled",    "Softboiled"},
                {"Solar Beam",     "SolarBeam"},
                {"Sonic Boom",     "SonicBoom"},
                {"Thunder Punch",  "ThunderPunch"},
                {"Thunder Shock",  "ThunderShock"},
                {"Vice Grip",      "ViceGrip"},
            };

            for(const auto& move_pair: REPLACEMENT_MOVES)
            {
                const std::string& new_move = move_pair.first;
                const std::string& old_move = move_pair.second;

#ifndef NDEBUG
                // Since we're not actually using the database, use it to
                // validate the map entries.
                (void)pkmn::database::move_name_to_id(new_move);
                (void)pkmn::database::move_name_to_id(old_move);
#endif
                std::replace(
                    ret.begin(),
                    ret.end(),
                    new_move,
                    old_move
                );
            }
        }

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
        if(game_id == COLOSSEUM)
        {
            static BOOST_CONSTEXPR const char* shadow_query =
                "SELECT name FROM move_names WHERE local_language_id=9 AND "
                "move_id=10001";

            pkmn::database::query_db_list<std::string>(
                shadow_query, ret
            );
        }
        else if(game_id == XD)
        {
            static BOOST_CONSTEXPR const char* shadow_query =
                "SELECT name FROM move_names WHERE local_language_id=9 AND "
                "move_id IN (SELECT id FROM moves WHERE type_id=10002) "
                "ORDER BY move_id";

            pkmn::database::query_db_list<std::string>(
                shadow_query, ret
            );
        }
        else if((game_id == X) || (game_id == Y))
        {
            // DTODO: this is likely fragile.
            for(size_t i = 0; i < 4; ++i)
            {
                ret.pop_back();
            }
        }

        return ret;
    }

    std::vector<pkmn::e_nature> get_nature_list()
    {
        static const std::string query =
            "SELECT game_index FROM natures ORDER BY game_index";

        std::vector<pkmn::e_nature> natures;
        pkmn::database::query_db_enum_list(
            query,
            natures
        );

        std::transform(
            natures.begin(),
            natures.end(),
            natures.begin(),
            [](pkmn::e_nature nature)
            {
                return static_cast<pkmn::e_nature>(static_cast<int>(nature)+1);
            }
        );

        return natures;
    }

    std::vector<std::string> get_nature_name_list()
    {
        static BOOST_CONSTEXPR const char* query =
            "SELECT nature_names.name FROM nature_names INNER JOIN natures ON "
            "(nature_names.nature_id=natures.id) WHERE nature_names.local_language_id=9 "
            "ORDER BY natures.game_index";

        std::vector<std::string> ret;
        pkmn::database::query_db_list<std::string>(
            query, ret
        );

        return ret;
    }

    std::vector<pkmn::e_species> get_pokemon_list(
        int generation,
        bool include_previous
    )
    {
        pkmn::enforce_bounds("generation", generation, 1, 6);

        static const std::string with_previous_query =
            "SELECT id FROM pokemon_species WHERE generation_id<=?";

        static const std::string no_previous_query =
            "SELECT id FROM pokemon_species WHERE generation_id=?";

        std::vector<pkmn::e_species> species;
        pkmn::database::query_db_enum_list_bind1(
            (include_previous ? with_previous_query : no_previous_query),
            species,
            generation
        );

        return species;
    }

    std::vector<std::string> get_pokemon_name_list(
        int generation,
        bool include_previous
    )
    {
        pkmn::enforce_bounds("generation", generation, 1, 6);

        static BOOST_CONSTEXPR const char* with_previous_query =
            "SELECT name FROM pokemon_species_names WHERE local_language_id=9 AND "
            "pokemon_species_id IN (SELECT id FROM pokemon_species WHERE generation_id<=?) "
            "ORDER BY pokemon_species_id";

        static BOOST_CONSTEXPR const char* no_previous_query =
            "SELECT name FROM pokemon_species_names WHERE local_language_id=9 AND "
            "pokemon_species_id IN (SELECT id FROM pokemon_species WHERE generation_id=?) "
            "ORDER BY pokemon_species_id";

        std::vector<std::string> ret;
        pkmn::database::query_db_list_bind1<std::string, int>(
            (include_previous ? with_previous_query : no_previous_query),
            ret, generation
        );

        return ret;
    }

    std::vector<std::string> get_region_name_list()
    {
        static BOOST_CONSTEXPR const char* query =
            "SELECT name FROM region_names WHERE local_language_id=9 "
            "AND region_id<=6 ORDER BY region_id";

        std::vector<std::string> ret;
        pkmn::database::query_db_list<std::string>(
            query, ret
        );
        ret.insert(ret.begin()+3, "Orre");

        return ret;
    }

    std::vector<std::string> get_ribbon_name_list(
        int generation
    )
    {
        (void)generation;
        return std::vector<std::string>();
    }

    std::vector<std::string> get_super_training_medal_name_list()
    {
        static BOOST_CONSTEXPR const char* query =
            "SELECT name FROM ribbons_medals WHERE super_training_medal=1";

        std::vector<std::string> ret;
        pkmn::database::query_db_list<std::string>(
            query, ret
        );

        return ret;
    }

    std::vector<pkmn::e_move> get_tm_move_list(
        pkmn::e_game game
    )
    {
        static const std::string query =
            "SELECT move_names.move_id FROM move_names INNER JOIN machines ON (machines.move_id=move_names.move_id) "
            "WHERE machines.version_group_id=? AND machines.machine_number<101 "
            "AND move_names.local_language_id=9 ORDER BY machines.machine_number";

        int version_group_id = game_enum_to_version_group(game);

        std::vector<pkmn::e_move> tm_moves;
        pkmn::database::query_db_enum_list_bind1(
            query,
            tm_moves,
            version_group_id
        );
        return tm_moves;
    }

    std::vector<std::string> get_tm_move_name_list(
        pkmn::e_game game
    )
    {
        static BOOST_CONSTEXPR const char* query =
            "SELECT name FROM move_names INNER JOIN machines ON (machines.move_id=move_names.move_id) "
            "WHERE machines.version_group_id=? AND machines.machine_number<101 "
            "AND move_names.local_language_id=9 ORDER BY machines.machine_number";

        int game_id = game_enum_to_id(game);
        int version_group_id = 0;

        if(game_is_gamecube(game_id))
        {
            BOOST_STATIC_CONSTEXPR int RS_VERSION_GROUP_ID = 5;

            version_group_id = RS_VERSION_GROUP_ID;
        }
        else
        {
            version_group_id = game_id_to_version_group(game_id);
        }

        std::vector<std::string> ret;
        pkmn::database::query_db_list_bind1<std::string, int>(
            query, ret, version_group_id
        );

        return ret;
    }

    std::vector<pkmn::e_type> get_type_list(pkmn::e_game game)
    {
        int game_id = game_enum_to_id(game);
        int generation = game_enum_to_generation(game);

        static const std::string query =
            "SELECT id FROM types WHERE generation_id<=? AND id<100";

        std::vector<pkmn::e_type> types;
        pkmn::database::query_db_enum_list_bind1(
            query,
            types,
            generation
        );

        // The Shadow type only exists in the Gamecube games.
        if(game_is_gamecube(game_id))
        {
            types.emplace_back(pkmn::e_type::SHADOW);
        }

        // The ??? type only exists in Generations II-IV.
        if((generation >= 2) && (generation <= 4))
        {
            types.emplace_back(pkmn::e_type::QUESTION_MARK);
        }

        return types;
    }

    std::vector<std::string> get_type_name_list(
        pkmn::e_game game
    )
    {
        int game_id = game_enum_to_id(game);
        int generation = game_enum_to_generation(game);

        static BOOST_CONSTEXPR const char* query =
            "SELECT name FROM type_names WHERE local_language_id=9 AND "
            "type_id IN (SELECT id FROM types WHERE generation_id<=?) "
            "AND type_id<100 ORDER BY type_id";

        std::vector<std::string> ret;
        pkmn::database::query_db_list_bind1<std::string, int>(
            query, ret, generation
        );

        // The Shadow type only exists in the Gamecube games.
        if(game_is_gamecube(game_id))
        {
            ret.emplace_back("Shadow");
        }

        // The ??? type only exists in Generations II-IV.
        if((generation >= 2) && (generation <= 4))
        {
            ret.emplace_back("???");
        }

        return ret;
    }
}}

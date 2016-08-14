/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "database_common.hpp"
#include "id_to_index.hpp"
#include "id_to_string.hpp"

#include <boost/assign.hpp>
#include <boost/config.hpp>

#include <pkmn/database/pokemon_entry.hpp>

#ifdef PKMN_SQLITE_DEBUG
#include <iostream>
#endif

namespace pkmn { namespace database {

    static pkmn::database::sptr _db;

    pokemon_entry::pokemon_entry():
        _species_id(0),
        _pokemon_id(0),
        _form_id(0),
        _pokemon_index(0),
        _game_id(0),
        _generation(0),
        _version_group_id(0),
        _none(true),
        _invalid(false)
    {}

    pokemon_entry::pokemon_entry(
        int pokemon_index,
        int game_id
    ):
        _species_id(0),
        _pokemon_id(0),
        _form_id(0),
        _pokemon_index(pokemon_index),
        _game_id(game_id),
        _none(pokemon_index == 0),
        _invalid(false) // TODO: valid check
    {
        // Connect to database
        pkmn::database::get_connection(_db);

        _generation = pkmn::database::game_id_to_generation(
                          _game_id
                      );
        _version_group_id = pkmn::database::game_id_to_version_group(
                                _game_id
                            );

        _set_vars(true);
    }

    pokemon_entry::pokemon_entry(
        const std::string &species_name,
        const std::string &game_name,
        const std::string &form_name
    ):
        _pokemon_id(0),
        _form_id(0),
        _none(false),
        _invalid(false)
    {
        // Connect to database
        pkmn::database::get_connection(_db);

        _species_id = pkmn::database::species_name_to_id(
                          species_name
                      );
        _game_id = pkmn::database::game_name_to_id(
                          game_name
                      );
        _generation = pkmn::database::game_id_to_generation(
                          _game_id
                      );
        _version_group_id = pkmn::database::game_id_to_version_group(
                                _game_id
                            );
        (void)form_name;

        _set_vars(false);
    }

    std::string pokemon_entry::get_name() const {
        return pkmn::database::species_id_to_name(
                   _species_id
               );
    }

    std::string pokemon_entry::get_game() const {
        return pkmn::database::game_id_to_name(
                   _game_id
               );
    }

    std::string pokemon_entry::get_species() const {
        static BOOST_CONSTEXPR const char* query = \
            "SELECT genus FROM pokemon_species_names WHERE "
            "pokemon_species_id=? AND local_language_id=9";

        return pkmn::database::query_db_bind1<std::string, int>(
                   _db, query, _species_id
               );
    }

    std::string pokemon_entry::get_pokedex_entry() const {
        static BOOST_CONSTEXPR const char* query = \
            "SELECT flavor_text FROM pokemon_species_flavor_text WHERE "
            "species_id=? AND version_id=? AND language_id=9";

        std::string from_db = pkmn::database::query_db_bind2<std::string, int, int>(
                                  _db, query, _species_id, _game_id
                              );
        return fix_veekun_whitespace(from_db);
    }

    std::string pokemon_entry::get_form() const {
        return "";
    }

    float pokemon_entry::get_height() const {
        if(_none or _invalid) {
            return -1.0f;
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT height FROM pokemon WHERE id=?";

        // SQLite uses doubles, so avoid implicit casting ambiguity
        return (float)pkmn::database::query_db_bind1<double, int>(
                   _db, query, _pokemon_id
               );
    }

    float pokemon_entry::get_weight() const {
        if(_none or _invalid) {
            return -1.0f;
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT weight FROM pokemon WHERE id=?";

        // SQLite uses doubles, so avoid implicit casting ambiguity
        return (float)pkmn::database::query_db_bind1<double, int>(
                          _db, query, _pokemon_id
                      );
    }

    /*
     * The database stores gender rates oddly, so this table
     * converts them to actual probabilities.
     */
    static const std::map<int, float> _veekun_gender_rates = boost::assign::map_list_of
        (-1, 0.0f)
        (0,  1.0f)
        (1,  0.875f)
        (2,  0.75f)
        (4,  0.5f)
        (6,  0.25f)
        (8,  0.0f)
    ;

    float pokemon_entry::get_chance_male() const {
        if(_none or _invalid) {
            return -1.0f;
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT gender_rate FROM pokemon_species WHERE id=?";

        int gender_rate_from_db = pkmn::database::query_db_bind1<int, int>(
                                      _db, query, _species_id
                                  );
        return _veekun_gender_rates.at(gender_rate_from_db);
    }

    float pokemon_entry::get_chance_female() const {
        if(_none or _invalid) {
            return -1.0f;
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT gender_rate FROM pokemon_species WHERE id=?";

        int gender_rate_from_db = pkmn::database::query_db_bind1<int, int>(
                                      _db, query, _species_id
                                  );
        if(gender_rate_from_db == -1) {
            return 0.0f;
        } else {
            return (1.0f - _veekun_gender_rates.at(gender_rate_from_db));
        }
    }

    bool pokemon_entry::has_gender_differences() const {
        // Physical gender differences started in Generation IV
        if(_none or _invalid or _generation < 4) {
            return false;
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT has_gender_differences FROM pokemon_species WHERE id=?";

        // SQLite has no bool type, so just test the integral value
        return (pkmn::database::query_db_bind1<int, int>(
                   _db, query, _species_id
                ) > 0);
    }

    int pokemon_entry::get_base_happiness() const {
        // Happiness was introduced in Generation II
        if(_none or _invalid or _generation == 1) {
            return -1;
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT base_happiness FROM pokemon_species WHERE id=?";

        return pkmn::database::query_db_bind1<int, int>(
                   _db, query, _species_id
               );
    }

    std::pair<std::string, std::string> pokemon_entry::get_types() const {
        if(_none) {
            return std::make_pair("None", "None");
        } else if(_invalid) {
            // TODO: Generation I, try to get types
            return std::make_pair("Unknown", "Unknown");
        }

        static BOOST_CONSTEXPR const char* query1 = \
            "SELECT type_id FROM pokemon_types WHERE pokemon_id=? AND slot=1";
        static BOOST_CONSTEXPR const char* query2 = \
            "SELECT type_id FROM pokemon_types WHERE pokemon_id=? AND slot=2";

        std::pair<std::string, std::string> ret;
        ret.first = pkmn::database::query_db_bind1<std::string, int>(
                        _db, query1, _pokemon_id
                    );
        if(not pkmn::database::maybe_query_db_bind1<std::string, int>(
               _db, query2, ret.second, _pokemon_id
           )) {
            ret.second = "None";
        }

        return ret;
    }

    std::pair<std::string, std::string> pokemon_entry::get_abilities() const {
        // Abilities were introduced in Generation III
        if(_none or _generation < 3) {
            return std::make_pair("None", "None");
        } else if(_invalid) {
            return std::make_pair("Unknown", "Unknown");
        }

        static BOOST_CONSTEXPR const char* query1 = \
            "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id=? AND "
            "is_hidden=0 AND slot=1";
        static BOOST_CONSTEXPR const char* query2 = \
            "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id=? AND "
            "is_hidden=0 AND slot=2";

        std::pair<std::string, std::string> ret;
        ret.first = pkmn::database::query_db_bind1<std::string, int>(
                        _db, query1, _pokemon_id
                    );
        if(not pkmn::database::maybe_query_db_bind1<std::string, int>(
               _db, query2, ret.second, _pokemon_id
           )) {
            ret.second = "None";
        }

        return ret;
    }

    std::string pokemon_entry::get_hidden_ability() const {
        // Hidden Abilities were introduced in Generation V
        if(_none or _generation < 5) {
            return "None";
        } else if(_invalid) {
            return "Unknown";
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id=? AND "
            "is_hidden=1";

        (void)query;
        return "";
    }

    std::pair<std::string, std::string> pokemon_entry::get_egg_groups() const {
        // Breeding was introduced in Generation I
        if(_none or _generation == 1) {
            return std::make_pair("None", "None");
        } else if(_invalid) {
            return std::make_pair("Unknown", "Unknown");
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT egg_group_id FROM pokemon_egg_groups WHERE species_id=? "
            "ORDER BY egg_group_id";
#ifdef PKMN_SQLITE_DEBUG
        std::cout << "Query: " << query << std::endl;
#endif

        std::pair<std::string, std::string> ret;
        SQLite::Statement stmt((*_db), query);
        stmt.executeStep();
#ifdef PKMN_SQLITE_DEBUG
        std::cout << "Results: " << std::string(stmt.getColumn(0)) << ", " << std::flush;
#endif
        ret.first = std::string(stmt.getColumn(0));
        if(stmt.executeStep()) {
#ifdef PKMN_SQLITE_DEBUG
            std::cout << std::string(stmt.getColumn(0)) << std::endl;
#endif
            ret.second = std::string(stmt.getColumn(0));
        } else {
#ifdef PKMN_SQLITE_DEBUG
            std::cout << "\b\b " << std::endl;
#endif
            ret.second = "None";
        }

        return ret;
    }

    std::map<std::string, int> pokemon_entry::get_base_stats() const {
        // TODO: original query can probably be optimized
        // TODO: none or invalid, all stats with values -1
        return std::map<std::string,int>();
    }

    std::map<std::string, int> pokemon_entry::get_EV_yields() const {
        // TODO: original query can probably be optimized
        // TODO: none or invalid, all stats with values -1
        return std::map<std::string,int>();
    }

    int pokemon_entry::get_experience_yield() const {
        if(_none or _invalid) {
            return -1;
        }

        return 0;
    }

    int pokemon_entry::get_experience_at_level(
        int level
    ) const {
        if(_none or _invalid) {
            return -1;
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT experience.experience FROM pokemon_species "
            "INNER JOIN experience ON pokemon_species.growth_rate_id=experience.growth_rate_id "
            "WHERE (pokemon_species.id=? AND experience.level=?) "
            "ORDER BY experience.experience";

        return pkmn::database::query_db_bind2<int, int, int>(
                   _db, query, _species_id, level
               );
    }

    int pokemon_entry::get_level_at_experience(
        int experience
    ) const {
        if(_none or _invalid) {
            return -1;
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT experience.level FROM experience "
            "INNER JOIN pokemon_species "
            "ON experience.growth_rate_id=pokemon_species.growth_rate_id "
            "AND experience.growth_rate_id=pokemon_species.growth_rate_id "
            "WHERE (experience.experience<=? "
            "AND pokemon_species.id=?) ORDER BY experience.level DESC";

        return pkmn::database::query_db_bind2<int, int, int>(
                   _db, query, experience, _species_id
               );
    }

    void pokemon_entry::get_levelup_moves(
        pkmn::database::levelup_moves_t &levelup_moves_out
    ) const {
        if(_none or _invalid) {
            return;
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT move_id,level FROM pokemon_moves WHERE pokemon_id=? "
            "AND version_group_id=? AND pokemon_move_method_id=1 ORDER BY level";
#ifdef PKMN_SQLITE_DEBUG
        std::cout << "Query: " << query << std::endl
                  << " * Bind " << _pokemon_id << " to 1" << std::endl
                  << " * Bind " << _version_group_id << " to 2" << std::endl
                  << " * Results: " << std::flush;
#endif

        levelup_moves_out.clear();
        SQLite::Statement stmt((*_db), query);
        stmt.bind(1, _pokemon_id);
        stmt.bind(2, _version_group_id);
        while(stmt.executeStep()) {
#ifdef PKMN_SQLITE_DEBUG
            std::cout << "(" << int(stmt.getColumn(0)) << "," << int(stmt.getColumn(1))
                      << "), " << std::flush;
#endif
            levelup_moves_out.emplace_back(
                pkmn::database::levelup_move(
                    pkmn::database::move_entry(
                        int(stmt.getColumn(0)),
                        _game_id
                    ),
                    int(stmt.getColumn(1))
                )
            );
        }
#ifdef PKMN_SQLITE_DEBUG
        std::cout << "\b\b " << std::endl;
#endif
    }

    void pokemon_entry::get_tm_hm_moves(
        pkmn::database::move_list_t &tm_hm_moves_out
    ) const {
        if(_none or _invalid) {
            return;
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT move_id FROM machines WHERE version_group_id=? AND move_id IN "
            "(SELECT move_id FROM pokemon_moves WHERE pokemon_move_method_id=4 AND "
            "pokemon_id=? AND version_group_id=?) ORDER BY machine_number";

        tm_hm_moves_out.clear();
        _query_to_move_list(query, tm_hm_moves_out);
    }

    void pokemon_entry::get_egg_moves(
        pkmn::database::move_list_t &egg_moves_out
    ) const {
        if(_none or _invalid) {
            return;
        }

        static BOOST_CONSTEXPR const char* evolution_query = \
            "SELECT evolves_from_species_id FROM pokemon_species WHERE id=?";

        static BOOST_CONSTEXPR const char* move_query = \
            "SELECT move_id FROM pokemon_moves WHERE pokemon_move_method_id=2 AND "
            "pokemon_id=? AND version_group_id=?";

        // TODO: get from evolution_query
        (void)evolution_query;
        int actual_pokemon_id = 0;
        egg_moves_out.clear();
        _query_to_move_list(move_query, egg_moves_out, actual_pokemon_id);
    }

    void pokemon_entry::get_tutor_moves(
        pkmn::database::move_list_t &tutor_moves_out
    ) const {
        if(_none or _invalid) {
            return;
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT move_id FROM pokemon_moves WHERE pokemon_move_method_id=3 AND "
            "pokemon_id=? AND version_group_id=?";

        tutor_moves_out.clear();
        _query_to_move_list(query, tutor_moves_out);
    }

    std::vector<std::string> pokemon_entry::get_forms() const {
        return std::vector<std::string>();
    }

    pkmn::database::pokemon_entries_t pokemon_entry::get_evolutions() const {
        if(_none or _invalid) {
            return pkmn::database::pokemon_entries_t();
        }

        return pkmn::database::pokemon_entries_t();
    }

    void pokemon_entry::set_form(
        const std::string &form_name
    ) {
        (void)form_name;
    }

    void pokemon_entry::_set_vars(
        bool from_index
    ) {
        if(from_index) {
            _pokemon_id = pkmn::database::pokemon_index_to_id(
                              _pokemon_index, _game_id
                          );

            static BOOST_CONSTEXPR const char* species_id_query = \
                "SELECT pokemon_species_id FROM pokemon WHERE id=?";

            _species_id = pkmn::database::query_db_bind1<int, int>(
                              _db, species_id_query, _pokemon_id
                          );

            // TODO: Gen III Deoxys form corner case
            // TODO: confirm version exists in this version, probably
            //       a check in pokemon_forms table

            static BOOST_CONSTEXPR const char* form_id_query = \
                "SELECT id FROM pokemon_forms WHERE pokemon_id=?";

            _form_id = pkmn::database::query_db_bind1<int, int>(
                           _db, form_id_query, _pokemon_id
                       );
        } else {
            // TODO: form name to ID, species+form IDs to Pokémon ID

            _pokemon_index = pkmn::database::pokemon_id_to_index(
                                 _pokemon_id, _game_id
                             );
        }
    }

    void pokemon_entry::_query_to_move_list(
        const char* query,
        pkmn::database::move_list_t &move_list_out,
        int overwrite_pokemon_id
    ) const {
        int pokemon_id = (overwrite_pokemon_id == -1) ? _pokemon_id
                                                      : overwrite_pokemon_id;

#ifdef PKMN_SQLITE_DEBUG
        std::cout << "Query: " << query << std::endl
                  << " * Bind " << pokemon_id << " to 1" << std::endl
                  << " * Bind " << _version_group_id << " to 2" << std::endl
                  << " * Results: " << std::flush;
#endif
        SQLite::Statement stmt((*_db), query);
        stmt.bind(1, pokemon_id);
        stmt.bind(2, _version_group_id);

        while(stmt.executeStep()) {
#ifdef PKMN_SQLITE_DEBUG
            std::cout << int(stmt.getColumn(0)) << ", ";
#endif
            move_list_out.emplace_back(
                pkmn::database::move_entry(
                    int(stmt.getColumn(0)),
                    _game_id
                )
            );
        }
#ifdef PKMN_SQLITE_DEBUG
        std::cout << "\b\b " << std::endl;
#endif
    }

}}

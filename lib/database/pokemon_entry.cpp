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
#include <boost/format.hpp>

#include <pkmn/database/pokemon_entry.hpp>

#ifdef PKMN_SQLITE_DEBUG
#include <iostream>
#endif

namespace pkmn { namespace database {

    static pkmn::database::sptr _db;

    /*
     * IDs and indices for hardcoded cases
     */

    BOOST_STATIC_CONSTEXPR int PIKACHU_ID          = 25;
    BOOST_STATIC_CONSTEXPR int ROCKSTAR_PIKACHU_ID = 10182;
    BOOST_STATIC_CONSTEXPR int COSPLAY_PIKACHU_ID  = 10187;

    BOOST_STATIC_CONSTEXPR int PICHU_ID             = 172;
    BOOST_STATIC_CONSTEXPR int SPIKY_EARED_PICHU_ID = 10065;

    BOOST_STATIC_CONSTEXPR int UNOWN_INDEX          = 201;
    BOOST_STATIC_CONSTEXPR int UNOWN_B_INDEX        = 10001;
    BOOST_STATIC_CONSTEXPR int UNOWN_Z_INDEX        = 10005;
    BOOST_STATIC_CONSTEXPR int UNOWN_QUESTION_INDEX = 10027;

    BOOST_STATIC_CONSTEXPR int DEOXYS_ID         = 386;
    BOOST_STATIC_CONSTEXPR int DEOXYS_GEN3_INDEX = 410;
    BOOST_STATIC_CONSTEXPR int DEOXYS_NORMAL_ID  = 386;

    BOOST_STATIC_CONSTEXPR int DEOXYS_ATTACK_ID  = 10001;
    BOOST_STATIC_CONSTEXPR int DEOXYS_DEFENSE_ID = 10002;
    BOOST_STATIC_CONSTEXPR int DEOXYS_SPEED_ID   = 10003;

    BOOST_STATIC_CONSTEXPR int DEOXYS_ATTACK_FORM_ID  = 10031;
    BOOST_STATIC_CONSTEXPR int DEOXYS_DEFENSE_FORM_ID = 10032;
    BOOST_STATIC_CONSTEXPR int DEOXYS_SPEED_FORM_ID   = 10033;

    BOOST_STATIC_CONSTEXPR int ARCEUS_ID = 493;

    BOOST_STATIC_CONSTEXPR int EMERALD   = 9;
    BOOST_STATIC_CONSTEXPR int FIRERED   = 10;
    BOOST_STATIC_CONSTEXPR int LEAFGREEN = 11;

    BOOST_STATIC_CONSTEXPR int HGSS = 10;
    BOOST_STATIC_CONSTEXPR int ORAS = 16;

    static PKMN_CONSTEXPR_OR_INLINE bool form_id_is_cosplay_pikachu(
        int form_id
    ) {
        return (form_id >= ROCKSTAR_PIKACHU_ID) and
               (form_id <= COSPLAY_PIKACHU_ID);
    }

    static PKMN_CONSTEXPR_OR_INLINE bool pokemon_index_is_unown(
        int pokemon_index,
        bool gen2
    ) {
        return (pokemon_index == UNOWN_INDEX) or
               (pokemon_index >= UNOWN_B_INDEX and
                pokemon_index <= (gen2 ? UNOWN_Z_INDEX
                                       : UNOWN_QUESTION_INDEX));
    }

    pokemon_entry::pokemon_entry(
        int pokemon_index,
        int game_id
    ):
        _pokemon_index(pokemon_index),
        _game_id(game_id),
        _none(pokemon_index == 0)
    {
        // Connect to database
        pkmn::database::get_connection(_db);

        /*
         * Game-related info
         */
        _generation = pkmn::database::game_id_to_generation(
                          _game_id
                      );
        _version_group_id = pkmn::database::game_id_to_version_group(
                                _game_id
                            );

        /*
         * Pokémon-related info
         */

        /*
         * Generation III does things differently than later games,
         * so we need to hardcode these cases.
         *
         * Unown has separate in-game indices for each of its forms,
         * unlike later games, where the form is a field inside the
         * Pokémon data structure.
         *
         * Deoxys's form is game-dependent, but they share a common
         * index. Like Unown, this form is in a separate field and is
         * no longer game-dependent.
         */
        if(_none) {
            _species_id = _pokemon_id = _form_id = 0;
        } else if(_generation == 3) {
            if(pokemon_index_is_unown(_pokemon_index, false)) {
                _species_id = _pokemon_id = UNOWN_INDEX;
                _invalid = false;

                static BOOST_CONSTEXPR const char* unown_query = \
                    "SELECT pokemon_id FROM pokemon_forms WHERE id="
                    "(SELECT form_id FROM gen3_unown_game_indices "
                    "WHERE game_index=?)";

                _form_id = pkmn::database::query_db_bind1<int, int>(
                               _db, unown_query, _pokemon_index
                           );
            } else if(_pokemon_index == DEOXYS_GEN3_INDEX) {
                _species_id = DEOXYS_NORMAL_ID;
                _invalid = false;
                switch(_game_id) {
                    case FIRERED:
                        _pokemon_id = DEOXYS_ATTACK_ID;
                        _form_id    = DEOXYS_ATTACK_FORM_ID;
                        break;

                    case LEAFGREEN:
                        _pokemon_id = DEOXYS_DEFENSE_ID;
                        _form_id    = DEOXYS_DEFENSE_FORM_ID;
                        break;

                    case EMERALD:
                        _pokemon_id = DEOXYS_SPEED_ID;
                        _form_id    = DEOXYS_SPEED_FORM_ID;
                        break;

                    default:
                        _pokemon_id = _form_id = DEOXYS_NORMAL_ID;
                        break;
                }
            }
        } else {
            static BOOST_CONSTEXPR const char* species_id_query = \
                "SELECT pokemon_species_id FROM pokemon WHERE id=?";

            static BOOST_CONSTEXPR const char* form_id_query = \
                "SELECT id FROM pokemon_forms WHERE pokemon_id=?";

            try {
                _pokemon_id = pkmn::database::pokemon_index_to_id(
                                  _pokemon_index, _game_id
                              );
                _invalid = false;
            } catch(const std::invalid_argument&) {
                _invalid = true;
            }

            if(_invalid) {
                _species_id = _pokemon_id = _form_id = -1;
            } else {
                _species_id = pkmn::database::query_db_bind1<int, int>(
                                  _db, species_id_query, _pokemon_id
                              );

                _form_id = pkmn::database::query_db_bind1<int, int>(
                               _db, form_id_query, _pokemon_id
                           );
            }
        }
    }

    pokemon_entry::pokemon_entry(
        const std::string &species_name,
        const std::string &game_name,
        const std::string &form_name
    ):
        _none(species_name == "None"),
        _invalid(false)
    {
        // Connect to database
        pkmn::database::get_connection(_db);

        /*
         * Game-related info
         */
        _game_id = pkmn::database::game_name_to_id(
                          game_name
                      );
        _generation = pkmn::database::game_id_to_generation(
                          _game_id
                      );
        _version_group_id = pkmn::database::game_id_to_version_group(
                                _game_id
                            );

        /*
         * Pokémon-related info
         */
        _species_id = pkmn::database::species_name_to_id(
                          species_name
                      );

        if(form_name == "" or form_name == species_name) {
            _pokemon_id = _form_id = _species_id;
        } else {
            this->set_form(form_name);
        }
    }

    std::string pokemon_entry::get_name() const {
        if(_none) {
            return "None";
        } else if(_invalid) {
            return str(boost::format("Invalid (0x%x)") % _pokemon_index);
        }

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
        if(_none) {
            return "None";
        } else if(_invalid) {
            return "Unknown";
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT genus FROM pokemon_species_names WHERE "
            "pokemon_species_id=? AND local_language_id=9";

        return pkmn::database::query_db_bind1<std::string, int>(
                   _db, query, _species_id
               );
    }

    std::string pokemon_entry::get_pokedex_entry() const {
        if(_none) {
            return "None";
        } else if(_invalid) {
            return "Unknown";
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT flavor_text FROM pokemon_species_flavor_text WHERE "
            "species_id=? AND version_id=? AND language_id=9";

        std::string from_db = pkmn::database::query_db_bind2<std::string, int, int>(
                                  _db, query, _species_id, _game_id
                              );
        return fix_veekun_whitespace(from_db);
    }

    static std::string _get_species_standard_form(
        int species_id
    ) {
        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM libpkmn_pokemon_form_names WHERE form_id=?";

        std::string ret;
        if(pkmn::database::maybe_query_db_bind1<std::string, int>(
               _db, query, ret, species_id
          ))
        {
            return ret;
        } else {
            return "Standard";
        }
    }

    std::string pokemon_entry::get_form() const {
        if(_none) {
            return "None";
        } else if(_invalid) {
            return "Unknown";
        }

        if(_form_id == _species_id) {
            return _get_species_standard_form(_species_id);
        } else {
            static BOOST_CONSTEXPR const char* query = \
                "SELECT name FROM libpkmn_pokemon_form_names WHERE form_id=?";

            return pkmn::database::query_db_bind1<std::string, int>(
                       _db, query, _form_id
                   );
        }

        return "";
    }

    float pokemon_entry::get_height() const {
        if(_none or _invalid) {
            return -1.0f;
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT height FROM pokemon WHERE id=?";

        double from_db = pkmn::database::query_db_bind1<double, int>(
                             _db, query, _pokemon_id
                         );

        return (float(from_db) / 10.0f);
    }

    float pokemon_entry::get_weight() const {
        if(_none or _invalid) {
            return -1.0f;
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT weight FROM pokemon WHERE id=?";

        double from_db = pkmn::database::query_db_bind1<double, int>(
                             _db, query, _pokemon_id
                         );

        return (float(from_db) / 10.0f);
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
            return std::make_pair("Unknown", "Unknown");
        }

        static BOOST_CONSTEXPR const char* query1 = \
            "SELECT name FROM type_names WHERE local_language_id=9 AND type_id="
            "(SELECT type_id FROM pokemon_types WHERE pokemon_id=? AND slot=1)";
        static BOOST_CONSTEXPR const char* query2 = \
            "SELECT name FROM type_names WHERE local_language_id=9 AND type_id="
            "(SELECT type_id FROM pokemon_types WHERE pokemon_id=? AND slot=2)";

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

    /*
     * Just return these for None and Invalid entries
     */
    static const std::map<std::string, int> _bad_stat_map_old = boost::assign::map_list_of
        ("HP", 0)("Attack", 0)("Defense", 0)
        ("Speed", 0)("Special", 0)
    ;
    static const std::map<std::string, int> _bad_stat_map = boost::assign::map_list_of
        ("HP", 0)("Attack", 0)("Defense", 0)
        ("Speed", 0)("Special Attack", 0)("Special Defense", 0)
    ;

    // Convenience (TODO: debug output)
    static PKMN_INLINE void execute_stat_stmt_and_get(
        SQLite::Statement &stmt,
        std::map<std::string, int> &ret,
        const std::string &key
    ) {
        stmt.executeStep();
        ret[key] = stmt.getColumn(0);
    }

    std::map<std::string, int> pokemon_entry::get_base_stats() const {
        if(_none or _invalid) {
            return (_generation == 1) ? _bad_stat_map_old
                                      : _bad_stat_map;
        }

        static BOOST_CONSTEXPR const char* old_query = \
            "SELECT base_stat FROM pokemon_stats WHERE pokemon_id=? AND "
            "stat_id IN (1,2,3,6,9)";

        static BOOST_CONSTEXPR const char* main_query = \
            "SELECT base_stat FROM pokemon_stats WHERE pokemon_id=? AND "
            "stat_id IN (1,2,3,6,4,5)";

        std::map<std::string, int> ret;

        // TODO: debug output
        SQLite::Statement stmt(
            (*_db),
            ((_generation == 1) ? old_query : main_query)
        );
        stmt.bind(1, _pokemon_id);

        execute_stat_stmt_and_get(stmt, ret, "HP");
        execute_stat_stmt_and_get(stmt, ret, "Attack");
        execute_stat_stmt_and_get(stmt, ret, "Defense");
        execute_stat_stmt_and_get(stmt, ret, "Speed");
        if(_generation == 1) {
            execute_stat_stmt_and_get(stmt, ret, "Special");
        } else {
            execute_stat_stmt_and_get(stmt, ret, "Special Attack");
            execute_stat_stmt_and_get(stmt, ret, "Special Defense");
        }

        return (ret);
    }

    std::map<std::string, int> pokemon_entry::get_EV_yields() const {
        if(_none or _invalid) {
            return (_generation <= 2) ? _bad_stat_map_old
                                     : _bad_stat_map;
        } else if(_generation == 1) {
            // EV's are just base stats
            return this->get_base_stats();
        } else if(_generation == 2) {
            // EV's almost match base stats but just have Special
            std::map<std::string, int> ret = this->get_base_stats();
            ret["Special"] = ret["Special Attack"];
            ret.erase("Special Attack");
            ret.erase("Special Defense");

            return (ret);
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT effort FROM pokemon_stats WHERE pokemon_id=? AND "
            "stat_id IN (1,2,3,6,4,5)";

        std::map<std::string, int> ret;

        SQLite::Statement stmt((*_db), query);
        stmt.bind(1, _pokemon_id);

        execute_stat_stmt_and_get(stmt, ret, "HP");
        execute_stat_stmt_and_get(stmt, ret, "Attack");
        execute_stat_stmt_and_get(stmt, ret, "Defense");
        execute_stat_stmt_and_get(stmt, ret, "Speed");
        execute_stat_stmt_and_get(stmt, ret, "Special Attack");
        execute_stat_stmt_and_get(stmt, ret, "Special Defense");

        return (ret);
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

    pkmn::database::levelup_moves_t pokemon_entry::get_levelup_moves() const {
        if(_none or _invalid) {
            return pkmn::database::levelup_moves_t();
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

        pkmn::database::levelup_moves_t ret;
        SQLite::Statement stmt((*_db), query);
        stmt.bind(1, _pokemon_id);
        stmt.bind(2, _version_group_id);
        while(stmt.executeStep()) {
#ifdef PKMN_SQLITE_DEBUG
            std::cout << "(" << int(stmt.getColumn(0)) << "," << int(stmt.getColumn(1))
                      << "), " << std::flush;
#endif
            ret.emplace_back(
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
        return ret;
    }

    pkmn::database::move_list_t pokemon_entry::get_tm_hm_moves() const {
        if(_none or _invalid) {
            return pkmn::database::move_list_t();
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT move_id FROM machines WHERE version_group_id=? AND move_id IN "
            "(SELECT move_id FROM pokemon_moves WHERE pokemon_move_method_id=4 AND "
            "pokemon_id=? AND version_group_id=?) ORDER BY machine_number";

        pkmn::database::move_list_t ret;
        _query_to_move_list(query, ret);
        return ret;
    }

    pkmn::database::move_list_t pokemon_entry::get_egg_moves() const {
        if(_none or _invalid) {
            return pkmn::database::move_list_t();
        }

        static BOOST_CONSTEXPR const char* evolution_query = \
            "SELECT evolves_from_species_id FROM pokemon_species WHERE id=?";

        static BOOST_CONSTEXPR const char* move_query = \
            "SELECT move_id FROM pokemon_moves WHERE pokemon_move_method_id=2 AND "
            "pokemon_id=? AND version_group_id=?";

        // TODO: get from evolution_query
        (void)evolution_query;
        int actual_pokemon_id = 0;

        pkmn::database::move_list_t ret;
        _query_to_move_list(move_query, ret, actual_pokemon_id);
        return ret;
    }

    pkmn::database::move_list_t pokemon_entry::get_tutor_moves() const {
        if(_none or _invalid) {
            return pkmn::database::move_list_t();
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT move_id FROM pokemon_moves WHERE pokemon_move_method_id=3 AND "
            "pokemon_id=? AND version_group_id=?";

        pkmn::database::move_list_t ret;
        _query_to_move_list(query, ret);
        return ret;
    }

    std::vector<std::string> pokemon_entry::get_forms() const {
        std::vector<std::string> ret;
        ret.emplace_back(
            _get_species_standard_form(_species_id)
        );

        /*
         * We need to hardcode a couple things here. Pichu's Spiky-eared form was
         * only in HeartGold/SoulSilver, so don't let it appear later. Same applies
         * with Arceus's '???' form, which only appeared in Generation IV.
         */
        if(not (_species_id == PICHU_ID and _version_group_id != HGSS)) {
            static BOOST_CONSTEXPR const char* form_ids_query = \
                "SELECT name FROM libpkmn_pokemon_form_names WHERE form_id IN "
                "(SELECT id FROM pokemon_forms WHERE introduced_in_version_group_id<=? "
                "AND pokemon_id IN (SELECT id FROM pokemon WHERE species_id=?)) AND "
                "form_id!=? AND form_id!=10057";

            pkmn::database::query_db_list_bind3<std::string, int, int, int>(
                _db, form_ids_query, ret, _version_group_id, _species_id, _species_id
            );
        }

        if(_species_id == ARCEUS_ID and _generation == 4) {
            ret.emplace_back("???");
        }

        return ret;
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
        if(form_name == "") {
            _form_id = _pokemon_id = _species_id;
            return;
        }

        /*
         * Start by checking which forms exist in this game. If not, immediately
         * throw an error.
         */
        std::vector<std::string> forms = this->get_forms();
        if(std::find(forms.begin(), forms.end(), form_name) == forms.end()) {
            throw std::invalid_argument("Invalid form.");
        }

        // Set the form and Pokémon ID
        static BOOST_CONSTEXPR const char* query = \
            "SELECT id,pokemon_id FROM pokemon_forms WHERE id="
            "(SELECT form_id FROM libpkmn_pokemon_form_names WHERE name=?)";

#ifdef PKMN_SQLITE_DEBUG
        std::cout << "Query: " << query << std::endl
                  << " * Bind " << form_name << " to 1" << std::endl
                  << " * Results: " << std::flush;
#endif
        SQLite::Statement stmt((*_db), query);
        stmt.bind(1, form_name);
        stmt.executeStep();
#ifdef PKMN_SQLITE_DEBUG
        std::cout << int(stmt.getColumn(0)) << " "
                  << int(stmt.getColumn(1)) << std::endl;
#endif
        /*
         * Now that we have the form ID, check some of the hardcoded cases
         * before assigning the proper IDs.
         */
        int form_id = stmt.getColumn(0);

        if(_generation == 3 and _species_id == DEOXYS_ID) {
            switch(_game_id) {
                case FIRERED:
                    if(form_id != DEOXYS_ATTACK_ID) {
                        throw std::invalid_argument(
                                  "Deoxys can only be in its Attack Forme in FireRed."
                              );
                    }
                    break;

                case LEAFGREEN:
                    if(form_id != DEOXYS_DEFENSE_ID) {
                        throw std::invalid_argument(
                                  "Deoxys can only be in its Defense Forme in LeafGreen."
                              );
                    }
                    break;

                case EMERALD:
                    if(form_id != DEOXYS_SPEED_ID) {
                        throw std::invalid_argument(
                                  "Deoxys can only be in its Speed Forme in Emerald."
                              );
                    }
                    break;

                default:
                    if(form_id != DEOXYS_NORMAL_ID) {
                        throw std::invalid_argument(
                            str(boost::format("Deoxys can only be in its Normal Forme in %s.")
                                    % this->get_game().c_str()
                               )
                        );
                    }
                    break;
            }

            _pokemon_index = DEOXYS_GEN3_INDEX;
        } else if(_version_group_id != HGSS and form_id == SPIKY_EARED_PICHU_ID) {
            throw std::invalid_argument("Spiky-Eared Pichu is only in HeartGold/SoulSilver.");
        } else if(form_id_is_cosplay_pikachu(form_id) and _version_group_id != ORAS) {
            throw std::invalid_argument(
                str(boost::format("%s can only be in its Normal Forme in Omega Ruby/Alpha Sapphire.")
                        % form_name.c_str()
                   )
            );
        }

        _form_id    = stmt.getColumn(0);
        _pokemon_id = stmt.getColumn(1);
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

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

namespace pkmn { namespace database {

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
        return species_id_to_name(
                   _species_id
               );
    }

    std::string pokemon_entry::get_game() const {
        return game_id_to_name(
                   _game_id
               );
    }

    std::string pokemon_entry::get_species() const {
        static BOOST_CONSTEXPR const char* query = \
            "SELECT genus FROM pokemon_species_names WHERE "
            "pokemon_species_id=? AND local_language_id=9";

        return pkmn_db_query_bind1<std::string, int>(
                   query, _species_id
               );
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

        return pkmn_db_query_bind1<float, int>(
                   query, _pokemon_id
               );
    }

    float pokemon_entry::get_weight() const {
        if(_none or _invalid) {
            return -1.0f;
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT weight FROM pokemon WHERE id=?";

        return pkmn_db_query_bind1<float, int>(
                   query, _pokemon_id
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

        int gender_rate_from_db = pkmn_db_query_bind1<int, int>(
                                      query, _species_id
                                  );
        return _veekun_gender_rates.at(gender_rate_from_db);
    }

    float pokemon_entry::get_chance_female() const {
        if(_none or _invalid) {
            return -1.0f;
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT gender_rate FROM pokemon_species WHERE id=?";

        int gender_rate_from_db = pkmn_db_query_bind1<int, int>(
                                      query, _species_id
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

        return pkmn_db_query_bind1<bool, int>(
                   query, _species_id
               );
    }

    int pokemon_entry::get_base_happiness() const {
        // Happiness was introduced in Generation II
        if(_none or _invalid or _generation == 1) {
            return -1;
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT base_happiness FROM pokemon_species WHERE id=?";

        return pkmn_db_query_bind1<int, int>(
                   query, _species_id
               );
    }

    std::pair<std::string, std::string> pokemon_entry::get_types() const {
        if(_none) {
            return std::make_pair("None", "None");
        } else if(_invalid) {
            // TODO: Generation I, try to get types
            return std::make_pair("Unknown", "Unknown");
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT type_id FROM pokemon_types WHERE pokemon_id=? ORDER BY slot";

        (void)query;
        return std::pair<std::string,std::string>();
    }

    std::pair<std::string, std::string> pokemon_entry::get_abilities() const {
        // Abilities were introduced in Generation III
        if(_none or _generation < 3) {
            return std::make_pair("None", "None");
        } else if(_invalid) {
            return std::make_pair("Unknown", "Unknown");
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id=? AND "
            "is_hidden=0 ORDER BY slot";

        (void)query;
        return std::pair<std::string,std::string>();
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
            "SELECT egg_group_id FROM pokemon_egg_groups WHERE pokemon_id=? AND "
            "is_hidden=0 ORDER BY slot";

        (void)query;
        return std::pair<std::string,std::string>();
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

        return pkmn_db_query_bind2<int, int, int>(
                   query, _species_id, level
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
            "AND pokemon_species.id=?) ORDER  BY experience.level DESC";

        return pkmn_db_query_bind2<int, int, int>(
                   query, experience, _species_id
               );
    }

    pkmn::database::levelup_moves_t pokemon_entry::get_levelup_moves() const {
        if(_none or _invalid) {
            return pkmn::database::levelup_moves_t();
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT level,move_id FROM pokemon_moves WHERE pokemon_id=? "
            "AND version_group_id=? AND pokemon_move_method_id=1 ORDER BY level";

        (void)query;
        return pkmn::database::levelup_moves_t();
    }

    pkmn::database::move_list_t pokemon_entry::get_tm_hm_moves() const {
        if(_none or _invalid) {
            return pkmn::database::move_list_t();
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT move_id FROM machines WHERE version_group_id=? AND move_id IN "
            "(SELECT move_id FROM pokemon_moves WHERE pokemon_move_method_id=4 AND "
            "pokemon_id=? AND version_group_id=?) ORDER BY machine_number";

        (void)query;
        return pkmn::database::move_list_t();
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

        (void)evolution_query;
        (void)move_query;
        return pkmn::database::move_list_t();
    }

    pkmn::database::move_list_t pokemon_entry::get_tutor_moves() const {
        if(_none or _invalid) {
            return pkmn::database::move_list_t();
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT move_id FROM pokemon_moves WHERE pokemon_move_method_id=3 AND "
            "pokemon_id=? AND version_group_id=?";

        (void)query;
        return pkmn::database::move_list_t();
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

            _species_id = pkmn_db_query_bind1<int, int>(
                              species_id_query, _pokemon_id
                          );

            // TODO: Gen III Deoxys form corner case
            // TODO: confirm version exists in this version, probably
            //       a check in pokemon_forms table

            static BOOST_CONSTEXPR const char* form_id_query = \
                "SELECT id FROM pokemon_forms WHERE pokemon_id=?";

            _form_id = pkmn_db_query_bind1<int, int>(
                           form_id_query, _pokemon_id
                       );
        } else {
            // TODO: form name to ID, species+form IDs to Pokémon ID

            _pokemon_index = pkmn::database::pokemon_id_to_index(
                                 _pokemon_id, _game_id
                             );
        }
    }

}}

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
#include "id_to_index.hpp"
#include "id_to_string.hpp"

#include <pkmn/exception.hpp>
#include <pkmn/database/pokemon_entry.hpp>
#include <pkmn/utils/paths.hpp>

#include <boost/assign.hpp>
#include <boost/config.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <stdexcept>
#include <unordered_map>

namespace fs = boost::filesystem;

namespace pkmn { namespace database {

    static pkmn::database::sptr _db;

    /*
     * IDs and indices for hardcoded cases
     */

    BOOST_STATIC_CONSTEXPR int ROCKSTAR_PIKACHU_ID = 10182;
    BOOST_STATIC_CONSTEXPR int COSPLAY_PIKACHU_ID  = 10187;

    BOOST_STATIC_CONSTEXPR int PICHU_ID             = 172;
    BOOST_STATIC_CONSTEXPR int SPIKY_EARED_PICHU_ID = 10065;

    BOOST_STATIC_CONSTEXPR int UNOWN_INDEX          = 201;
    BOOST_STATIC_CONSTEXPR int UNOWN_B_INDEX        = 413;
    BOOST_STATIC_CONSTEXPR int UNOWN_QUESTION_INDEX = 439;

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

    BOOST_STATIC_CONSTEXPR int SWAMPERT_MEGA_ID = 10064;

    BOOST_STATIC_CONSTEXPR int VOLCANION_ID = 721;

    BOOST_STATIC_CONSTEXPR int RUBY      = 7;
    BOOST_STATIC_CONSTEXPR int EMERALD   = 9;
    BOOST_STATIC_CONSTEXPR int FIRERED   = 10;
    BOOST_STATIC_CONSTEXPR int LEAFGREEN = 11;
    BOOST_STATIC_CONSTEXPR int COLOSSEUM = 19;
    BOOST_STATIC_CONSTEXPR int XD        = 20;
    BOOST_STATIC_CONSTEXPR int X         = 23;
    BOOST_STATIC_CONSTEXPR int Y         = 24;
    BOOST_STATIC_CONSTEXPR int OMEGARUBY = 25;

    BOOST_STATIC_CONSTEXPR int RS        = 5;
    BOOST_STATIC_CONSTEXPR int HGSS      = 10;
    BOOST_STATIC_CONSTEXPR int ORAS      = 16;

    static PKMN_CONSTEXPR_OR_INLINE bool form_id_is_cosplay_pikachu(
        int form_id
    ) {
        return (form_id >= ROCKSTAR_PIKACHU_ID) and
               (form_id <= COSPLAY_PIKACHU_ID);
    }

    static PKMN_CONSTEXPR_OR_INLINE bool pokemon_index_is_unown(
        int pokemon_index,
        bool XD
    ) {
        return (pokemon_index == UNOWN_INDEX) or
               (pokemon_index >= (XD ? (UNOWN_B_INDEX + 2) : UNOWN_B_INDEX) and
                pokemon_index <= (XD ? (UNOWN_QUESTION_INDEX + 2) : UNOWN_QUESTION_INDEX));
    }

    static void _query_to_move_list(
        const char* query,
        std::vector<pkmn::e_move>& move_list_out,
        int pokemon_id,
        int version_group_id,
        bool tmhm
    )
    {
        SQLite::Statement stmt(get_connection(), query);
        stmt.bind(1, pokemon_id);
        stmt.bind(2, version_group_id);
        if(tmhm)
        {
            stmt.bind(3, version_group_id);
        }

        while(stmt.executeStep())
        {
            move_list_out.emplace_back(
                static_cast<pkmn::e_move>(int(stmt.getColumn(0)))
            );
        }
    }

    pokemon_entry::pokemon_entry(
        int pokemon_index,
        int game_id
    ):
        _pokemon_index(pokemon_index),
        _game_id(game_id),
        _none(pokemon_index == 0),
        _invalid(false),
        _shadow(false)
    {
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
        if(_none)
        {
            _species_id = _pokemon_id = _form_id = 0;
        }
        else if((_generation == 3) && pokemon_index_is_unown(
                                          _pokemon_index,
                                          (_game_id == XD)
                                      )
               )
        {
            _species_id = _pokemon_id = UNOWN_INDEX;
            _invalid = false;

            static BOOST_CONSTEXPR const char* unown_query = \
                "SELECT form_id FROM gen3_unown_game_indices "
                "WHERE game_index=?";

            _form_id = pkmn::database::query_db_bind1<int, int>(
                           unown_query, _pokemon_index
                       );

        }
        else if((_generation == 3) && (_pokemon_index == DEOXYS_GEN3_INDEX))
        {
            _species_id = DEOXYS_NORMAL_ID;
            _invalid = false;
            switch(_game_id)
            {
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
        else
        {
            static BOOST_CONSTEXPR const char* species_id_query = \
                "SELECT species_id FROM pokemon WHERE id=?";

            static BOOST_CONSTEXPR const char* form_id_query = \
                "SELECT id FROM pokemon_forms WHERE pokemon_id=?";

            try
            {
                int game_id = game_is_gamecube(_game_id) ? RUBY : _game_id;
                _pokemon_id = pkmn::database::pokemon_index_to_id(
                                  _pokemon_index, game_id
                              );
                _invalid = false;
            }
            catch(const std::invalid_argument&)
            {
                _invalid = true;
            }

            if(_invalid)
            {
                _species_id = _pokemon_id = _form_id = -1;
            }
            else
            {
                _species_id = pkmn::database::query_db_bind1<int, int>(
                                  species_id_query, _pokemon_id
                              );

                _form_id = pkmn::database::query_db_bind1<int, int>(
                               form_id_query, _pokemon_id
                           );
            }
        }
    }

    pokemon_entry::pokemon_entry(
        pkmn::e_species species,
        pkmn::e_game game,
        const std::string& form_name
    ):
        _none(species == pkmn::e_species::NONE),
        _invalid(species == pkmn::e_species::INVALID),
        _shadow(false)
    {
        /*
         * Game-related info
         */
        _game_id = pkmn::database::game_enum_to_id(
                       game
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
        if(_none)
        {
            _pokemon_id = 0;
            _species_id = 0;
            _form_id = 0;
            _pokemon_index = 0;
        }
        else if(_invalid)
        {
            _pokemon_id = -1;
            _species_id = -1;
            _form_id = -1;
            _pokemon_index = -1;
        }
        else
        {
            _species_id = static_cast<int>(species);

            set_form(form_name);
        }
    }

    std::string pokemon_entry::get_species_name() const
    {
        std::string ret;

        if(_none)
        {
            ret = "None";
        }
        else if(_invalid)
        {
            ret = str(boost::format("Invalid (0x%x)") % _pokemon_index);
        }
        else
        {
            ret = pkmn::database::species_id_to_name(
                      _species_id
                  );
        }

        return ret;
    }

    pkmn::e_game pokemon_entry::get_game() const
    {
        return pkmn::database::game_id_to_enum(_game_id);
    };

    std::string pokemon_entry::get_category() const
    {
        std::string ret;

        if(_none)
        {
            ret = "None";
        }
        else if(_invalid)
        {
            ret = "Unknown";
        }
        else
        {
            static BOOST_CONSTEXPR const char* query =
                "SELECT genus FROM pokemon_species_names WHERE "
                "pokemon_species_id=? AND local_language_id=9";

            ret = pkmn::database::query_db_bind1<std::string, int>(
                      query, _species_id
                  );
        }

        return ret;
    }

    std::string pokemon_entry::get_pokedex_entry() const {
        std::string ret;
        if(_none) {
            ret = "None";
        } else if(_invalid) {
            ret = "Unknown";
        } else {
            /*
             * Just use Ruby's Pokédex entry for the Gamecube games.
             */
            int game_id = game_is_gamecube(_game_id) ? 7 : _game_id;

            static BOOST_CONSTEXPR const char* query = \
                "SELECT flavor_text FROM pokemon_species_flavor_text WHERE "
                "species_id=? AND version_id=? AND language_id=9";

            std::string from_db = pkmn::database::query_db_bind2<std::string, int, int>(
                                      query, _species_id, game_id
                                  );
            ret = fix_veekun_whitespace(from_db);
        }

        return ret;
    }

    static std::string _get_species_standard_form(
        int species_id
    ) {
        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM libpkmn_pokemon_form_names WHERE form_id=?";

        std::string ret;
        if(not pkmn::database::maybe_query_db_bind1<std::string, int>(
                   query, ret, species_id
           ))
        {
            ret = "Standard";
        }

        return ret;
    }

    std::string pokemon_entry::get_form() const {
        std::string ret;

        if(_none) {
            ret = "None";
        } else if(_invalid) {
            ret = "Unknown";
        } else if(_shadow) {
            ret = "Shadow";
        } else if(_form_id == _species_id) {
            ret = _get_species_standard_form(_species_id);
        } else {
            static BOOST_CONSTEXPR const char* query = \
                "SELECT name FROM libpkmn_pokemon_form_names WHERE form_id=?";

            ret = pkmn::database::query_db_bind1<std::string, int>(
                      query, _form_id
                  );
        }

        return ret;
    }

    float pokemon_entry::get_height() const {
        float ret = 0.0f;
        if(_none or _invalid) {
            ret = -1.0f;
        } else {
            static BOOST_CONSTEXPR const char* query = \
                "SELECT height FROM pokemon WHERE id=?";

            double from_db = pkmn::database::query_db_bind1<double, int>(
                                 query, _pokemon_id
                             );

            ret = (float(from_db) / 10.0f);
        }

        return ret;
    }

    float pokemon_entry::get_weight() const {
        float ret = 0.0f;

        if(_none or _invalid) {
            ret = -1.0f;
        } else {
            static BOOST_CONSTEXPR const char* query = \
                "SELECT weight FROM pokemon WHERE id=?";

            double from_db = pkmn::database::query_db_bind1<double, int>(
                                 query, _pokemon_id
                             );

            ret = (float(from_db) / 10.0f);
        }

        return ret;
    }

    /*
     * The database stores gender rates oddly, so this table
     * converts them to actual probabilities.
     */
    static const std::unordered_map<int, float> _veekun_gender_rates = boost::assign::map_list_of
        (-1, 0.0f)
        (0,  1.0f)
        (1,  0.875f)
        (2,  0.75f)
        (4,  0.5f)
        (6,  0.25f)
        (8,  0.0f)
    ;

    float pokemon_entry::get_chance_male() const {
        float ret = 0.0f;

        if(_none or _invalid) {
            ret = -1.0f;
        } else {
            static BOOST_CONSTEXPR const char* query = \
                "SELECT gender_rate FROM pokemon_species WHERE id=?";

            int gender_rate_from_db = pkmn::database::query_db_bind1<int, int>(
                                          query, _species_id
                                      );
            ret = _veekun_gender_rates.at(gender_rate_from_db);
        }

        return ret;
    }

    float pokemon_entry::get_chance_female() const {
        float ret = 0.0f;

        if(_none or _invalid) {
            ret = -1.0f;
        } else {
            static BOOST_CONSTEXPR const char* query = \
                "SELECT gender_rate FROM pokemon_species WHERE id=?";

            int gender_rate_from_db = pkmn::database::query_db_bind1<int, int>(
                                          query, _species_id
                                      );
            if(gender_rate_from_db == -1) {
                ret = 0.0f;
            } else {
                ret = (1.0f - _veekun_gender_rates.at(gender_rate_from_db));
            }
        }

        return ret;
    }

    bool pokemon_entry::has_gender_differences() const {
        bool ret = true;

        // Physical gender differences started in Generation IV
        if(_none or _invalid or _generation < 4) {
            ret = false;
        } else {
            static BOOST_CONSTEXPR const char* query = \
                "SELECT has_gender_differences FROM pokemon_species WHERE id=?";

            // SQLite has no bool type, so just test the integral value
            ret = (pkmn::database::query_db_bind1<int, int>(
                      query, _species_id
                   ) > 0);
        }

        return ret;
    }

    int pokemon_entry::get_base_friendship() const {
        int ret = 0;

        // Happiness was introduced in Generation II
        if(_none or _invalid or _generation == 1) {
            ret = -1;
        } else {
            static BOOST_CONSTEXPR const char* query = \
                "SELECT base_happiness FROM pokemon_species WHERE id=?";

            ret = pkmn::database::query_db_bind1<int, int>(
                      query, _species_id
                  );
        }

        return ret;
    }

    /*
     * Some Pokémon had different types before the introduction of the Steel
     * Fairy types.
     */
    BOOST_STATIC_CONSTEXPR int old_normal_only[]    = {35,36,125,126,173,175};
    BOOST_STATIC_CONSTEXPR int old_normal_primary[] = {176,468};
    BOOST_STATIC_CONSTEXPR int old_none_secondary[] = {
        39,40,81,82,122,174,183,184,280,281,282,298,303,439,546,547
    };

    static inline bool species_id_had_normal_only(
        int species_id
    ) {
        return (std::find(old_normal_only, old_normal_only+6, species_id) != (old_normal_only+6));
    }

    static PKMN_CONSTEXPR_OR_INLINE bool species_id_had_normal_primary(
        int species_id
    ) {
        return (species_id == old_normal_primary[0]) or
               (species_id == old_normal_primary[1]);
    }

    static inline bool species_id_had_none_secondary(
        int species_id
    ) {
        return (std::find(old_none_secondary, old_none_secondary+14, species_id) != (old_none_secondary+14));
    }

    pkmn::type_pair_t pokemon_entry::get_types() const
    {
        static const pkmn::type_pair_t normal_only_pair =
        {
            pkmn::e_type::NORMAL,
            pkmn::e_type::NONE
        };

        pkmn::type_pair_t ret =
        {
            pkmn::e_type::NONE,
            pkmn::e_type::NONE
        };

        if(!_none && !_invalid)
        {
            if((_generation < 6) && species_id_had_normal_only(_species_id))
            {
                // Corner cases
                ret = normal_only_pair;
            }
            else
            {
                static const std::string first_type_query =
                    "SELECT type_id FROM pokemon_types WHERE pokemon_id=? AND slot=1";

                ret.first = static_cast<pkmn::e_type>(int(
                                pkmn::database::query_db_bind1<int, int>(
                                    first_type_query.c_str(),
                                    _pokemon_id
                                )));

                static const std::string second_type_query =
                    "SELECT type_id FROM pokemon_types WHERE pokemon_id=? AND slot=2";
                int second_type_as_int = 0;

                if(pkmn::database::maybe_query_db_bind1<int, int>(
                       second_type_query.c_str(),
                       second_type_as_int,
                       _pokemon_id
                   ))
                {
                    ret.second = static_cast<pkmn::e_type>(second_type_as_int);
                }

                if((_generation < 6) && species_id_had_normal_primary(_species_id))
                {
                    ret.first = pkmn::e_type::NORMAL;
                }
                else if((_generation < 6) && species_id_had_none_secondary(_species_id))
                {
                    ret.second = pkmn::e_type::NONE;
                }
            }
        }

        return ret;
    }

    pkmn::ability_pair_t pokemon_entry::get_abilities() const
    {
        pkmn::ability_pair_t ret =
        {
            pkmn::e_ability::NONE,
            pkmn::e_ability::NONE
        };

        // Abilities were introduced in Generation III
        if(!_none && !_invalid && (_generation >= 3))
        {
            static const std::string first_ability_query =
                "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id=? "
                "AND is_hidden=0 AND slot=1";

            // Account for abilities being added in later generations.
            static const std::string second_ability_query =
                "SELECT abilities.id FROM abilities INNER JOIN pokemon_abilities ON "
                "(abilities.id=pokemon_abilities.ability_id) WHERE pokemon_abilities.pokemon_id=? "
                "AND pokemon_abilities.is_hidden=0 AND pokemon_abilities.slot=2 AND "
                "abilities.generation_id<=?";

            ret.first = static_cast<pkmn::e_ability>(
                            pkmn::database::query_db_bind1<int, int>(
                                first_ability_query.c_str(),
                                _pokemon_id
                            )
                        );

            int second_ability_as_int = 0;
            if(pkmn::database::maybe_query_db_bind2<int, int>(
                   second_ability_query.c_str(),
                   second_ability_as_int,
                   _pokemon_id,
                   _generation))
            {
                ret.second = static_cast<pkmn::e_ability>(second_ability_as_int);
            }
        }

        return ret;
    }

    pkmn::e_ability pokemon_entry::get_hidden_ability() const
    {
        pkmn::e_ability ret = pkmn::e_ability::NONE;

        // Hidden Abilities were introduced in Generation V.
        if(!_none && !_invalid && (_generation >= 5))
        {
            static const std::string query =
                "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id=? "
                "AND is_hidden=1";

            int hidden_ability_as_int = 0;
            if(pkmn::database::maybe_query_db_bind1<int, int>(
                   query.c_str(),
                   hidden_ability_as_int,
                   _pokemon_id))
            {
                ret = static_cast<pkmn::e_ability>(hidden_ability_as_int);
            }
        }

        return ret;
    }

    pkmn::egg_group_pair_t pokemon_entry::get_egg_groups() const
    {
        pkmn::egg_group_pair_t ret =
        {
            pkmn::e_egg_group::NONE,
            pkmn::e_egg_group::NONE
        };

        // Breeding was introduced in Generation II
        if(!_none && !_invalid && (_generation >= 2))
        {
            static const std::string query =
                "SELECT egg_group_id FROM pokemon_egg_groups WHERE species_id=? ORDER BY egg_group_id";

            SQLite::Statement stmt(get_connection(), query);
            stmt.bind(1, _species_id);

            stmt.executeStep();
            ret.first = static_cast<pkmn::e_egg_group>(int(stmt.getColumn(0)));
            if(stmt.executeStep())
            {
                ret.second = static_cast<pkmn::e_egg_group>(int(stmt.getColumn(0)));
            }
            else
            {
                ret.second = pkmn::e_egg_group::NONE;
            }
        }

        return ret;
    }

    /*
     * Just return these for None and Invalid entries
     */
    static const std::map<pkmn::e_stat, int> _bad_stat_map_old = boost::assign::map_list_of
        (pkmn::e_stat::HP, 0)
        (pkmn::e_stat::ATTACK, 0)
        (pkmn::e_stat::DEFENSE, 0)
        (pkmn::e_stat::SPEED, 0)
        (pkmn::e_stat::SPECIAL, 0)
    ;
    static const std::map<pkmn::e_stat, int> _bad_stat_map = boost::assign::map_list_of
        (pkmn::e_stat::HP, 0)
        (pkmn::e_stat::ATTACK, 0)
        (pkmn::e_stat::DEFENSE, 0)
        (pkmn::e_stat::SPEED, 0)
        (pkmn::e_stat::SPECIAL_ATTACK, 0)
        (pkmn::e_stat::SPECIAL_DEFENSE, 0)
    ;

    static inline void execute_stat_stmt_and_get(
        SQLite::Statement& r_stmt,
        std::map<pkmn::e_stat, int>& r_ret,
        pkmn::e_stat key
    )
    {
        r_stmt.executeStep();
        r_ret[key] = r_stmt.getColumn(0);
    }

    std::map<pkmn::e_stat, int> pokemon_entry::get_base_stats() const
    {
        std::map<pkmn::e_stat, int> ret;

        if(_none or _invalid)
        {
            ret = (_generation == 1) ? _bad_stat_map_old
                                     : _bad_stat_map;
        }
        else
        {
            static BOOST_CONSTEXPR const char* old_query = \
                "SELECT base_stat FROM pokemon_stats WHERE pokemon_id=? AND "
                "stat_id IN (1,2,3,6,9)";

            static BOOST_CONSTEXPR const char* main_query = \
                "SELECT base_stat FROM pokemon_stats WHERE pokemon_id=? AND "
                "stat_id IN (1,2,3,4,5,6)";

            SQLite::Statement stmt(
                get_connection(),
                ((_generation == 1) ? old_query : main_query)
            );
            stmt.bind(1, _pokemon_id);

            execute_stat_stmt_and_get(stmt, ret, pkmn::e_stat::HP);
            execute_stat_stmt_and_get(stmt, ret, pkmn::e_stat::ATTACK);
            execute_stat_stmt_and_get(stmt, ret, pkmn::e_stat::DEFENSE);
            if(_generation == 1) {
                execute_stat_stmt_and_get(stmt, ret, pkmn::e_stat::SPEED);
                execute_stat_stmt_and_get(stmt, ret, pkmn::e_stat::SPECIAL);
            } else {
                execute_stat_stmt_and_get(stmt, ret, pkmn::e_stat::SPECIAL_ATTACK);
                execute_stat_stmt_and_get(stmt, ret, pkmn::e_stat::SPECIAL_DEFENSE);
                execute_stat_stmt_and_get(stmt, ret, pkmn::e_stat::SPEED);
            }
        }

        return ret;
    }

    std::map<pkmn::e_stat, int> pokemon_entry::get_EV_yields() const
    {
        std::map<pkmn::e_stat, int> ret;

        if(_none or _invalid)
        {
            ret = (_generation <= 2) ? _bad_stat_map_old
                                     : _bad_stat_map;
        }
        else if(_generation == 1)
        {
            // EV's are just base stats
            ret = this->get_base_stats();
        }
        else if(_generation == 2)
        {
            // EV's almost match base stats but just have Special
            ret = this->get_base_stats();
            ret[pkmn::e_stat::SPECIAL] = ret[pkmn::e_stat::SPECIAL_ATTACK];
            ret.erase(pkmn::e_stat::SPECIAL_ATTACK);
            ret.erase(pkmn::e_stat::SPECIAL_DEFENSE);
        }
        else
        {
            static BOOST_CONSTEXPR const char* query = \
                "SELECT effort FROM pokemon_stats WHERE pokemon_id=? AND "
                "stat_id IN (1,2,3,4,5,6)";

            SQLite::Statement stmt(get_connection(), query);
            stmt.bind(1, _pokemon_id);

            execute_stat_stmt_and_get(stmt, ret, pkmn::e_stat::HP);
            execute_stat_stmt_and_get(stmt, ret, pkmn::e_stat::ATTACK);
            execute_stat_stmt_and_get(stmt, ret, pkmn::e_stat::DEFENSE);
            execute_stat_stmt_and_get(stmt, ret, pkmn::e_stat::SPECIAL_ATTACK);
            execute_stat_stmt_and_get(stmt, ret, pkmn::e_stat::SPECIAL_DEFENSE);
            execute_stat_stmt_and_get(stmt, ret, pkmn::e_stat::SPEED);
        }

        return ret;
    }

    /*
     * Experience yields were rebalanced in Generation V, so which
     * value to return depends on what generation the entry came from.
     * However, there are some exceptions where the value was different
     * in Generation IV as well.
     */
    static const std::unordered_map<int, int> gen4_different_yields = boost::assign::map_list_of
        (63,75)(66,75)(74,73)(138,99)(140,99)(141,199)
        (206,125)(269,161)(345,99)(346,199)(347,99)(348,199)
    ;

    inline bool gen4_has_different_yield(
        int species_id
    ) {
        return (gen4_different_yields.count(species_id) > 0);
    }

    int pokemon_entry::get_experience_yield() const {
        int ret = 0;

        if(_none or _invalid) {
            ret = -1;
        } else {
            static BOOST_CONSTEXPR const char* old_query = \
                "SELECT exp_yield FROM old_exp_yields WHERE species_id=?";

            static BOOST_CONSTEXPR const char* main_query = \
                "SELECT base_experience FROM pokemon WHERE id=?";

            if(_generation == 4 and gen4_has_different_yield(_species_id)) {
                ret = gen4_different_yields.at(_species_id);
            } else if(_generation < 5) {
                ret = pkmn::database::query_db_bind1<int, int>(
                          old_query, _species_id
                      );
            } else {
                ret = pkmn::database::query_db_bind1<int, int>(
                          main_query, _pokemon_id
                      );
            }
        }

        return ret;
    }

    int pokemon_entry::get_experience_at_level(
        int level
    ) const
    {
        // Allow for glitch Pokémon
        pkmn::enforce_bounds(
            "Level",
            level,
            0,
            255
        );

        int ret = 0;

        if(_none or _invalid)
        {
            ret = -1;
        }
        else
        {
            static BOOST_CONSTEXPR const char* query = \
                "SELECT experience.experience FROM pokemon_species "
                "INNER JOIN experience ON pokemon_species.growth_rate_id=experience.growth_rate_id "
                "WHERE (pokemon_species.id=? AND experience.level=?) "
                "ORDER BY experience.experience";

            ret = pkmn::database::query_db_bind2<int, int, int>(
                      query, _species_id, level
                  );
        }

        return ret;
    }

    int pokemon_entry::get_level_at_experience(
        int experience
    ) const
    {
        int ret = 0;

        pkmn::enforce_comparator(
            "Experience",
            experience,
            0,
            pkmn::value_comparator::GE
        );

        if(_none or _invalid)
        {
            ret = -1;
        }
        else
        {
            static BOOST_CONSTEXPR const char* query = \
                "SELECT experience.level FROM experience "
                "INNER JOIN pokemon_species "
                "ON experience.growth_rate_id=pokemon_species.growth_rate_id "
                "AND experience.growth_rate_id=pokemon_species.growth_rate_id "
                "WHERE (experience.experience<=? "
                "AND pokemon_species.id=?) ORDER BY experience.level DESC";

            ret = pkmn::database::query_db_bind2<int, int, int>(
                      query, experience, _species_id
                  );
        }

        return ret;
    }

    pkmn::database::levelup_moves_t pokemon_entry::get_levelup_moves() const
    {
        pkmn::database::levelup_moves_t ret;

        if(!_none && !_invalid)
        {
            static const std::string query =
                "SELECT move_id,level FROM pokemon_moves WHERE pokemon_id=? "
                "AND version_group_id=? AND pokemon_move_method_id=1 ORDER BY level";

            SQLite::Statement stmt(get_connection(), query);
            stmt.bind(1, _pokemon_id);
            stmt.bind(2, _version_group_id);
            while(stmt.executeStep())
            {
                ret.emplace_back(
                    static_cast<pkmn::e_move>(int(stmt.getColumn(0))),
                    int(stmt.getColumn(1))
                );
            }
        }

        return ret;
    }

    std::vector<pkmn::e_move> pokemon_entry::get_tm_hm_moves() const
    {
        std::vector<pkmn::e_move> ret;

        if(!_none && !_invalid)
        {
            static const std::string& query =
                "SELECT move_id FROM machines WHERE move_id IN "
                "(SELECT move_id FROM pokemon_moves WHERE pokemon_move_method_id=4 AND "
                "pokemon_id=? AND version_group_id=?) AND version_group_id=? ORDER BY "
                "machine_number";

            // Gamecube results match Ruby/Sapphire, so use that instead
            int version_group_id = 0;
            if(game_is_gamecube(_game_id))
            {
                version_group_id = RS;
            }
            else
            {
                version_group_id = _version_group_id;
            }

            _query_to_move_list(
                query.c_str(), ret, _pokemon_id, version_group_id, true
            );
        }

        return ret;
    }

    /*
     * Veekun's database only stores egg moves for unevolved Pokemon, so we need to
     * figure out this Pokemon's earliest evolution and use that for the query.
     */
    std::vector<pkmn::e_move> pokemon_entry::get_egg_moves() const
    {
        std::vector<pkmn::e_move> ret;

        if(!_none && !_invalid)
        {
            static const std::string evolution_query =
                "SELECT evolves_from_species_id FROM pokemon_species WHERE id=?";

            static const std::string move_query =
                "SELECT move_id FROM pokemon_moves WHERE pokemon_move_method_id=2 AND "
                "pokemon_id=? AND version_group_id=?";

            int species_id = _species_id;
            SQLite::Statement stmt(get_connection(), evolution_query.c_str());
            stmt.bind(1, species_id);
            while(stmt.executeStep())
            {
                // The final query will be valid but return 0, which we can't use
                if(int(stmt.getColumn(0)) == 0)
                {
                    break;
                }
                species_id = stmt.getColumn(0);
                stmt.reset();
                stmt.clearBindings();
                stmt.bind(1, species_id);
            }

            // Gamecube results match Ruby/Sapphire, so use that instead
            int version_group_id = 0;
            if(game_is_gamecube(_game_id))
            {
                version_group_id = RS;
            }
            else
            {
                version_group_id = _version_group_id;
            }

            _query_to_move_list(
                move_query.c_str(), ret, species_id, version_group_id, false
            );
        }

        return ret;
    }

    std::vector<pkmn::e_move> pokemon_entry::get_tutor_moves() const
    {
        std::vector<pkmn::e_move> ret;

        if(!_none && !_invalid)
        {
            static const std::string query =
                "SELECT move_id FROM pokemon_moves WHERE pokemon_move_method_id=3 AND "
                "pokemon_id=? AND version_group_id=?";

            _query_to_move_list(
                query.c_str(), ret, _pokemon_id, _version_group_id, false
            );
        }

        return ret;
    }

    std::vector<std::string> pokemon_entry::get_forms() const {
        std::vector<std::string> ret;
        if(not (_none or _invalid)) {
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
                    form_ids_query, ret, _version_group_id, _species_id, _species_id
                );
            }

            if(_species_id == ARCEUS_ID and _generation == 4) {
                ret.emplace_back("???");
            }

            // If this is a Gamecube game, check for a Shadow form.
            if(_game_id == COLOSSEUM or _game_id == XD)
            {
                static const char* shadow_query = \
                    "SELECT shadow_pokemon_id FROM shadow_pokemon WHERE species_id=? AND "
                    "colosseum=?";

                int shadow_pokemon_id = -1;
                bool has_shadow = pkmn::database::maybe_query_db_bind2<int, int, int>(
                                      shadow_query, shadow_pokemon_id, _species_id,
                                      ((_game_id == COLOSSEUM) ? 1 : 0)
                                  );
                if(has_shadow)
                {
                    ret.emplace_back("Shadow");
                }
            }
        }

        return ret;
    }

    std::vector<pkmn::e_species> pokemon_entry::get_evolutions() const
    {
        std::vector<pkmn::e_species> ret;

        if(!_none && !_invalid)
        {
            static const std::string query =
                "SELECT id FROM pokemon_species WHERE evolves_from_species_id=? "
                "AND generation_id<=?";

            SQLite::Statement stmt(get_connection(), query);
            stmt.bind(1, _species_id);
            stmt.bind(2, _generation);
            while(stmt.executeStep())
            {
                ret.emplace_back(
                    static_cast<pkmn::e_species>(int(stmt.getColumn(0)))
                );
            }
        }

        return ret;
    }

    void pokemon_entry::set_form(
        const std::string& form_name
    ) {
        if(not (_none or _invalid)) {
            if(form_name == "") {
                _form_id = _pokemon_id = _species_id;
            } else {
                /*
                 * Start by checking which forms exist in this game. If not, immediately
                 * throw an error.
                 */
                std::vector<std::string> forms = this->get_forms();
                auto form_iter = std::find(forms.begin(), forms.end(), form_name);
                if(form_iter == forms.end()) {
                    throw std::invalid_argument("Invalid form.");
                } else if(form_iter == forms.begin()) {
                    // Standard form, whatever that may be named
                    _form_id = _pokemon_id = _species_id;
                    if(game_is_gamecube(_game_id))
                    {
                        _shadow = false;
                    }
                } else {

                    if(game_is_gamecube(_game_id) and form_name == "Shadow")
                    {
                        // Track this separately because there are no IDs associated with it.
                        set_form(forms.front());
                        _shadow = true;
                    }
                    else
                    {
                        if(game_is_gamecube(_game_id))
                        {
                            _shadow = false;
                        }

                        // Set the form and Pokémon ID
                        static BOOST_CONSTEXPR const char* query = \
                            "SELECT id,pokemon_id FROM pokemon_forms WHERE id="
                            "(SELECT form_id FROM libpkmn_pokemon_form_names WHERE name=?) "
                            "AND pokemon_id IN (SELECT id FROM pokemon WHERE species_id=?)";

                        SQLite::Statement stmt(get_connection(), query);
                        stmt.bind(1, form_name);
                        stmt.bind(2, _species_id);
                        stmt.executeStep();
                        /*
                         * Now that we have the form ID, check some of the hardcoded cases
                         * before assigning the proper IDs.
                         */
                        int form_id = stmt.getColumn(0);

                        if(_generation == 3 and _species_id == DEOXYS_ID) {
                            switch(_game_id) {
                                case FIRERED:
                                    if(form_id != DEOXYS_ATTACK_FORM_ID) {
                                        throw std::invalid_argument(
                                                  "Deoxys can only be in its Attack Forme in FireRed."
                                              );
                                    }
                                    break;

                                case LEAFGREEN:
                                    if(form_id != DEOXYS_DEFENSE_FORM_ID) {
                                        throw std::invalid_argument(
                                                  "Deoxys can only be in its Defense Forme in LeafGreen."
                                              );
                                    }
                                    break;

                                case EMERALD:
                                    if(form_id != DEOXYS_SPEED_FORM_ID) {
                                        throw std::invalid_argument(
                                                  "Deoxys can only be in its Speed Forme in Emerald."
                                              );
                                    }
                                    break;

                                default:
                                    if(form_id != _species_id) {
                                        throw std::invalid_argument(
                                            str(boost::format("Deoxys can only be in its Normal Forme in %s.")
                                                    % game_enum_to_name(this->get_game()).c_str()
                                               )
                                        );
                                    }
                                    break;
                            }

                            _pokemon_index = DEOXYS_GEN3_INDEX;
                        } else if(_version_group_id != HGSS and form_id == SPIKY_EARED_PICHU_ID) {
                            throw pkmn::feature_not_in_game_error("Spiky-Eared Pichu is only in HeartGold/SoulSilver.");
                        } else if(form_id_is_cosplay_pikachu(form_id) and _version_group_id != ORAS) {
                            throw std::invalid_argument(
                                str(boost::format("%s can only be in its Normal Forme in X/Y.")
                                        % form_name.c_str()
                                   )
                            );
                        }

                        _form_id    = stmt.getColumn(0);
                        _pokemon_id = stmt.getColumn(1);
                    }
                }
            }

            if(_generation == 3 and _species_id == UNOWN_INDEX) {
                static BOOST_CONSTEXPR const char* unown_query = \
                    "SELECT game_index FROM gen3_unown_game_indices WHERE form_id=?";

                _pokemon_index = pkmn::database::query_db_bind1<int, int>(
                                     unown_query, _form_id
                                 );
            } else {
                static BOOST_CONSTEXPR const char* pokemon_index_query = \
                    "SELECT game_index FROM pokemon_game_indices WHERE pokemon_id=? "
                    "AND version_id=?";

                /*
                 * Some of the Veekun database's contributors haven't taken into account
                 * that the "pokemon_game_indices" table stores indices by version_id, so
                 * indices need to be added for ALL games they apply to. Here, we're just
                 * hoping they added to at least the lowest-indexed game.
                 */
                int game_id = 0;
                if(game_is_gamecube(_game_id)) {
                    game_id = RUBY;
                } else if(_game_id == Y) {
                    game_id = X;
                } else if(_game_id >= OMEGARUBY) {
                    game_id = (_form_id >= SWAMPERT_MEGA_ID) ? OMEGARUBY : X;
                } else {
                    game_id = _game_id;
                }

                /*
                 * Not every form has an index, so if this query fails, default to the primary one
                 * and hope for the best.
                 */
                if(not pkmn::database::maybe_query_db_bind2<int, int, int>(
                           pokemon_index_query, _pokemon_index, _pokemon_id, game_id
                       )
                ) {
                    _pokemon_index = pkmn::database::query_db_bind2<int, int, int>(
                                         pokemon_index_query, _species_id, game_id
                                     );
                }
            }
        }
    }

    static BOOST_CONSTEXPR const char* IMAGES_SUBDIR_STRINGS[] = {
        "",
        "red-blue",
        "red-blue",
        "yellow",
        "gold",
        "silver",
        "crystal",
        "ruby-sapphire",
        "ruby-sapphire",
        "emerald",
        "firered-leafgreen",
        "firered-leafgreen",
        "diamond-pearl",
        "diamond-pearl",
        "platinum",
        "heartgold-soulsilver",
        "heartgold-soulsilver",
        "black-white",
        "black-white",

        // Use Ruby/Sapphire sprites for Gamecube games
        "ruby-sapphire",
        "ruby-sapphire",

        "black2-white2",
        "black2-white2",
        "x-y",
        "x-y",
        "omegaruby-alphasapphire",
        "omegaruby-alphasapphire"
    };

    static BOOST_CONSTEXPR const char* image_name_query = \
        "SELECT image_name FROM libpkmn_pokemon_form_names WHERE form_id=?";

    static inline bool has_different_female_icon(
        int species_id
    ) {
        return (species_id == 521 or species_id == 592 or species_id == 593);
    }

    std::string pokemon_entry::get_icon_filepath(
        bool female
    ) const {
        if(_species_id > VOLCANION_ID) {
            throw pkmn::unimplemented_error();
        }

        fs::path icon_filepath(pkmn::get_images_dir());
        icon_filepath /= "pokemon-icons";
        if(_none or _invalid) {
            icon_filepath /= "0.png";
        } else {
            if(female and has_different_female_icon(_species_id)) {
                icon_filepath /= "female";
            }

            std::string form_suffix;
            (void)pkmn::database::maybe_query_db_bind1<std::string, int>(
                      image_name_query, form_suffix, _form_id
                  );
            if(not form_suffix.empty()) {
                form_suffix.insert(0, "-");
            }

            icon_filepath /= str(boost::format("%d%s.png") % _species_id % form_suffix.c_str());
        }

        return icon_filepath.string();
    }

    std::string pokemon_entry::get_sprite_filepath(
        bool female,
        bool shiny
    ) const {
        if(_generation > 5) {
            throw pkmn::unimplemented_error();
        }

        fs::path sprite_filepath(pkmn::get_images_dir());
        sprite_filepath /= str(boost::format("generation-%d") % _generation);
        sprite_filepath /= IMAGES_SUBDIR_STRINGS[_game_id];

        if(_none) {
            sprite_filepath /= "0.png";
        } else if(_invalid) {
            sprite_filepath /= "substitute.png";
        } else {
            if(shiny) {
                if(_generation > 1) {
                    sprite_filepath /= "shiny";
                } else {
                    throw pkmn::feature_not_in_game_error("Shininess", "Generation I");
                }
            }
            if(female and has_gender_differences()) {
                sprite_filepath /= "female";
            }

            std::string form_suffix;
            (void)pkmn::database::maybe_query_db_bind1<std::string, int>(
                      image_name_query, form_suffix, _form_id
                  );
            if(not form_suffix.empty()) {
                form_suffix.insert(0, "-");
            }

            sprite_filepath /= str(boost::format("%d%s.png") % _species_id % form_suffix.c_str());
        }

        return sprite_filepath.string();
    }

}}

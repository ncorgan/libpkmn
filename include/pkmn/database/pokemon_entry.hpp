/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_DATABASE_POKEMON_ENTRY_HPP
#define PKMN_DATABASE_POKEMON_ENTRY_HPP

#include <pkmn/config.hpp>

#include <pkmn/database/levelup_move.hpp>
#include <pkmn/database/move_entry.hpp>

#include <map>
#include <string>
#include <vector>

namespace pkmn { namespace database {

    /*
     * We need this forward declaration to use the typedef
     * in the class itself.
     */
    class pokemon_entry;

    //! List of Pokémon entries.
    typedef std::vector<pokemon_entry> pokemon_entries_t;

    /*
     * TODO:
     *  * Fixes from unit test
     *  * See which ctors, move+copy ops, and ID getters can be constexpr'd
     *  * Can any other info be added?
     */
    class PKMN_API pokemon_entry {
        public:
            PKMN_CONSTEXPR_OR_INLINE pokemon_entry():
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

            #ifndef __DOXYGEN__
            pokemon_entry(
                int pokemon_index,
                int game_id
            );
            #endif

            pokemon_entry(
                const std::string &species_name,
                const std::string &game_name,
                const std::string &form_name
            );

            std::string get_name() const;

            std::string get_game() const;

            std::string get_species() const;

            std::string get_pokedex_entry() const;

            std::string get_form() const;

            float get_height() const;

            float get_weight() const;

            float get_chance_male() const;

            float get_chance_female() const;

            bool has_gender_differences() const;

            int get_base_friendship() const;

            std::pair<std::string, std::string> get_types() const;

            std::pair<std::string, std::string> get_abilities() const;

            std::string get_hidden_ability() const;

            std::pair<std::string, std::string> get_egg_groups() const;

            std::map<std::string, int> get_base_stats() const;

            std::map<std::string, int> get_EV_yields() const;

            int get_experience_yield() const;

            int get_experience_at_level(
                int level
            ) const;

            int get_level_at_experience(
                int experience
            ) const;

            pkmn::database::levelup_moves_t get_levelup_moves() const;

            pkmn::database::move_list_t get_tm_hm_moves() const;

            pkmn::database::move_list_t get_egg_moves() const;

            pkmn::database::move_list_t get_tutor_moves() const;

            std::vector<std::string> get_forms() const;

            pkmn::database::pokemon_entries_t get_evolutions() const;

            void set_form(
                const std::string &form_name
            );

            #ifndef __DOXYGEN__
            PKMN_CONSTEXPR_OR_INLINE int get_species_id() const {
                return _species_id;
            }

            PKMN_CONSTEXPR_OR_INLINE int get_pokemon_id() const {
                return _pokemon_id;
            }

            PKMN_CONSTEXPR_OR_INLINE int get_form_id() const {
                return _form_id;
            }

            PKMN_CONSTEXPR_OR_INLINE int get_pokemon_index() const {
                return _pokemon_index;
            }

            PKMN_CONSTEXPR_OR_INLINE int get_game_id() const {
                return _game_id;
            }
            #endif

            //! Equality check between two Pokémon entries
            PKMN_CONSTEXPR_OR_INLINE bool operator==(
                const pokemon_entry &rhs
            ) const {
                return ((this->_game_id == rhs._game_id) and
                        (this->_pokemon_id == rhs._pokemon_id));
            }

            //! Inequality check between two Pokémon entries
            PKMN_CONSTEXPR_OR_INLINE bool operator!=(
                const pokemon_entry &rhs
            ) const {
                return ((this->_game_id != rhs._game_id) or
                        (this->_pokemon_id != rhs._pokemon_id));
            }

        private:
            int _species_id, _pokemon_id, _form_id, _pokemon_index;
            int _game_id, _generation, _version_group_id;
            bool _none, _invalid;
    };

}}
#endif /* PKMN_DATABASE_POKEMON_ENTRY_HPP */

/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
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

    /*!
     * @brief Class representing a database entry for a Pokémon.
     *
     * This class abstracts the database calls needed to various tables in order
     * to get relevant information about in-game species. It takes into account
     * changes in the move between games and can be set to correspond to any
     * of the Pokémon's forms.
     */
    class PKMN_API pokemon_entry
    {
        public:
            /*!
             * @brief Default constructor. Corresponds to an invalid Pokémon.
             */
            pokemon_entry():
                _species_id(0),
                _pokemon_id(0),
                _form_id(0),
                _pokemon_index(0),
                _game_id(0),
                _generation(0),
                _version_group_id(0),
                _none(true),
                _invalid(false),
                _shadow(false)
            {}

#if !defined(__DOXYGEN__) && !defined(SWIG)
            pokemon_entry(
                int pokemon_index,
                int game_id
            );
#endif

            /*!
             * @brief A constructor taking in a species, game, and, optionally, a form.
             *
             * To use the Pokémon's default form, pass an empty string into the form_name
             * parameter.
             *
             * \throws std::invalid_argument If the species or game are invalid
             * \throws std::invalid_argument If the form is invalid for the given Pokémon and game
             */
            pokemon_entry(
                const std::string& species_name,
                const std::string& game_name,
                const std::string& form_name
            );

            pokemon_entry(const pokemon_entry&) = default;
            pokemon_entry& operator=(const pokemon_entry&) = default;

#ifndef SWIG
            pokemon_entry(pokemon_entry&&) = default;
            pokemon_entry& operator=(pokemon_entry&&) = default;
#endif

            /*!
             * @brief Returns the Pokémon's name.
             *
             * \returns For "None" entries: "None"
             * \returns For invalid Pokémon: "Invalid (0xXX)", where XX corresponds
             *          to the Pokémon's in-game index in hex.
             */
            std::string get_name() const;

            /*!
             * @brief Returns the game this entry corresponds to.
             */
            std::string get_game() const;

            /*!
             * @brief Returns the Pokémon's species.
             *
             * This is the classification given in the Pokédex, not the name
             * of the Pokémon. For example, for a Pikachu entry, get_name()
             * will return "Pikachu", and this function will return "Mouse".
             *
             * \returns For "None" entries: "None"
             * \returns For invalid Pokémon: "Unknown"
             */
            std::string get_species() const;

            /*!
             * @brief Returns this Pokémon's Pokédex entry for the given game.
             *
             * \returns For "None" entries: "None"
             * \returns For invalid Pokémon: "Unknown"
             */
            std::string get_pokedex_entry() const;

            /*!
             * @brief Returns the form this entry corresponds to.
             *
             * \returns For "None" entries: "None"
             * \returns For invalid Pokémon: "Unknown"
             */
            std::string get_form() const;

            /*!
             * @brief Returns the Pokémon's height in meters.
             *
             * \returns For "None" entries: 0.0
             * \returns For invalid Pokémon: -1.0
             */
            float get_height() const;

            /*!
             * @brief Returns the Pokémon's weight in kilograms.
             *
             * \returns For "None" entries: 0.0
             * \returns For invalid Pokémon: -1.0
             */
            float get_weight() const;

            /*!
             * @brief Returns the chance a random instance of this Pokémon
             *        will be male.
             *
             * If a Pokémon is genderless or female-only, this value will be 0.0.
             *
             * \returns For "None" entries: 0.0
             * \returns For invalid Pokémon: -1.0
             * \returns Otherwise: a number in the range [0.0-1.0]
             */
            float get_chance_male() const;

            /*!
             * @brief Returns the chance a random instance of this Pokémon
             *        will be male.
             *
             * If a Pokémon is genderless or male-only, this value will be 0.0.
             *
             * \returns For "None" entries: 0.0
             * \returns For invalid Pokémon: -1.0
             * \returns Otherwise: a number in the range [0.0-1.0]
             */
            float get_chance_female() const;

            /*!
             * @brief Returns whether there are visual differences between male
             *        and female members of this species.
             *
             * This concept was introduced in Generation IV, so even if a Pokémon
             * has visual differences in Generation IV, this function will return
             * false for the same species in earlier games.
             *
             * This function will always return false for genderless Pokémon.
             *
             * \returns For Generation I-III entries: false
             * \returns For None/invalid Pokémon: false
             */
            bool has_gender_differences() const;

            /*!
             * @brief Returns the initial friendship value this Pokémon will have
             *        when caught.
             *
             * This concept was introduced in Generation II, so for Generation I
             * entries, this function will always return 0.
             *
             * \returns For Generation I entries: 0
             * \returns For None/invalid Pokémon: -1
             * \returns Otherwise: a number in the range [0-255]
             */
            int get_base_friendship() const;

            /*!
             * @brief Returns this Pokémon's types.
             *
             * This value can vary between generations and between forms.
             *
             * If the Pokémon has a single type, the second value will be "None".
             *
             * \returns For "None" entries: "None"/"None"
             * \returns For invalid Pokémon: "Unknown"/"Unknown"
             */
            std::pair<std::string, std::string> get_types() const;

            /*!
             * @brief Returns this Pokémon's types.
             *
             * Abilities were introduced in Generation III, so for entries from
             * previous generations, this function will always return "None"/"None".
             *
             * If the Pokémon has a single ability, the second value will be "None".
             *
             * This value can vary between forms.
             *
             * \returns For Generation I-II entries: "None"/"None"
             * \returns For "None" entries: "None"/"None"
             * \returns For invalid Pokémon: "Unknown"/"Unknown"
             */
            std::pair<std::string, std::string> get_abilities() const;

            /*!
             * @brief Returns this Pokémon's hidden ability.
             *
             * Hidden Abilities were introduced in Generation V, so for entries
             * corresponding to earlier games, this function will always return
             * "None".
             *
             * \returns For Generation I-IV entries: "None"
             * \returns For "None" entries: "None"
             * \returns For invalid Pokémon: "Unknown"
             */
            std::string get_hidden_ability() const;

            /*!
             * @brief Returns this Pokémon's egg groups.
             *
             * Breeding was introduced in Generation II, so for entries from
             * Generation I, this function will always return "None"/"None".
             *
             * If the Pokémon has a single egg group, the second value will be "None".
             *
             * \returns For Generation I entries: "None"/"None"
             * \returns For "None" entries: "None"/"None"
             * \returns For invalid Pokémon: "Unknown"/"Unknown"
             */
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
                const std::string& form_name
            );

            std::string get_icon_filepath(
                bool female
            ) const;

            std::string get_sprite_filepath(
                bool female,
                bool shiny
            ) const;

            #ifndef __DOXYGEN__
            inline int get_species_id() const
            {
                return _species_id;
            }

            inline int get_pokemon_id() const
            {
                return _pokemon_id;
            }

            inline int get_form_id() const
            {
                return _form_id;
            }

            inline int get_pokemon_index() const
            {
                return _pokemon_index;
            }

            inline int get_game_id() const
            {
                return _game_id;
            }
            #endif

            //! Equality check between two Pokémon entries
            inline bool operator==(const pokemon_entry& rhs) const
            {
                return ((this->_game_id == rhs._game_id) and
                        (this->_pokemon_id == rhs._pokemon_id));
            }

            //! Inequality check between two Pokémon entries
            inline bool operator!=(const pokemon_entry& rhs) const
            {
                return !operator==(rhs);
            }

        private:
            int _species_id, _pokemon_id, _form_id, _pokemon_index;
            int _game_id, _generation, _version_group_id;
            bool _none, _invalid, _shadow;
    };

}}
#endif /* PKMN_DATABASE_POKEMON_ENTRY_HPP */

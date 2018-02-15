/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_WRAPPERS_POKEMON_HPP
#define CPP_WRAPPERS_POKEMON_HPP

#include "exception_internal.hpp"

#include <pkmn/config.hpp>
#include <pkmn/exception.hpp>
#include <pkmn/pokemon.hpp>

namespace pkmn { namespace swig {

    class pokemon_EV_map
    {
        public:
            pokemon_EV_map():
                _pokemon(nullptr)
            {}

            pokemon_EV_map(
                pkmn::pokemon::sptr pokemon
            ): _pokemon(pokemon)
            {}

            inline int get_EV(
                const std::string& stat
            )
            {
                if(!_pokemon)
                {
                    throw std::runtime_error("This class should only be used as a member of another class, rather than standalone.");
                }

                const std::map<std::string, int>& EVs = _pokemon->get_EVs();
                if(EVs.count(stat) == 0)
                {
                    throw std::invalid_argument("Invalid stat.");
                }

                return EVs.at(stat);
            }

            inline void set_EV(
                const std::string& stat,
                int value
            )
            {
                if(!_pokemon)
                {
                    throw std::runtime_error("This class should only be used as a member of another class, rather than standalone.");
                }

                _pokemon->set_EV(stat, value);
            }

            inline bool has_key(
                const std::string& key
            )
            {
                return (_pokemon->get_EVs().count(key) > 0);
            }

            inline pkmn::pokemon::sptr get_internal()
            {
                return _pokemon;
            }

        private:
            pkmn::pokemon::sptr _pokemon;
    };

    class pokemon_IV_map
    {
        public:
            pokemon_IV_map():
                _pokemon(nullptr)
            {}

            pokemon_IV_map(
                pkmn::pokemon::sptr pokemon
            ): _pokemon(pokemon)
            {}

            inline int get_IV(
                const std::string& stat
            )
            {
                if(!_pokemon)
                {
                    throw std::runtime_error("This class should only be used as a member of another class, rather than standalone.");
                }

                const std::map<std::string, int>& IVs = _pokemon->get_IVs();
                if(IVs.count(stat) == 0)
                {
                    throw std::invalid_argument("Invalid stat.");
                }

                return IVs.at(stat);
            }

            inline void set_IV(
                const std::string& stat,
                int value
            )
            {
                if(!_pokemon)
                {
                    throw std::runtime_error("This class should only be used as a member of another class, rather than standalone.");
                }

                _pokemon->set_IV(stat, value);
            }

            inline bool has_key(
                const std::string& key
            )
            {
                return (_pokemon->get_IVs().count(key) > 0);
            }

            inline pkmn::pokemon::sptr get_internal()
            {
                return _pokemon;
            }

        private:
            pkmn::pokemon::sptr _pokemon;
    };

    class pokemon_marking_map
    {
        public:
            pokemon_marking_map():
                _pokemon(nullptr)
            {}

            pokemon_marking_map(
                pkmn::pokemon::sptr pokemon
            ): _pokemon(pokemon)
            {}

            inline bool get_marking(
                const std::string& marking
            )
            {
                if(!_pokemon)
                {
                    throw std::runtime_error("This class should only be used as a member of another class, rather than standalone.");
                }

                const std::map<std::string, bool>& markings = _pokemon->get_markings();
                if(markings.count(marking) == 0)
                {
                    throw std::invalid_argument("Invalid marking.");
                }

                return markings.at(marking);
            }

            inline void set_marking(
                const std::string& stat,
                bool value
            )
            {
                if(!_pokemon)
                {
                    throw std::runtime_error("This class should only be used as a member of another class, rather than standalone.");
                }

                _pokemon->set_marking(stat, value);
            }

            inline bool has_key(
                const std::string& key
            )
            {
                return (_pokemon->get_markings().count(key) > 0);
            }

            inline pkmn::pokemon::sptr get_internal()
            {
                return _pokemon;
            }

        private:
            pkmn::pokemon::sptr _pokemon;
    };

    class pokemon_ribbon_map
    {
        public:
            pokemon_ribbon_map():
                _pokemon(nullptr)
            {}

            pokemon_ribbon_map(
                pkmn::pokemon::sptr pokemon
            ): _pokemon(pokemon)
            {}

            inline bool get_ribbon(
                const std::string& ribbon
            )
            {
                if(!_pokemon)
                {
                    throw std::runtime_error("This class should only be used as a member of another class, rather than standalone.");
                }

                const std::map<std::string, bool>& ribbons = _pokemon->get_ribbons();
                if(ribbons.count(ribbon) == 0)
                {
                    throw std::invalid_argument("Invalid ribbon.");
                }

                return ribbons.at(ribbon);
            }

            inline void set_ribbon(
                const std::string& stat,
                bool value
            )
            {
                if(!_pokemon)
                {
                    throw std::runtime_error("This class should only be used as a member of another class, rather than standalone.");
                }

                _pokemon->set_ribbon(stat, value);
            }

            inline bool has_key(
                const std::string& key
            )
            {
                return (_pokemon->get_ribbons().count(key) > 0);
            }

            inline pkmn::pokemon::sptr get_internal()
            {
                return _pokemon;
            }

        private:
            pkmn::pokemon::sptr _pokemon;
    };

    class pokemon_contest_stat_map
    {
        public:
            pokemon_contest_stat_map():
                _pokemon(nullptr)
            {}

            pokemon_contest_stat_map(
                pkmn::pokemon::sptr pokemon
            ): _pokemon(pokemon)
            {}

            inline int get_contest_stat(
                const std::string& stat
            )
            {
                if(!_pokemon)
                {
                    throw std::runtime_error("This class should only be used as a member of another class, rather than standalone.");
                }

                const std::map<std::string, int>& contest_stats = _pokemon->get_contest_stats();
                if(contest_stats.count(stat) == 0)
                {
                    throw std::invalid_argument("Invalid contest stat.");
                }

                return contest_stats.at(stat);
            }

            inline void set_contest_stat(
                const std::string& stat,
                int value
            )
            {
                if(!_pokemon)
                {
                    throw std::runtime_error("This class should only be used as a member of another class, rather than standalone.");
                }

                _pokemon->set_contest_stat(stat, value);
            }

            inline bool has_key(
                const std::string& key
            )
            {
                return (_pokemon->get_contest_stats().count(key) > 0);
            }

            inline pkmn::pokemon::sptr get_internal()
            {
                return _pokemon;
            }

        private:
            pkmn::pokemon::sptr _pokemon;
    };

    class pokemon_move_slot_wrapper
    {
        public:
            pokemon_move_slot_wrapper():
                _pokemon(nullptr),
                _index(0)
            {}

            pokemon_move_slot_wrapper(
                const pkmn::pokemon::sptr& cpp_pokemon,
                int index
            ): _pokemon(cpp_pokemon),
               _index(index)
            {}

            const std::string& get_move()
            {
                if(!_pokemon)
                {
                    throw std::runtime_error("This class should only be used as a member of another class, rather than standalone.");
                }

                return _pokemon->get_moves().at(_index).move;
            }

            void set_move(
                const std::string& move
            )
            {
                if(!_pokemon)
                {
                    throw std::runtime_error("This class should only be used as a member of another class, rather than standalone.");
                }

                _pokemon->set_move(move, _index);
            }

            int get_pp()
            {
                if(!_pokemon)
                {
                    throw std::runtime_error("This class should only be used as a member of another class, rather than standalone.");
                }

                return _pokemon->get_moves().at(_index).pp;
            }

            // TODO: set_pp when underlying function implemented

        private:
            pkmn::pokemon::sptr _pokemon;
            int _index;
    };

    class pokemon_move_slots_wrapper
    {
        public:
            pokemon_move_slots_wrapper():
                _pokemon(nullptr)
            {}

            pokemon_move_slots_wrapper(
                const pkmn::pokemon::sptr& cpp_pokemon
            ): _pokemon(cpp_pokemon)
            {
                _init();
            }

            pokemon_move_slot_wrapper get_move_slot(
                int index
            )
            {
                if(!_pokemon)
                {
                    throw std::runtime_error("This class should only be used as a member of another class, rather than standalone.");
                }

                // TODO: Lua check
                pkmn::enforce_bounds("Move index", index, 0, 3);

                return _moves.at(index);
            }

            size_t size()
            {
                return _moves.size();
            }

        private:
            pkmn::pokemon::sptr _pokemon;

            std::vector<pokemon_move_slot_wrapper> _moves;

            void _init()
            {
                for(int i = 0; i < 4; ++i)
                {
                    _moves.emplace_back(
                        pokemon_move_slot_wrapper(
                            _pokemon,
                            i
                        )
                    );
                }
            }
    };

    /*
     * This class is a thin wrapper around pkmn::pokemon::sptr and
     * will be what some SWIG wrappers will use instead of the class
     * itself. It will allow syntax like the following to be used:
     *
     * bulbasaur.EVs["Attack"] = 100
     */
    class pokemon
    {
        public:
            pokemon():
                _pokemon(nullptr)
            {}

            pokemon(
                const pkmn::pokemon::sptr& cpp_pokemon
            ): _pokemon(cpp_pokemon)
            {
                _init();
            }

            pokemon(
                const std::string& species,
                const std::string& game,
                const std::string& form,
                int level
            ): _pokemon(pkmn::pokemon::make(species, game, form, level))
            {
                _init();
            }

            pokemon(
                const std::string& filepath
            ): _pokemon(pkmn::pokemon::from_file(filepath))
            {
                _init();
            }

            pokemon(
                const pokemon& other
            ): _pokemon(other._pokemon)
            {
                _init();
            }

            bool operator==(
                const pokemon& rhs
            ) const
            {
                return (_pokemon == rhs._pokemon);
            }

            inline pkmn::pokemon::sptr get_internal() const
            {
                return _pokemon;
            }

            inline std::string get_species()
            {
                return _pokemon->get_species();
            }

            inline std::string get_game()
            {
                return _pokemon->get_game();
            }

            inline std::string get_form()
            {
                return _pokemon->get_form();
            }

            inline void set_form(
                const std::string& form
            )
            {
                _pokemon->set_form(form);
            }

            inline const pkmn::database::pokemon_entry& get_database_entry()
            {
                return _pokemon->get_database_entry();
            }

            inline std::string get_nickname()
            {
                return _pokemon->get_nickname();
            }

            inline void set_nickname(
                const std::string& nickname
            )
            {
                _pokemon->set_nickname(nickname);
            }

            inline std::string get_gender()
            {
                return _pokemon->get_gender();
            }

            inline void set_gender(
                const std::string& gender
            )
            {
                _pokemon->set_gender(gender);
            }

            inline bool is_shiny()
            {
                return _pokemon->is_shiny();
            }

            inline void set_shininess(
                bool value
            )
            {
                _pokemon->set_shininess(value);
            }

            inline std::string get_held_item()
            {
                return _pokemon->get_held_item();
            }

            inline void set_held_item(
                const std::string& held_item
            )
            {
                _pokemon->set_held_item(held_item);
            }

            inline std::string get_original_trainer_name()
            {
                return _pokemon->get_original_trainer_name();
            }

            inline void set_original_trainer_name(
                const std::string& trainer_name
            )
            {
                _pokemon->set_original_trainer_name(trainer_name);
            }

            inline uint16_t get_original_trainer_public_id()
            {
                return _pokemon->get_original_trainer_public_id();
            }

            inline uint16_t get_original_trainer_secret_id()
            {
                return _pokemon->get_original_trainer_secret_id();
            }

            inline uint32_t get_original_trainer_id()
            {
                return _pokemon->get_original_trainer_id();
            }

            inline void set_original_trainer_public_id(
                uint16_t public_id
            )
            {
                _pokemon->set_original_trainer_public_id(public_id);
            }

            inline void set_original_trainer_secret_id(
                uint16_t secret_id
            )
            {
                _pokemon->set_original_trainer_secret_id(secret_id);
            }

            inline void set_original_trainer_id(
                uint32_t public_id
            )
            {
                _pokemon->set_original_trainer_id(public_id);
            }

            inline std::string get_original_trainer_gender()
            {
                return _pokemon->get_original_trainer_gender();
            }

            inline void set_original_trainer_gender(
                const std::string& trainer_gender
            )
            {
                _pokemon->set_original_trainer_gender(trainer_gender);
            }

            inline int get_current_trainer_friendship()
            {
                return _pokemon->get_current_trainer_friendship();
            }

            inline void set_current_trainer_friendship(
                int friendship
            )
            {
                _pokemon->set_current_trainer_friendship(friendship);
            }

            inline std::string get_ability()
            {
                return _pokemon->get_ability();
            }

            inline void set_ability(
                const std::string& ability
            )
            {
                _pokemon->set_ability(ability);
            }

            inline std::string get_ball()
            {
                return _pokemon->get_ball();
            }

            inline void set_ball(
                const std::string& ball
            )
            {
                _pokemon->set_ball(ball);
            }

            inline int get_level_met()
            {
                return _pokemon->get_level_met();
            }

            inline void set_level_met(
                int level_met
            )
            {
                _pokemon->set_level_met(level_met);
            }

            inline std::string get_location_met(
                bool as_egg
            )
            {
                return _pokemon->get_location_met(as_egg);
            }

            inline void set_location_met(
                const std::string& location,
                bool as_egg
            )
            {
                _pokemon->set_location_met(location, as_egg);
            }

            inline std::string get_original_game()
            {
                return _pokemon->get_original_game();
            }

            inline void set_original_game(
                const std::string& original_game
            )
            {
                _pokemon->set_original_game(original_game);
            }

            inline uint32_t get_personality()
            {
                return _pokemon->get_personality();
            }

            inline void set_personality(
                uint32_t personality
            )
            {
                _pokemon->set_personality(personality);
            }

            inline int get_experience()
            {
                return _pokemon->get_experience();
            }

            inline void set_experience(
                int experience
            )
            {
                _pokemon->set_experience(experience);
            }

            inline int get_level()
            {
                return _pokemon->get_level();
            }

            inline void set_level(
                int level
            )
            {
                _pokemon->set_level(level);
            }

            inline pokemon_EV_map& get_EVs()
            {
                return _EV_map;
            }

            inline pokemon_IV_map& get_IVs()
            {
                return _IV_map;
            }

            inline pokemon_marking_map& get_markings()
            {
                // To throw pkmn::unimplemented_error if appropriate.
                (void)_pokemon->get_markings();

                return _marking_map;
            }

            inline pokemon_ribbon_map& get_ribbons()
            {
                // To throw pkmn::unimplemented_error if appropriate.
                (void)_pokemon->get_ribbons();

                return _ribbon_map;
            }

            inline pokemon_contest_stat_map& get_contest_stats()
            {
                // To throw pkmn::unimplemented_error if appropriate.
                (void)_pokemon->get_contest_stats();

                return _contest_stat_map;
            }

            inline pokemon_move_slots_wrapper& get_moves()
            {
                return _move_slots_wrapper;
            }

            // Stats are read-only, so no need to wrap.
            inline const std::map<std::string, int>& get_stats()
            {
                return _pokemon->get_stats();
            }

            inline std::string get_icon_filepath()
            {
                return _pokemon->get_icon_filepath();
            }

            inline std::string get_sprite_filepath()
            {
                return _pokemon->get_sprite_filepath();
            }

            inline int get_numeric_attribute(
                const std::string& attribute_name
            )
            {
                return _pokemon->get_numeric_attribute(attribute_name);
            }

            inline void set_numeric_attribute(
                const std::string& attribute_name,
                int value
            )
            {
                _pokemon->set_numeric_attribute(attribute_name, value);
            }

            inline std::vector<std::string> get_numeric_attribute_names()
            {
                return _pokemon->get_numeric_attribute_names();
            }

            inline std::string get_string_attribute(
                const std::string& attribute_name
            )
            {
                return _pokemon->get_string_attribute(attribute_name);
            }

            inline void set_string_attribute(
                const std::string& attribute_name,
                const std::string& value
            )
            {
                _pokemon->set_string_attribute(attribute_name, value);
            }

            inline std::vector<std::string> get_string_attribute_names()
            {
                return _pokemon->get_string_attribute_names();
            }

        private:
            pkmn::pokemon::sptr _pokemon;
            pokemon_EV_map _EV_map;
            pokemon_IV_map _IV_map;
            pokemon_marking_map _marking_map;
            pokemon_ribbon_map _ribbon_map;
            pokemon_contest_stat_map _contest_stat_map;
            pokemon_move_slots_wrapper _move_slots_wrapper;

            void _init()
            {
                _EV_map = pokemon_EV_map(_pokemon);
                _IV_map = pokemon_IV_map(_pokemon);
                _marking_map = pokemon_marking_map(_pokemon);
                _ribbon_map = pokemon_ribbon_map(_pokemon);
                _contest_stat_map = pokemon_contest_stat_map(_pokemon);
                _move_slots_wrapper = pokemon_move_slots_wrapper(_pokemon);
            }
    };

}}

#endif /* CPP_WRAPPERS_POKEMON_HPP */

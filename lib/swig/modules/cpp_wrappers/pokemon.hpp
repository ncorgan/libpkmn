/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_WRAPPERS_POKEMON_HPP
#define CPP_WRAPPERS_POKEMON_HPP

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

            PKMN_INLINE int get_EV(
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

            PKMN_INLINE void set_EV(
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

            PKMN_INLINE bool has_key(
                const std::string& key
            )
            {
                return (_pokemon->get_EVs().count(key) > 0);
            }

            PKMN_INLINE pkmn::pokemon::sptr get_internal()
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

            PKMN_INLINE int get_IV(
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

            PKMN_INLINE void set_IV(
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

            PKMN_INLINE bool has_key(
                const std::string& key
            )
            {
                return (_pokemon->get_IVs().count(key) > 0);
            }

            PKMN_INLINE pkmn::pokemon::sptr get_internal()
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

            PKMN_INLINE bool get_marking(
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

            PKMN_INLINE void set_marking(
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

            PKMN_INLINE bool has_key(
                const std::string& key
            )
            {
                return (_pokemon->get_markings().count(key) > 0);
            }

            PKMN_INLINE pkmn::pokemon::sptr get_internal()
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

            PKMN_INLINE bool get_ribbon(
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

            PKMN_INLINE void set_ribbon(
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

            PKMN_INLINE bool has_key(
                const std::string& key
            )
            {
                return (_pokemon->get_ribbons().count(key) > 0);
            }

            PKMN_INLINE pkmn::pokemon::sptr get_internal()
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

            PKMN_INLINE int get_contest_stat(
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

            PKMN_INLINE void set_contest_stat(
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

            PKMN_INLINE bool has_key(
                const std::string& key
            )
            {
                return (_pokemon->get_contest_stats().count(key) > 0);
            }

            PKMN_INLINE pkmn::pokemon::sptr get_internal()
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

            pokemon_move_slot_wrapper& get_move_slot(
                int index
            )
            {
                if(!_pokemon)
                {
                    throw std::runtime_error("This class should only be used as a member of another class, rather than standalone.");
                }

                // TODO: Lua check
                if(index < 0 or index > 3)
                {
                    pkmn::throw_out_of_range("index", 0, 3);
                }

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

            PKMN_INLINE pkmn::pokemon::sptr get_internal() const
            {
                return _pokemon;
            }

            PKMN_INLINE std::string get_species()
            {
                return _pokemon->get_species();
            }

            PKMN_INLINE std::string get_game()
            {
                return _pokemon->get_game();
            }

            PKMN_INLINE std::string get_form()
            {
                return _pokemon->get_form();
            }

            PKMN_INLINE void set_form(
                const std::string& form
            )
            {
                _pokemon->set_form(form);
            }

            PKMN_INLINE const pkmn::database::pokemon_entry& get_database_entry()
            {
                return _pokemon->get_database_entry();
            }

            PKMN_INLINE std::string get_nickname()
            {
                return _pokemon->get_nickname();
            }

            PKMN_INLINE void set_nickname(
                const std::string& nickname
            )
            {
                _pokemon->set_nickname(nickname);
            }

            PKMN_INLINE std::string get_gender()
            {
                return _pokemon->get_gender();
            }

            PKMN_INLINE void set_gender(
                const std::string& gender
            )
            {
                _pokemon->set_gender(gender);
            }

            PKMN_INLINE bool is_shiny()
            {
                return _pokemon->is_shiny();
            }

            PKMN_INLINE void set_shininess(
                bool value
            )
            {
                _pokemon->set_shininess(value);
            }

            PKMN_INLINE std::string get_held_item()
            {
                return _pokemon->get_held_item();
            }

            PKMN_INLINE void set_held_item(
                const std::string& held_item
            )
            {
                _pokemon->set_held_item(held_item);
            }

            PKMN_INLINE std::string get_trainer_name()
            {
                return _pokemon->get_trainer_name();
            }

            PKMN_INLINE void set_trainer_name(
                const std::string& trainer_name
            )
            {
                _pokemon->set_trainer_name(trainer_name);
            }

            PKMN_INLINE uint16_t get_trainer_public_id()
            {
                return _pokemon->get_trainer_public_id();
            }

            PKMN_INLINE uint16_t get_trainer_secret_id()
            {
                return _pokemon->get_trainer_secret_id();
            }

            PKMN_INLINE uint32_t get_trainer_id()
            {
                return _pokemon->get_trainer_id();
            }

            PKMN_INLINE void set_trainer_public_id(
                uint16_t public_id
            )
            {
                _pokemon->set_trainer_public_id(public_id);
            }

            PKMN_INLINE void set_trainer_secret_id(
                uint16_t secret_id
            )
            {
                _pokemon->set_trainer_secret_id(secret_id);
            }

            PKMN_INLINE void set_trainer_id(
                uint32_t public_id
            )
            {
                _pokemon->set_trainer_id(public_id);
            }

            PKMN_INLINE std::string get_trainer_gender()
            {
                return _pokemon->get_trainer_gender();
            }

            PKMN_INLINE void set_trainer_gender(
                const std::string& trainer_gender
            )
            {
                _pokemon->set_trainer_gender(trainer_gender);
            }

            PKMN_INLINE int get_friendship()
            {
                return _pokemon->get_friendship();
            }

            PKMN_INLINE void set_friendship(
                int friendship
            )
            {
                _pokemon->set_friendship(friendship);
            }

            PKMN_INLINE std::string get_ability()
            {
                return _pokemon->get_ability();
            }

            PKMN_INLINE void set_ability(
                const std::string& ability
            )
            {
                _pokemon->set_ability(ability);
            }

            PKMN_INLINE std::string get_ball()
            {
                return _pokemon->get_ball();
            }

            PKMN_INLINE void set_ball(
                const std::string& ball
            )
            {
                _pokemon->set_ball(ball);
            }

            PKMN_INLINE int get_level_met()
            {
                return _pokemon->get_level_met();
            }

            PKMN_INLINE void set_level_met(
                int level_met
            )
            {
                _pokemon->set_level_met(level_met);
            }

            PKMN_INLINE std::string get_location_met(
                bool as_egg
            )
            {
                return _pokemon->get_location_met(as_egg);
            }

            PKMN_INLINE void set_location_met(
                const std::string& location,
                bool as_egg
            )
            {
                _pokemon->set_location_met(location, as_egg);
            }

            PKMN_INLINE std::string get_original_game()
            {
                return _pokemon->get_original_game();
            }

            PKMN_INLINE void set_original_game(
                const std::string& original_game
            )
            {
                _pokemon->set_original_game(original_game);
            }

            PKMN_INLINE uint32_t get_personality()
            {
                return _pokemon->get_personality();
            }

            PKMN_INLINE void set_personality(
                uint32_t personality
            )
            {
                _pokemon->set_personality(personality);
            }

            PKMN_INLINE int get_experience()
            {
                return _pokemon->get_experience();
            }

            PKMN_INLINE void set_experience(
                int experience
            )
            {
                _pokemon->set_experience(experience);
            }

            PKMN_INLINE int get_level()
            {
                return _pokemon->get_level();
            }

            PKMN_INLINE void set_level(
                int level
            )
            {
                _pokemon->set_level(level);
            }

            PKMN_INLINE pokemon_EV_map& get_EVs()
            {
                return _EV_map;
            }

            PKMN_INLINE pokemon_IV_map& get_IVs()
            {
                return _IV_map;
            }

            PKMN_INLINE pokemon_marking_map& get_markings()
            {
                // To throw pkmn::unimplemented_error if appropriate.
                (void)_pokemon->get_markings();

                return _marking_map;
            }

            PKMN_INLINE pokemon_ribbon_map& get_ribbons()
            {
                // To throw pkmn::unimplemented_error if appropriate.
                (void)_pokemon->get_ribbons();

                return _ribbon_map;
            }

            PKMN_INLINE pokemon_contest_stat_map& get_contest_stats()
            {
                // To throw pkmn::unimplemented_error if appropriate.
                (void)_pokemon->get_contest_stats();

                return _contest_stat_map;
            }

            PKMN_INLINE pokemon_move_slots_wrapper& get_moves()
            {
                return _move_slots_wrapper;
            }

            // Stats are read-only, so no need to wrap.
            PKMN_INLINE const std::map<std::string, int>& get_stats()
            {
                return _pokemon->get_stats();
            }

            PKMN_INLINE std::string get_icon_filepath()
            {
                return _pokemon->get_icon_filepath();
            }

            PKMN_INLINE std::string get_sprite_filepath()
            {
                return _pokemon->get_sprite_filepath();
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

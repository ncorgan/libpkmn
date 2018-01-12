/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_WRAPPERS_HELPERS_HPP
#define CPP_WRAPPERS_HELPERS_HPP

#include "exception_internal.hpp"
#include "misc_common.hpp"

#include <pkmn/config.hpp>
#include <pkmn/exception.hpp>
#include <pkmn/pokemon.hpp>

#include <vector>

namespace pkmn { namespace swig {

    typedef std::map<std::string, bool> string_bool_map;
    typedef std::map<std::string, int> string_int_map;

    class EV_map
    {
        public:
            EV_map():
                _pokemon(nullptr)
            {}

            EV_map(
                const pkmn::pokemon::sptr& pokemon
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

            inline size_t size()
            {
                return _pokemon->get_EVs().size();
            }

            inline std::vector<std::string> keys()
            {
                return pkmn::map_keys_to_vector<string_int_map, std::string>(
                           _pokemon->get_EVs()
                       );
            }

            inline bool has_key(
                const std::string& key
            )
            {
                return (_pokemon->get_EVs().count(key) > 0);
            }

            uintmax_t cptr()
            {
                return uintmax_t(_pokemon.get());
            }

            bool operator==(const EV_map& rhs) const
            {
                return (_pokemon == rhs._pokemon);
            }

        private:
            pkmn::pokemon::sptr _pokemon;
    };

    class IV_map
    {
        public:
            IV_map():
                _pokemon(nullptr)
            {}

            IV_map(
                const pkmn::pokemon::sptr& pokemon
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

            inline size_t size()
            {
                return _pokemon->get_IVs().size();
            }

            inline std::vector<std::string> keys()
            {
                return pkmn::map_keys_to_vector<string_int_map, std::string>(
                           _pokemon->get_IVs()
                       );
            }

            inline bool has_key(
                const std::string& key
            )
            {
                return (_pokemon->get_IVs().count(key) > 0);
            }

            uintmax_t cptr()
            {
                return uintmax_t(_pokemon.get());
            }

            bool operator==(const IV_map& rhs) const
            {
                return (_pokemon == rhs._pokemon);
            }

        private:
            pkmn::pokemon::sptr _pokemon;
    };

    class marking_map
    {
        public:
            marking_map():
                _pokemon(nullptr)
            {}

            marking_map(
                const pkmn::pokemon::sptr& pokemon
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

            inline size_t size()
            {
                return _pokemon->get_markings().size();
            }

            inline std::vector<std::string> keys()
            {
                return pkmn::map_keys_to_vector<string_bool_map, std::string>(
                           _pokemon->get_markings()
                       );
            }

            inline bool has_key(
                const std::string& key
            )
            {
                return (_pokemon->get_markings().count(key) > 0);
            }

            uintmax_t cptr()
            {
                return uintmax_t(_pokemon.get());
            }

            bool operator==(const marking_map& rhs) const
            {
                return (_pokemon == rhs._pokemon);
            }

        private:
            pkmn::pokemon::sptr _pokemon;
    };

    class ribbon_map
    {
        public:
            ribbon_map():
                _pokemon(nullptr)
            {}

            ribbon_map(
                const pkmn::pokemon::sptr& pokemon
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

            inline size_t size()
            {
                return _pokemon->get_ribbons().size();
            }

            inline std::vector<std::string> keys()
            {
                return pkmn::map_keys_to_vector<string_bool_map, std::string>(
                           _pokemon->get_ribbons()
                       );
            }

            uintmax_t cptr()
            {
                return uintmax_t(_pokemon.get());
            }

            bool operator==(const ribbon_map& rhs) const
            {
                return (_pokemon == rhs._pokemon);
            }

        private:
            pkmn::pokemon::sptr _pokemon;
    };

    class contest_stat_map
    {
        public:
            contest_stat_map():
                _pokemon(nullptr)
            {}

            contest_stat_map(
                const pkmn::pokemon::sptr& pokemon
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

            inline size_t size()
            {
                return _pokemon->get_contest_stats().size();
            }

            inline std::vector<std::string> keys()
            {
                return pkmn::map_keys_to_vector<string_int_map, std::string>(
                           _pokemon->get_contest_stats()
                       );
            }

            inline bool has_key(
                const std::string& key
            )
            {
                return (_pokemon->get_contest_stats().count(key) > 0);
            }

            uintmax_t cptr()
            {
                return uintmax_t(_pokemon.get());
            }

            bool operator==(const contest_stat_map& rhs) const
            {
                return (_pokemon == rhs._pokemon);
            }

        private:
            pkmn::pokemon::sptr _pokemon;
    };

    class move_slot
    {
        public:
            move_slot():
                _pokemon(nullptr),
                _index(0)
            {}

            move_slot(
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

            void set_pp(int pp)
            {
                if(!_pokemon)
                {
                    throw std::runtime_error("This class should only be used as a member of another class, rather than standalone.");
                }

                _pokemon->set_move_pp(_index, pp);
            }

            uintmax_t cptr()
            {
                return uintmax_t(_pokemon.get());
            }

            int index()
            {
                return _index;
            }

            bool operator==(const move_slot& rhs) const
            {
                return (_pokemon == rhs._pokemon) && (_index == rhs._index);
            }

        private:
            pkmn::pokemon::sptr _pokemon;
            int _index;
    };

    class move_slots
    {
        public:
            move_slots():
                _pokemon(nullptr)
            {}

            move_slots(
                const pkmn::pokemon::sptr& cpp_pokemon
            ): _pokemon(cpp_pokemon)
            {
                _init();
            }

            move_slot get_move_slot(
                int index
            )
            {
                if(!_pokemon)
                {
                    throw std::runtime_error("This class should only be used as a member of another class, rather than standalone.");
                }

#ifdef SWIGLUA
                pkmn::enforce_bounds(
                    "Move index",
                    index,
                    1,
                    4
                );
#else
                pkmn::enforce_bounds(
                    "Move index",
                    index,
                    0,
                    3
                );
#endif

                return _moves.at(index);
            }

            size_t size()
            {
                return _moves.size();
            }

            uintmax_t cptr()
            {
                return uintmax_t(_pokemon.get());
            }

            bool operator==(const move_slots& rhs) const
            {
                return (_pokemon == rhs._pokemon) && (_moves == rhs._moves);
            }

        private:
            pkmn::pokemon::sptr _pokemon;

            std::vector<move_slot> _moves;

            void _init()
            {
                for(int i = 0; i < 4; ++i)
                {
                    _moves.emplace_back(
                        move_slot(
                            _pokemon,
                            i
                        )
                    );
                }
            }
    };

}}

#endif /* CPP_WRAPPERS_HELPERS_HPP */

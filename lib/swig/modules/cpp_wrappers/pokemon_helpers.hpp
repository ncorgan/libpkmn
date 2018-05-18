/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_WRAPPERS_POKEMON_HELPERS_HPP
#define CPP_WRAPPERS_POKEMON_HELPERS_HPP

#include "exception_internal.hpp"
#include "private_exports.hpp"
#include "utils/misc.hpp"

#include <pkmn/config.hpp>
#include <pkmn/exception.hpp>
#include <pkmn/pokemon.hpp>

#include <boost/assert.hpp>

#include <vector>

namespace pkmn { namespace swig {

    typedef std::map<std::string, bool> string_bool_map;
    typedef std::map<std::string, int> string_int_map;

    class EV_map
    {
        public:
            explicit EV_map(
                const pkmn::pokemon::sptr& pokemon
            ): _pokemon(pokemon)
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);
            }

            inline int get_EV(
                const std::string& stat
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                const string_int_map& EVs = _pokemon->get_EVs();
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
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_EV(stat, value);
            }

            inline size_t size()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return _pokemon->get_EVs().size();
            }

            inline std::vector<std::string> keys()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return pkmn::map_keys_to_vector<string_int_map, std::string>(
                           _pokemon->get_EVs()
                       );
            }

            inline bool has_key(
                const std::string& key
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return (_pokemon->get_EVs().count(key) > 0);
            }

#ifdef SWIGCSHARP
            inline uintmax_t cptr()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return uintmax_t(_pokemon.get());
            }
#else
            inline bool operator==(const EV_map& rhs) const
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return (_pokemon == rhs._pokemon);
            }

            inline bool operator!=(const EV_map& rhs) const
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return !operator==(rhs);
            }
#endif

        private:
            const pkmn::pokemon::sptr& _pokemon;

    };

    class IV_map
    {
        public:
            explicit IV_map(
                const pkmn::pokemon::sptr& pokemon
            ): _pokemon(pokemon)
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);
            }

            inline int get_IV(
                const std::string& stat
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                const string_int_map& IVs = _pokemon->get_IVs();
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
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_IV(stat, value);
            }

            inline size_t size()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return _pokemon->get_IVs().size();
            }

            inline std::vector<std::string> keys()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return pkmn::map_keys_to_vector<string_int_map, std::string>(
                           _pokemon->get_IVs()
                       );
            }

            inline bool has_key(
                const std::string& key
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return (_pokemon->get_IVs().count(key) > 0);
            }

#ifdef SWIGCSHARP
            inline uintmax_t cptr()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return uintmax_t(_pokemon.get());
            }
#else
            inline bool operator==(const IV_map& rhs) const
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return (_pokemon == rhs._pokemon);
            }

            inline bool operator!=(const IV_map& rhs) const
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return !operator==(rhs);
            }
#endif

        private:
            const pkmn::pokemon::sptr& _pokemon;
    };

    class marking_map
    {
        public:
            explicit marking_map(
                const pkmn::pokemon::sptr& pokemon
            ): _pokemon(pokemon)
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);
            }

            inline bool get_marking(
                const std::string& marking
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                const string_bool_map& markings = _pokemon->get_markings();
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
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_marking(stat, value);
            }

            inline size_t size()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                size_t ret = 0;

                if(pkmn::priv::game_name_to_generation(_pokemon->get_game()) >= 3)
                {
                    ret = _pokemon->get_markings().size();
                }

                return ret;
            }

            inline std::vector<std::string> keys()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                std::vector<std::string> ret;

                if(pkmn::priv::game_name_to_generation(_pokemon->get_game()) >= 3)
                {
                    ret = pkmn::map_keys_to_vector<string_bool_map, std::string>(
                              _pokemon->get_markings()
                          );
                }

                return ret;
            }

            inline bool has_key(
                const std::string& key
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return (_pokemon->get_markings().count(key) > 0);
            }

#ifdef SWIGCSHARP
            inline uintmax_t cptr()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return uintmax_t(_pokemon.get());
            }
#else
            inline bool operator==(const marking_map& rhs) const
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return (_pokemon == rhs._pokemon);
            }

            inline bool operator!=(const marking_map& rhs) const
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return !operator==(rhs);
            }
#endif

        private:
            const pkmn::pokemon::sptr& _pokemon;
    };

    class ribbon_map
    {
        public:
            explicit ribbon_map(
                const pkmn::pokemon::sptr& pokemon
            ): _pokemon(pokemon)
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);
            }

            inline bool get_ribbon(
                const std::string& ribbon
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                const string_bool_map& ribbons = _pokemon->get_ribbons();
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
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_ribbon(stat, value);
            }

            inline bool has_key(
                const std::string& key
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return (_pokemon->get_ribbons().count(key) > 0);
            }

            inline size_t size()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                size_t ret = 0;

                if(pkmn::priv::game_name_to_generation(_pokemon->get_game()) >= 3)
                {
                    ret = _pokemon->get_ribbons().size();
                }

                return ret;
            }

            inline std::vector<std::string> keys()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                std::vector<std::string> ret;

                if(pkmn::priv::game_name_to_generation(_pokemon->get_game()) >= 3)
                {
                    ret = pkmn::map_keys_to_vector<string_bool_map, std::string>(
                              _pokemon->get_ribbons()
                          );
                }

                return ret;
            }

#ifdef SWIGCSHARP
            inline uintmax_t cptr()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return uintmax_t(_pokemon.get());
            }
#else
            inline bool operator==(const ribbon_map& rhs) const
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return (_pokemon == rhs._pokemon);
            }

            inline bool operator!=(const ribbon_map& rhs) const
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return !operator==(rhs);
            }
#endif

        private:
            const pkmn::pokemon::sptr& _pokemon;
    };

    class contest_stat_map
    {
        public:
            explicit contest_stat_map(
                const pkmn::pokemon::sptr& pokemon
            ): _pokemon(pokemon)
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);
            }

            inline int get_contest_stat(
                const std::string& stat
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                const string_int_map& contest_stats = _pokemon->get_contest_stats();
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
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_contest_stat(stat, value);
            }

            inline size_t size()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                size_t ret = 0;

                if(pkmn::priv::game_name_to_generation(_pokemon->get_game()) >= 3)
                {
                    ret = _pokemon->get_contest_stats().size();
                }

                return ret;
            }

            inline std::vector<std::string> keys()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                std::vector<std::string> ret;

                if(pkmn::priv::game_name_to_generation(_pokemon->get_game()) >= 3)
                {
                    ret = pkmn::map_keys_to_vector<string_int_map, std::string>(
                              _pokemon->get_contest_stats()
                          );
                }

                return ret;
            }

            inline bool has_key(
                const std::string& key
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return (_pokemon->get_contest_stats().count(key) > 0);
            }

#ifdef SWIGCSHARP
            inline uintmax_t cptr()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return uintmax_t(_pokemon.get());
            }
#else
            inline bool operator==(const contest_stat_map& rhs) const
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return (_pokemon == rhs._pokemon);
            }

            inline bool operator!=(const contest_stat_map& rhs) const
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return !operator==(rhs);
            }
#endif

        private:
            const pkmn::pokemon::sptr& _pokemon;
    };

    class move_slot
    {
        public:
            move_slot(
                const pkmn::pokemon::sptr& cpp_pokemon,
                int index
            ): _pokemon(cpp_pokemon),
               _index(index)
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);
            }

            std::string get_move()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return _pokemon->get_moves().at(_index).move;
            }

            void set_move(
                const std::string& move
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_move(move, _index);
            }

            int get_pp()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return _pokemon->get_moves().at(_index).pp;
            }

            void set_pp(int pp)
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_move_pp(_index, pp);
            }

            int index()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return _index;
            }

#ifdef SWIGCSHARP
            inline uintmax_t cptr()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return uintmax_t(_pokemon.get());
            }
#else
            inline bool operator==(const move_slot& rhs) const
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return (_pokemon == rhs._pokemon) and
                       (_index == rhs._index);
            }

            inline bool operator!=(const move_slot& rhs) const
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return !operator==(rhs);
            }
#endif

        private:
            const pkmn::pokemon::sptr& _pokemon;
            int _index;
    };

    class move_slots
    {
        public:
            explicit move_slots(
                const pkmn::pokemon::sptr& cpp_pokemon
            ): _pokemon(cpp_pokemon)
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);
            }

            move_slot get_move_slot(
                int index
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

#ifdef SWIGLUA
                pkmn::enforce_bounds(
                    "Move index",
                    index,
                    1,
                    4
                );

                return move_slot(_pokemon, index-1);
#else
                pkmn::enforce_bounds(
                    "Move index",
                    index,
                    0,
                    3
                );

                return move_slot(_pokemon, index);
#endif
            }

            size_t size()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return _pokemon->get_moves().size();
            }

#ifdef SWIGCSHARP
            inline uintmax_t cptr()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return uintmax_t(_pokemon.get());
            }
#else
            inline bool operator==(const move_slots& rhs) const
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return (_pokemon == rhs._pokemon);
            }

            inline bool operator!=(const move_slots& rhs) const
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return !operator==(rhs);
            }
#endif

        private:
            const pkmn::pokemon::sptr& _pokemon;
    };

}}

#endif /* CPP_WRAPPERS_POKEMON_HELPERS_HPP */

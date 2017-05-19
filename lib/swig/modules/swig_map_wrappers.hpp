/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef SWIG_MAP_WRAPPERS_HPP
#define SWIG_MAP_WRAPPERS_HPP

#include <pkmn/config.hpp>
#include <pkmn/pokemon.hpp>

namespace pkmn { namespace swig {

    class pokemon_EV_map
    {
        public:
            pokemon_EV_map(
                pkmn::pokemon::sptr pokemon
            ): _pokemon(pokemon)
            {}

            PKMN_INLINE int get_EV(
                const std::string& stat
            )
            {
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
                _pokemon->set_EV(stat, value);
            }

        private:
            pkmn::pokemon::sptr _pokemon;
    };

    class pokemon_IV_map
    {
        public:
            pokemon_IV_map(
                pkmn::pokemon::sptr pokemon
            ): _pokemon(pokemon)
            {}

            PKMN_INLINE int get_IV(
                const std::string& stat
            )
            {
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
                _pokemon->set_IV(stat, value);
            }

        private:
            pkmn::pokemon::sptr _pokemon;
    };

    class pokemon_stat_map
    {
        public:
            pokemon_stat_map(
                pkmn::pokemon::sptr pokemon
            ): _pokemon(pokemon)
            {}

            PKMN_INLINE int get_stat(
                const std::string& stat
            )
            {
                const std::map<std::string, int>& stats = _pokemon->get_stats();
                if(stats.count(stat) == 0)
                {
                    throw std::invalid_argument("Invalid stat.");
                }

                return stats.at(stat);
            }

        private:
            pkmn::pokemon::sptr _pokemon;
    };

}}

#endif /* SWIG_MAP_WRAPPERS_HPP */

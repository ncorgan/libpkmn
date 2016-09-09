/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_IMPL_HPP
#define PKMN_POKEMON_IMPL_HPP

#include <pkmn/pokemon.hpp>

#include <boost/noncopyable.hpp>

namespace pkmn {

    class pokemon_impl: public pokemon, public boost::noncopyable {
        public:
            pokemon_impl() {}
            pokemon_impl(
                int pokemon_index,
                int game_id
            );

            std::string get_game();

            const pkmn::database::pokemon_entry& get_database_entry();

            const std::map<std::string, int>& get_stats();

            const std::map<std::string, int>& get_EVs();

            const std::map<std::string, int>& get_IVs();

            void* get_native_pc_data();

            void* get_native_party_data();

        protected:
            std::map<std::string, int> _stats, _EVs, _IVs;

            pkmn::database::pokemon_entry _database_entry;

            void* _native_pc;
            void* _native_party;

            virtual void _update_EVs() = 0;
            virtual void _update_IVs() = 0;
            virtual void _update_stats() = 0;
    };

}

#endif /* PKMN_POKEMON_IMPL_HPP */

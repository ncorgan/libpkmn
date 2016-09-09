/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_HPP
#define PKMN_POKEMON_HPP

#include <pkmn/config.hpp>
#include <pkmn/database/item_entry.hpp>
#include <pkmn/database/move_entry.hpp>
#include <pkmn/database/pokemon_entry.hpp>
#include <pkmn/types/shared_ptr.hpp>

#include <map>
#include <string>

namespace pkmn {

    class PKMN_API pokemon {
        public:
            typedef pkmn::shared_ptr<pokemon> sptr;

            #ifndef __DOXYGEN__
            pokemon() {}
            virtual ~pokemon () {}
            #endif

            virtual std::string get_game() = 0;

            virtual const pkmn::database::pokemon_entry& get_database_entry() = 0;

            virtual std::string get_nickname() = 0;

            virtual int get_experience() = 0;

            virtual int get_level() = 0;

            virtual const std::map<std::string, int>& get_stats() = 0;

            virtual const std::map<std::string, int>& get_EVs() = 0;

            virtual const std::map<std::string, int>& get_IVs() = 0;

            #ifndef __DOXYGEN__
            virtual void* get_native_pc_data() = 0;
            virtual void* get_native_party_data() = 0;
            #endif
    };

}

#endif /* PKMN_POKEMON_HPP */

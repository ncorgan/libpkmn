/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_impl.hpp"

namespace pkmn {

    pokemon_impl::pokemon_impl(
        int pokemon_index,
        int game_id
    ): pokemon(),
       _database_entry(pkmn::database::pokemon_entry(pokemon_index, game_id))
    {}

    std::string pokemon_impl::get_game() {
        return _database_entry.get_game();
    }

    const pkmn::database::pokemon_entry& pokemon_impl::get_database_entry() {
        return _database_entry;
    }

    const pkmn::move_slots_t& pokemon_impl::get_moves() {
        _update_moves(-1);
        return _moves;
    }

    const std::map<std::string, int>& pokemon_impl::get_EVs() {
        _update_EV_map();
        return _EVs;
    }

    const std::map<std::string, int>& pokemon_impl::get_IVs() {
        _update_IV_map();
        return _IVs;
    }

    const std::map<std::string, int>& pokemon_impl::get_stats() {
        _update_stat_map();
        return _stats;
    }

    void* pokemon_impl::get_native_pc_data() {
        return _native_pc;
    }

    void* pokemon_impl::get_native_party_data() {
        return _native_party;
    }

}

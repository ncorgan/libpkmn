/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_box_gbaimpl.hpp"
#include "pokemon_gbaimpl.hpp"

#include <pkmn/exception.hpp>

#include <pksav/math/endian.h>

#include <cstring>
#include <stdexcept>

#define NATIVE_RCAST (reinterpret_cast<pksav_gba_pokemon_box_t*>(_native))

namespace pkmn {

    pokemon_box_gbaimpl::pokemon_box_gbaimpl(
        int game_id
    ): pokemon_box_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new pksav_gba_pokemon_box_t);
        std::memset(_native, 0, sizeof(pksav_gba_pokemon_box_t));
        _our_mem = true;

        _from_native();
    }

    pokemon_box_gbaimpl::pokemon_box_gbaimpl(
        int game_id,
        pksav_gba_pokemon_box_t* native
    ): pokemon_box_impl(game_id)
    {
        _native = reinterpret_cast<void*>(native);
        _our_mem = false;

        _from_native();
    }

    pokemon_box_gbaimpl::pokemon_box_gbaimpl(
        int game_id,
        const pksav_gba_pokemon_box_t &native
    ): pokemon_box_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new pksav_gba_pokemon_box_t);
        *NATIVE_RCAST = native;
        _our_mem = true;

        _from_native();
    }

    pokemon_box_gbaimpl::~pokemon_box_gbaimpl() {
        if(_our_mem) {
            delete NATIVE_RCAST;
        }
    }

    std::string pokemon_box_gbaimpl::get_name() {
        return _box_name;
    }

    void pokemon_box_gbaimpl::set_name(
        const std::string &name
    ) {
        if(name.size() > 8) {
            throw std::invalid_argument("Generation III box names have a maximum length of 8.");
        }

        _box_name = name;
    }

    int pokemon_box_gbaimpl::get_num_pokemon() {
        int num_pokemon = 0;
        for(int i = 0; i < get_capacity(); ++i) {
            if(pksav_littleendian16(NATIVE_RCAST->entries[i].blocks.growth.species) > 0) {
                ++num_pokemon;
            }
        }

        return num_pokemon;
    }

    int pokemon_box_gbaimpl::get_capacity() {
        return int(sizeof(NATIVE_RCAST->entries)/sizeof(NATIVE_RCAST->entries[0]));
    }

    void pokemon_box_gbaimpl::set_pokemon(
        int index,
        pkmn::pokemon::sptr new_pokemon
    )
    {
        int max_index = get_capacity();

        if(index < 0 or index > max_index)
        {
            pkmn::throw_out_of_range("index", 0, max_index);
        }
        else if(_pokemon_list.at(index)->get_native_pc_data() == new_pokemon->get_native_pc_data())
        {
            throw std::invalid_argument("Cannot set a Pokémon to itself.");
        }
        else if(_game_id != new_pokemon->get_database_entry().get_game_id())
        {
            throw std::invalid_argument("The given Pokémon must be from the same game as the party.");
        }

        boost::unique_lock<boost::recursive_mutex> scoped_lock(_mem_mutex);

        pokemon_impl* new_pokemon_impl_ptr = dynamic_cast<pokemon_impl*>(new_pokemon.get());
        pokemon_impl* old_box_pokemon_impl_ptr = dynamic_cast<pokemon_impl*>(_pokemon_list[index].get());

        // Make sure no one else is using the Pokémon variables.
        boost::unique_lock<boost::recursive_mutex> new_pokemon_lock(new_pokemon_impl_ptr->_mem_mutex);
        old_box_pokemon_impl_ptr->_mem_mutex.lock();

        // Copy the underlying memory to the party. At the end of this process,
        // all existing variables will correspond to the same Pokémon, even if
        // their underlying memory has changed.

        // Make a copy of the current Pokémon in the given party slot so it can be preserved in an sptr
        // that owns its own memory.
        copy_box_pokemon<pksav_gba_pc_pokemon_t, pksav_gba_pokemon_party_data_t>(index);

        // Copy the new Pokémon's internals into the party's internals and create a new sptr.
        void* new_pokemon_native_pc_ptr = new_pokemon_impl_ptr->_native_pc;

        // Unlock the old Pokémon's mutex is unlocked before it's destructor is called.
        old_box_pokemon_impl_ptr->_mem_mutex.unlock();

        NATIVE_RCAST->entries[index] = *reinterpret_cast<pksav_gba_pc_pokemon_t*>(new_pokemon_native_pc_ptr);

        _pokemon_list[index] = pkmn::make_shared<pokemon_gbaimpl>(
                                   &NATIVE_RCAST->entries[index],
                                   _game_id
                               );
    }

    void pokemon_box_gbaimpl::_from_native() {
        int capacity = get_capacity();

        // This shouldn't resize if the vector is populated.
        _pokemon_list.resize(capacity);

        for(int i = 0; i < capacity; ++i) {
            _pokemon_list[i] = pkmn::make_shared<pokemon_gbaimpl>(
                                   &NATIVE_RCAST->entries[i],
                                   _game_id
                               );
        }
    }
}

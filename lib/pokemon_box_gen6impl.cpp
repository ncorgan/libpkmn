/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "pokemon_box_gen6impl.hpp"
#include "pokemon_gen6impl.hpp"

#include <pkmn/exception.hpp>

#include <pksav/math/endian.h>

#include <boost/thread/lock_guard.hpp>

#include <cstring>
#include <stdexcept>

#define NATIVE_RCAST (reinterpret_cast<pksav_gen6_pokemon_box_t*>(_native))

namespace pkmn {

    pokemon_box_gen6impl::pokemon_box_gen6impl(
        int game_id
    ): pokemon_box_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new pksav_gen6_pokemon_box_t);
        std::memset(_native, 0, sizeof(pksav_gen6_pokemon_box_t));
        _our_mem = true;

        _from_native();
    }

    pokemon_box_gen6impl::pokemon_box_gen6impl(
        int game_id,
        pksav_gen6_pokemon_box_t* native
    ): pokemon_box_impl(game_id)
    {
        _native = reinterpret_cast<void*>(native);
        _our_mem = false;

        _from_native();
    }

    pokemon_box_gen6impl::pokemon_box_gen6impl(
        int game_id,
        const pksav_gen6_pokemon_box_t &native
    ): pokemon_box_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new pksav_gen6_pokemon_box_t);
        *NATIVE_RCAST = native;
        _our_mem = true;

        _from_native();
    }

    pokemon_box_gen6impl::~pokemon_box_gen6impl() {
        if(_our_mem) {
            delete NATIVE_RCAST;
        }
    }

    std::string pokemon_box_gen6impl::get_name() {
        return _box_name;
    }

    void pokemon_box_gen6impl::set_name(
        const std::string &name
    )
    {
        pkmn::enforce_string_length(
            "Box name",
            name,
            0,
            8
        );

        _box_name = name;
    }

    int pokemon_box_gen6impl::get_num_pokemon() {
        int num_pokemon = 0;
        for(int i = 0; i < get_capacity(); ++i) {
            if(pksav_littleendian16(NATIVE_RCAST->entries[i].blocks.blockA.species) > 0) {
                ++num_pokemon;
            }
        }

        return num_pokemon;
    }

    int pokemon_box_gen6impl::get_capacity() {
        return int(sizeof(NATIVE_RCAST->entries)/sizeof(NATIVE_RCAST->entries[0]));
    }

    void pokemon_box_gen6impl::set_pokemon(
        int index,
        pkmn::pokemon::sptr new_pokemon
    )
    {
        int max_index = get_capacity();
        pkmn::enforce_bounds("Box index", index, 0, max_index);

        if(_pokemon_list.at(index)->get_native_pc_data() == new_pokemon->get_native_pc_data())
        {
            throw std::invalid_argument("Cannot set a Pokémon to itself.");
        }

        // If the given Pokémon isn't from this box's game, convert it if we can.
        pkmn::pokemon::sptr actual_new_pokemon;
        if(_game_id == new_pokemon->get_database_entry().get_game_id())
        {
            actual_new_pokemon = new_pokemon;
        }
        else
        {
            actual_new_pokemon = new_pokemon->to_game(get_game());
        }

        pokemon_impl* new_pokemon_impl_ptr = dynamic_cast<pokemon_impl*>(actual_new_pokemon.get());
        pokemon_impl* old_box_pokemon_impl_ptr = dynamic_cast<pokemon_impl*>(_pokemon_list[index].get());

        // Make sure no one else is using the Pokémon variables.
        boost::lock_guard<pokemon_impl> new_pokemon_lock(*new_pokemon_impl_ptr);
        old_box_pokemon_impl_ptr->lock();

        // Copy the underlying memory to the party. At the end of this process,
        // all existing variables will correspond to the same Pokémon, even if
        // their underlying memory has changed.

        // Make a copy of the current Pokémon in the given party slot so it can be preserved in an sptr
        // that owns its own memory.
        copy_box_pokemon<pksav_gen6_pc_pokemon_t, pksav_gen6_pokemon_party_data_t>(index);

        // Copy the new Pokémon's internals into the party's internals and create a new sptr.
        void* new_pokemon_native_pc_ptr = new_pokemon_impl_ptr->_native_pc;

        // Unlock the old Pokémon's mutex is unlocked before it's destructor is called.
        old_box_pokemon_impl_ptr->unlock();

        NATIVE_RCAST->entries[index] = *reinterpret_cast<pksav_gen6_pc_pokemon_t*>(new_pokemon_native_pc_ptr);

        _pokemon_list[index] = pkmn::make_shared<pokemon_gen6impl>(
                                   &NATIVE_RCAST->entries[index],
                                   _game_id
                               );
    }

    void pokemon_box_gen6impl::_from_native() {
        int capacity = get_capacity();

        // This shouldn't resize if the vector is populated.
        _pokemon_list.resize(capacity);

        for(int i = 0; i < capacity; ++i) {
            _pokemon_list[i] = pkmn::make_shared<pokemon_gen6impl>(
                                   &NATIVE_RCAST->entries[i],
                                   _game_id
                               );
        }
    }
}
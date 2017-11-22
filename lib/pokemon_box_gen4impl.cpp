/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_box_gen4impl.hpp"
#include "pokemon_ndsimpl.hpp"

#include <pkmn/exception.hpp>

#include <pksav/gen4/pokemon.h>
#include <pksav/math/endian.h>

#include <boost/thread/lock_guard.hpp>

#include <cstring>
#include <stdexcept>

#define DPPT_RCAST reinterpret_cast<pksav_dppt_pokemon_box_t*>(_native)
#define HGSS_RCAST reinterpret_cast<pksav_hgss_pokemon_box_t*>(_native)

#define DPPT_SIZE (sizeof(DPPT_RCAST->entries)/sizeof(pksav_nds_pc_pokemon_t))
#define HGSS_SIZE (sizeof(HGSS_RCAST->entries)/sizeof(pksav_nds_pc_pokemon_t))

#define NATIVE_ENTRY_ADDR(index) (_hgss ? &HGSS_RCAST->entries[index] : &DPPT_RCAST->entries[index])

namespace pkmn {

    BOOST_STATIC_CONSTEXPR int HEARTGOLD  = 15;
    BOOST_STATIC_CONSTEXPR int SOULSILVER = 16;

    pokemon_box_gen4impl::pokemon_box_gen4impl(
        int game_id
    ): pokemon_box_impl(game_id),
       _hgss(game_id == HEARTGOLD or game_id == SOULSILVER)
    {
        if(_hgss) {
            _native = reinterpret_cast<void*>(new pksav_hgss_pokemon_box_t);
            std::memset(_native, 0, sizeof(pksav_hgss_pokemon_box_t));
        } else {
            _native = reinterpret_cast<void*>(new pksav_dppt_pokemon_box_t);
            std::memset(_native, 0, sizeof(pksav_dppt_pokemon_box_t));
        }

        _our_mem = true;

        _from_native();
    }

    pokemon_box_gen4impl::pokemon_box_gen4impl(
        int game_id,
        void* native
    ): pokemon_box_impl(game_id),
       _hgss(game_id == HEARTGOLD or game_id == SOULSILVER)
    {
        _native = native;
        _our_mem = false;

        _from_native();
    }

    pokemon_box_gen4impl::~pokemon_box_gen4impl()
    {
        if(_our_mem)
        {
            if(_hgss)
            {
                delete HGSS_RCAST;
            }
            else
            {
                delete DPPT_RCAST;
            }
        }
    }

    std::string pokemon_box_gen4impl::get_name()
    {
        return _box_name;
    }

    void pokemon_box_gen4impl::set_name(
        const std::string &name
    )
    {
        pkmn::enforce_string_length(
            "Box name",
            name,
            0,
            10
        );

        _box_name = name;
    }

    int pokemon_box_gen4impl::get_num_pokemon()
    {
        int num_pokemon = 0;
        for(int i = 0; i < get_capacity(); ++i)
        {
            if(pksav_littleendian16(NATIVE_ENTRY_ADDR(i)->blocks.blockA.species) > 0)
            {
                ++num_pokemon;
            }
        }

        return num_pokemon;
    }

    int pokemon_box_gen4impl::get_capacity()
    {
        return int(_hgss ? HGSS_SIZE : DPPT_SIZE);
    }

    void pokemon_box_gen4impl::set_pokemon(
        int index,
        pkmn::pokemon::sptr new_pokemon
    )
    {
        int max_index = get_capacity();
        pkmn::enforce_bounds("Box index", index, 0, max_index);

        pkmn::enforce_bounds(
            "Box index",
            index,
            0,
            max_index
        );

        if(_pokemon_list.at(index)->get_native_pc_data() == new_pokemon->get_native_pc_data())
        {
            throw std::invalid_argument("Cannot set a Pokémon to itself.");
        }

        boost::mutex::scoped_lock(_mem_mutex);

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
        copy_box_pokemon<pksav_nds_pc_pokemon_t, pksav_nds_pokemon_party_data_t>(index);

        // Copy the new Pokémon's internals into the party's internals and create a new sptr.
        void* new_pokemon_native_pc_ptr = new_pokemon_impl_ptr->_native_pc;

        // Unlock the old Pokémon's mutex is unlocked before its destructor is called.
        old_box_pokemon_impl_ptr->unlock();

        *NATIVE_ENTRY_ADDR(index) = *reinterpret_cast<pksav_nds_pc_pokemon_t*>(new_pokemon_native_pc_ptr);

        _pokemon_list[index] = pkmn::make_shared<pokemon_ndsimpl>(
                                   NATIVE_ENTRY_ADDR(index),
                                   _game_id
                               );
    }

    void pokemon_box_gen4impl::_from_native()
    {
        int capacity = get_capacity();

        // This shouldn't resize if the vector is populated.
        _pokemon_list.resize(capacity);

        for(int i = 0; i < capacity; ++i)
        {
            _pokemon_list[i] = pkmn::make_shared<pokemon_ndsimpl>(
                                   NATIVE_ENTRY_ADDR(i),
                                   _game_id
                               );
        }
    }
}

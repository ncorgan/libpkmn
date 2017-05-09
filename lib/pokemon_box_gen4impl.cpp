/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_box_gen4impl.hpp"
#include "pokemon_ndsimpl.hpp"

#include <pksav/gen4/pokemon.h>
#include <pksav/math/endian.h>

#include <cstring>
#include <stdexcept>

#define DPPT_RCAST reinterpret_cast<pksav_dppt_pokemon_box_t*>(_native)
#define HGSS_RCAST reinterpret_cast<pksav_hgss_pokemon_box_t*>(_native)

#define DPPT_SIZE (sizeof(DPPT_RCAST->entries)/sizeof(pksav_nds_pc_pokemon_t))
#define HGSS_SIZE (sizeof(HGSS_RCAST->entries)/sizeof(pksav_nds_pc_pokemon_t))

#define NATIVE_ENTRY(index) (_hgss ? &HGSS_RCAST->entries[index] : &DPPT_RCAST->entries[index])

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

    pokemon_box_gen4impl::~pokemon_box_gen4impl() {
        if(_our_mem) {
            if(_hgss) {
                delete HGSS_RCAST;
            } else {
                delete DPPT_RCAST;
            }
        }
    }

    std::string pokemon_box_gen4impl::get_name() {
        return _box_name;
    }

    void pokemon_box_gen4impl::set_name(
        const std::string &name
    ) {
        if(name.size() > 10) {
            throw std::invalid_argument("Generation IV box names have a maximum length of 10.");
        }

        _box_name = name;
    }

    int pokemon_box_gen4impl::get_num_pokemon() {
        int num_pokemon = 0;
        for(int i = 0; i < get_capacity(); ++i) {
            if(pksav_littleendian16(NATIVE_ENTRY(i)->blocks.blockA.species) > 0) {
                ++num_pokemon;
            }
        }

        return num_pokemon;
    }

    int pokemon_box_gen4impl::get_capacity() {
        return int(_hgss ? HGSS_SIZE : DPPT_SIZE);
    }

    void pokemon_box_gen4impl::_from_native() {
        int capacity = get_capacity();

        // This shouldn't resize if the vector is populated.
        _pokemon_list.resize(capacity);

        for(int i = 0; i < capacity; ++i) {
            _pokemon_list[i] = pkmn::make_shared<pokemon_ndsimpl>(
                                   NATIVE_ENTRY(i),
                                   _game_id
                               );
        }
    }
}

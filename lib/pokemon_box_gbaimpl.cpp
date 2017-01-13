/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_box_gbaimpl.hpp"
#include "pokemon_gbaimpl.hpp"

#include <pksav/gba/text.h>

#include <cstring>
#include <stdexcept>

#define NATIVE_RCAST reinterpret_cast<pksav_gba_pokemon_box_t*>(_native)

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
            throw std::invalid_argument("Generation II box names have a maximum length of 8.");
        }

        _box_name = name;
    }

    int pokemon_box_gbaimpl::get_capacity() {
        return int(sizeof(NATIVE_RCAST->entries)/sizeof(NATIVE_RCAST->entries[0]));
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

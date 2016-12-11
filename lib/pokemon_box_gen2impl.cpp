/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_box_gen2impl.hpp"
#include "pokemon_gen2impl.hpp"

#include "pksav/pksav_common.hpp"

#include <pksav/gen2/text.h>

#include <cstring>
#include <stdexcept>

#define NATIVE_RCAST reinterpret_cast<pksav_gen2_pokemon_box_t*>(_native)

namespace pkmn {

    pokemon_box_gen2impl::pokemon_box_gen2impl(
        int game_id
    ): pokemon_box_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new pksav_gen2_pokemon_box_t);
        std::memset(_native, 0, sizeof(pksav_gen2_pokemon_box_t));
        NATIVE_RCAST->species[20] = 0xFF;
        _our_mem = true;

        _from_native();
    }

    pokemon_box_gen2impl::pokemon_box_gen2impl(
        int game_id,
        pksav_gen2_pokemon_box_t* native
    ): pokemon_box_impl(game_id)
    {
        _native = reinterpret_cast<void*>(native);
        _our_mem = false;

        _from_native();
    }

    pokemon_box_gen2impl::pokemon_box_gen2impl(
        int game_id,
        const pksav_gen2_pokemon_box_t &native
    ): pokemon_box_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new pksav_gen2_pokemon_box_t);
        *NATIVE_RCAST = native;
        _our_mem = true;
    }

    pokemon_box_gen2impl::~pokemon_box_gen2impl() {
        if(_our_mem) {
            delete NATIVE_RCAST;
        }
    }

    std::string pokemon_box_gen2impl::get_name() {
        return _box_name;
    }

    void pokemon_box_gen2impl::set_name(
        const std::string &name
    ) {
        if(name.size() > 8) {
            throw std::invalid_argument("Generation II box names have a maximum length of 8.");
        }

        _box_name = name;
    }

    int pokemon_box_gen2impl::get_capacity() {
        return int(sizeof(NATIVE_RCAST->entries)/sizeof(NATIVE_RCAST->entries[0]));
    }

    void pokemon_box_gen2impl::_from_native() {
        int capacity = get_capacity();

        // This shouldn't resize if the vector is populated.
        _pokemon_list.resize(capacity);

        char nickname[11] = {0};
        char otname[8] = {0};

        // TODO: use species to set is_egg
        for(int i = 0; i < capacity; ++i) {
            _pokemon_list[i] = pkmn::make_shared<pokemon_gen2impl>(
                                   &NATIVE_RCAST->entries[i],
                                   _game_id
                               );

            PKSAV_CALL(
                pksav_text_from_gen2(
                    NATIVE_RCAST->nicknames[i],
                    nickname,
                    10
                );
            )
            if(std::strlen(nickname) > 0) {
                _pokemon_list[i]->set_nickname(nickname);
            }

            PKSAV_CALL(
                pksav_text_from_gen2(
                    NATIVE_RCAST->otnames[i],
                    otname,
                    7
                );
            )
            if(std::strlen(otname) > 0) {
                _pokemon_list[i]->set_trainer_name(otname);
            }
        }
    }
}

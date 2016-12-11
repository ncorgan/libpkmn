/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_box_gen1impl.hpp"
#include "pokemon_gen1impl.hpp"

#include "pksav/pksav_common.hpp"

#include <pksav/gen1/text.h>

#include <cstring>
#include <stdexcept>

#define NATIVE_RCAST reinterpret_cast<pksav_gen1_pokemon_box_t*>(_native)

namespace pkmn {

    pokemon_box_gen1impl::pokemon_box_gen1impl(
        int game_id
    ): pokemon_box_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new pksav_gen1_pokemon_box_t);
        std::memset(_native, 0, sizeof(pksav_gen1_pokemon_box_t));
        NATIVE_RCAST->species[20] = 0xFF;
        _our_mem = true;

        _from_native();
    }

    pokemon_box_gen1impl::pokemon_box_gen1impl(
        int game_id,
        pksav_gen1_pokemon_box_t* native
    ): pokemon_box_impl(game_id)
    {
        _native = reinterpret_cast<void*>(native);
        _our_mem = false;

        _from_native();
    }

    pokemon_box_gen1impl::pokemon_box_gen1impl(
        int game_id,
        const pksav_gen1_pokemon_box_t &native
    ): pokemon_box_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new pksav_gen1_pokemon_box_t);
        *NATIVE_RCAST = native;
        _our_mem = true;
    }

    pokemon_box_gen1impl::~pokemon_box_gen1impl() {
        if(_our_mem) {
            delete NATIVE_RCAST;
        }
    }

    std::string pokemon_box_gen1impl::get_name() {
        throw pkmn::feature_not_in_game_error("Box names", "Generation I");
    }

    void pokemon_box_gen1impl::set_name(
        PKMN_UNUSED(const std::string &name)
    ) {
        throw pkmn::feature_not_in_game_error("Box names", "Generation I");
    }

    int pokemon_box_gen1impl::get_capacity() {
        return int(sizeof(NATIVE_RCAST->entries)/sizeof(NATIVE_RCAST->entries[0]));
    }

    void pokemon_box_gen1impl::_from_native() {
        int capacity = get_capacity();

        // This shouldn't resize if the vector is populated.
        _pokemon_list.resize(capacity);

        char nickname[11] = {0};
        char otname[8] = {0};

        for(int i = 0; i < capacity; ++i) {
            _pokemon_list[i] = pkmn::make_shared<pokemon_gen1impl>(
                                   &NATIVE_RCAST->entries[i],
                                   _game_id
                               );

            PKSAV_CALL(
                pksav_text_from_gen1(
                    NATIVE_RCAST->nicknames[i],
                    nickname,
                    10
                );
            )
            if(std::strlen(nickname) > 0) {
                _pokemon_list[i]->set_nickname(nickname);
            }

            PKSAV_CALL(
                pksav_text_from_gen1(
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

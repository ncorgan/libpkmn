/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_pc_gbaimpl.hpp"
#include "pokemon_box_gbaimpl.hpp"

#include <pksav/gba/pokemon.h>
#include <pksav/gba/text.h>

#include <cstring>

#define NATIVE_RCAST reinterpret_cast<pksav_gba_pokemon_pc_t*>(_native)

namespace pkmn {

    pokemon_pc_gbaimpl::pokemon_pc_gbaimpl(
        int game_id
    ): pokemon_pc_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new pksav_gba_pokemon_pc_t);
        std::memset(_native, 0, sizeof(pksav_gba_pokemon_pc_t));
        _our_mem = true;

        _from_native();
    }

    pokemon_pc_gbaimpl::pokemon_pc_gbaimpl(
        int game_id,
        pksav_gba_pokemon_pc_t* native
    ): pokemon_pc_impl(game_id)
    {
        _native = reinterpret_cast<void*>(native);
        _our_mem = false;

        _from_native();
    }

    pokemon_pc_gbaimpl::pokemon_pc_gbaimpl(
        int game_id,
        const pksav_gba_pokemon_pc_t &native
    ): pokemon_pc_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new pksav_gba_pokemon_pc_t);
        *NATIVE_RCAST = native;
        _our_mem = true;

        _from_native();
    }

    pokemon_pc_gbaimpl::~pokemon_pc_gbaimpl() {
        if(_our_mem) {
            delete NATIVE_RCAST;
        }
    }

    int pokemon_pc_gbaimpl::get_num_boxes() {
        return GBA_NUM_BOXES;
    }

    void pokemon_pc_gbaimpl::_from_native() {
        _box_list.resize(GBA_NUM_BOXES);

        for(int i = 0; i < GBA_NUM_BOXES; ++i) {
            _box_list[i] = pkmn::make_shared<pokemon_box_gbaimpl>(
                               _game_id,
                               &NATIVE_RCAST->boxes[i]
                           );

            char box_name[9] = {0};
            pksav_text_from_gba(
                NATIVE_RCAST->box_names[i],
                box_name,
                8
            );
            _box_list[i]->set_name(box_name);
        }
    }

    void pokemon_pc_gbaimpl::_update_box_names() {
        for(int i = 0; i < GBA_NUM_BOXES; ++i) {
            _box_names[i] = _box_list[i]->get_name();

            pksav_text_to_gba(
                _box_names[i].c_str(),
                NATIVE_RCAST->box_names[i],
                8
            );
        }
    }
}

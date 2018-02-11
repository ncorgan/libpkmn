/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_pc_gen1impl.hpp"

#include "pokemon_box_gbimpl.hpp"

#include <pkmn/exception.hpp>

#include <cstring>

#define NATIVE_RCAST (reinterpret_cast<pkmn::gen1_pokemon_boxes_t*>(_native))

namespace pkmn {

    pokemon_pc_gen1impl::pokemon_pc_gen1impl(
        int game_id
    ): pokemon_pc_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new pkmn::gen1_pokemon_boxes_t);
        for(int i = 0; i < GEN1_NUM_BOXES; ++i) {
            NATIVE_RCAST->boxes[i] = new pksav_gen1_pokemon_box_t;
            std::memset(NATIVE_RCAST->boxes[i], 0, sizeof(pksav_gen1_pokemon_box_t));
            std::memset(NATIVE_RCAST->boxes[i]->nicknames, 0x50, sizeof(NATIVE_RCAST->boxes[i]->nicknames));
            std::memset(NATIVE_RCAST->boxes[i]->otnames, 0x50, sizeof(NATIVE_RCAST->boxes[i]->otnames));
            NATIVE_RCAST->boxes[i]->species[20] = 0xFF;
        }
        _our_mem = true;

        _from_native();
    }

    pokemon_pc_gen1impl::pokemon_pc_gen1impl(
        int game_id,
        pksav_gen1_pokemon_box_t* native,
        bool copy
    ): pokemon_pc_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new pkmn::gen1_pokemon_boxes_t);
        for(int i = 0; i < GEN1_NUM_BOXES; ++i) {
            if(copy) {
                NATIVE_RCAST->boxes[i] = new pksav_gen1_pokemon_box_t;
                *NATIVE_RCAST->boxes[i] = native[i];
            } else {
                NATIVE_RCAST->boxes[i] = &native[i];
            }
        }
        _our_mem = copy;

        _from_native();
    }

    pokemon_pc_gen1impl::pokemon_pc_gen1impl(
        int game_id,
        pksav_gen1_pokemon_box_t** native,
        bool copy
    ): pokemon_pc_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new pkmn::gen1_pokemon_boxes_t);
        for(int i = 0; i < GEN1_NUM_BOXES; ++i) {
            if(copy) {
                NATIVE_RCAST->boxes[i] = new pksav_gen1_pokemon_box_t;
                *NATIVE_RCAST->boxes[i] = *native[i];
            } else {
                NATIVE_RCAST->boxes[i] = native[i];
            }
        }
        _our_mem = copy;

        _from_native();
    }

    pokemon_pc_gen1impl::~pokemon_pc_gen1impl() {
        // _our_mem applies to the box pointers, not the struct
        if(_our_mem) {
            for(int i = 0; i < GEN1_NUM_BOXES; ++i) {
                delete NATIVE_RCAST->boxes[i];
            }
        }

        delete NATIVE_RCAST;
    }

    int pokemon_pc_gen1impl::get_num_boxes() {
        return GEN1_NUM_BOXES;
    }

    void pokemon_pc_gen1impl::_from_native() {
        _box_list.resize(GEN1_NUM_BOXES);

        for(int i = 0; i < GEN1_NUM_BOXES; ++i) {
            _box_list[i] = std::make_shared<pokemon_box_gen1impl>(
                               _game_id,
                               NATIVE_RCAST->boxes[i]
                           );
        }
    }

    void pokemon_pc_gen1impl::_update_box_names() {
        throw pkmn::feature_not_in_game_error("Box names", "Generation I");
    }
}

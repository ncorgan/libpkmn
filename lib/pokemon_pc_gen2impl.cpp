/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_pc_gen2impl.hpp"

#include "pokemon_box_gbimpl.hpp"

#include "pksav/pksav_call.hpp"

#include <pksav/gen2/text.h>

#include <cstring>

#define NATIVE_RCAST reinterpret_cast<pkmn::gen2_pokemon_full_pc_t*>(_native)

BOOST_STATIC_CONSTEXPR uint8_t GEN2_TEXT_TERMINATOR = 0x50;

namespace pkmn {

    pokemon_pc_gen2impl::pokemon_pc_gen2impl(
        int game_id
    ): pokemon_pc_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new pkmn::gen2_pokemon_full_pc_t);
        NATIVE_RCAST->pc = new pksav_gen2_pokemon_pc_t;
        NATIVE_RCAST->box_names = new pksav_gen2_pokemon_box_names_t;
        _our_mem = true;

        for(int i = 0; i < GEN2_NUM_BOXES; ++i) {
            std::memset(&NATIVE_RCAST->pc->boxes[i], 0, sizeof(pksav_gen2_pokemon_box_t));
            std::memset(NATIVE_RCAST->box_names, GEN2_TEXT_TERMINATOR, sizeof(pksav_gen2_pokemon_box_names_t));
            NATIVE_RCAST->pc->boxes[i].species[20] = 0xFF;
        }

        _from_native();
    }

    pokemon_pc_gen2impl::pokemon_pc_gen2impl(
        int game_id,
        pksav_gen2_pokemon_pc_t* pc,
        pksav_gen2_pokemon_box_names_t* box_names,
        bool copy
    ): pokemon_pc_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new pkmn::gen2_pokemon_full_pc_t);
        for(int i = 0; i < GEN2_NUM_BOXES; ++i) {
            if(copy) {
                NATIVE_RCAST->pc = new pksav_gen2_pokemon_pc_t;
                NATIVE_RCAST->box_names = new pksav_gen2_pokemon_box_names_t;

                *NATIVE_RCAST->pc = *pc;
                *NATIVE_RCAST->box_names = *box_names;
            } else {
                NATIVE_RCAST->pc = pc;
                NATIVE_RCAST->box_names = box_names;
            }
        }
        _our_mem = copy;

        _from_native();
    }

    pokemon_pc_gen2impl::~pokemon_pc_gen2impl() {
        // _our_mem applies to the struct members, not the struct itself
        if(_our_mem) {
            delete NATIVE_RCAST->pc;
            delete NATIVE_RCAST->box_names;
        }

        delete NATIVE_RCAST;
    }

    int pokemon_pc_gen2impl::get_num_boxes() {
        return GEN2_NUM_BOXES;
    }

    void pokemon_pc_gen2impl::_from_native() {
        _box_list.resize(GEN2_NUM_BOXES);

        for(int i = 0; i < GEN2_NUM_BOXES; ++i) {
            _box_list[i] = pkmn::make_shared<pokemon_box_gen2impl>(
                               _game_id,
                               &NATIVE_RCAST->pc->boxes[i]
                           );

            char box_name[10] = {0};
            PKSAV_CALL(
                pksav_text_from_gen2(
                    NATIVE_RCAST->box_names->names[i],
                    box_name,
                    9
                );
            )
            _box_list[i]->set_name(box_name);
        }
    }

    void pokemon_pc_gen2impl::_update_box_names() {
        _box_names.resize(GEN2_NUM_BOXES);

        for(int i = 0; i < GEN2_NUM_BOXES; ++i) {
            _box_names[i] = _box_list[i]->get_name();

            PKSAV_CALL(
                pksav_text_to_gen2(
                    _box_names[i].c_str(),
                    NATIVE_RCAST->box_names->names[i],
                    9
                );
            )
        }
    }
}

/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_pc_gen6impl.hpp"

#include "pokemon_box_gen6impl.hpp"

#include "pksav/pksav_call.hpp"

#include <pksav/gen6/text.h>

#include <cstring>

#define NATIVE_RCAST (reinterpret_cast<pkmn::gen6_pokemon_full_pc_t*>(_native))

namespace pkmn {

    pokemon_pc_gen6impl::pokemon_pc_gen6impl(
        int game_id
    ): pokemon_pc_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new pkmn::gen6_pokemon_full_pc_t);
        NATIVE_RCAST->pc_ptr = new pksav_gen6_pokemon_pc_t;
        NATIVE_RCAST->box_info_ptr = new pksav_gen6_pokemon_box_info_t;

        // Fill the box names with the terminator. Zero out everything else.
        std::memset(
            NATIVE_RCAST->pc_ptr,
            0,
            sizeof(*(NATIVE_RCAST->pc_ptr))
        );
        std::memset(
            NATIVE_RCAST->box_info_ptr->names,
            0xFF,
            sizeof(NATIVE_RCAST->box_info_ptr->names)
        );
        std::memset(
            NATIVE_RCAST->box_info_ptr->backgrounds,
            0,
            sizeof(NATIVE_RCAST->box_info_ptr->backgrounds)
        );
        std::memset(
            NATIVE_RCAST->box_info_ptr->flags,
            0,
            sizeof(NATIVE_RCAST->box_info_ptr->flags)
        );

        _our_mem = true;

        _from_native();
    }

    pokemon_pc_gen6impl::pokemon_pc_gen6impl(
        int game_id,
        pksav_gen6_pokemon_pc_t* pc_ptr,
        pksav_gen6_pokemon_box_info_t* box_info_ptr,
        bool copy
    ): pokemon_pc_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new pkmn::gen6_pokemon_full_pc_t);
        if(copy)
        {
            *(NATIVE_RCAST->pc_ptr) = *pc_ptr;
            *(NATIVE_RCAST->box_info_ptr) = *box_info_ptr;
        }
        else
        {
            NATIVE_RCAST->pc_ptr = pc_ptr;
            NATIVE_RCAST->box_info_ptr = box_info_ptr;
        }

        _our_mem = copy;

        _from_native();
    }

    pokemon_pc_gen6impl::~pokemon_pc_gen6impl()
    {
        // _our_mem applies to the struct members, not the struct itself
        if(_our_mem)
        {
            delete NATIVE_RCAST->pc_ptr;
            delete NATIVE_RCAST->box_info_ptr;
        }

        delete NATIVE_RCAST;
    }

    int pokemon_pc_gen6impl::get_num_boxes()
    {
        return PKSAV_GEN6_NUM_PC_BOXES;
    }

    void pokemon_pc_gen6impl::_from_native()
    {
        _box_list.resize(PKSAV_GEN6_NUM_PC_BOXES);

        for(int i = 0; i < PKSAV_GEN6_NUM_PC_BOXES; ++i)
        {
            _box_list[i] = pkmn::make_shared<pokemon_box_gen6impl>(
                               _game_id,
                               &NATIVE_RCAST->pc_ptr->boxes[i]
                           );

            char box_name[10] = {0};
            PKSAV_CALL(
                pksav_text_from_gen6(
                    NATIVE_RCAST->box_info_ptr->names[i].name,
                    box_name,
                    9
                );
            )
            _box_list[i]->set_name(box_name);
        }
    }

    void pokemon_pc_gen6impl::_update_box_names()
    {
        _box_names.resize(PKSAV_GEN6_NUM_PC_BOXES);

        for(int i = 0; i < PKSAV_GEN6_NUM_PC_BOXES; ++i) {
            _box_names[i] = _box_list[i]->get_name();

            PKSAV_CALL(
                pksav_text_to_gen6(
                    _box_names[i].c_str(),
                    NATIVE_RCAST->box_info_ptr->names[i].name,
                    9
                );
            )
        }
    }
}

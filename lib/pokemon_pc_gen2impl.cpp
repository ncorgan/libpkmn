/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_pc_gen2impl.hpp"

#include "pokemon_box_gbimpl.hpp"

#include "pksav/pksav_call.hpp"

#include <pksav/gen2/text.h>

#include <boost/thread/lock_guard.hpp>

#include <cstring>

#define NATIVE_RCAST (reinterpret_cast<pkmn::gen2_pokemon_full_pc_t*>(_native))

BOOST_STATIC_CONSTEXPR uint8_t GEN2_TEXT_TERMINATOR = 0x50;

namespace pkmn {

    pokemon_pc_gen2impl::pokemon_pc_gen2impl(
        int game_id
    ): pokemon_pc_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new pkmn::gen2_pokemon_full_pc_t);
        for(int i = 0; i < GEN2_NUM_BOXES; ++i) {
            NATIVE_RCAST->boxes[i] = new struct pksav_gen2_pokemon_box;
            std::memset(NATIVE_RCAST->boxes[i], 0, sizeof(struct pksav_gen2_pokemon_box));
            std::memset(NATIVE_RCAST->boxes[i]->nicknames, 0x50, sizeof(NATIVE_RCAST->boxes[i]->nicknames));
            std::memset(NATIVE_RCAST->boxes[i]->otnames, 0x50, sizeof(NATIVE_RCAST->boxes[i]->otnames));
            NATIVE_RCAST->boxes[i]->species[20] = 0xFF;
        }
        NATIVE_RCAST->box_names = new struct pksav_gen2_pokemon_box_names;
        std::memset(NATIVE_RCAST->box_names, GEN2_TEXT_TERMINATOR, sizeof(struct pksav_gen2_pokemon_box_names));

        _our_mem = true;

        _from_native();
    }

    pokemon_pc_gen2impl::pokemon_pc_gen2impl(
        int game_id,
        struct pksav_gen2_pokemon_box** boxes,
        struct pksav_gen2_pokemon_box_names* box_names,
        bool copy
    ): pokemon_pc_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new pkmn::gen2_pokemon_full_pc_t);
        if(copy)
        {
            for(int i = 0; i < GEN2_NUM_BOXES; ++i)
            {
                NATIVE_RCAST->boxes[i] = new struct pksav_gen2_pokemon_box;
                *NATIVE_RCAST->boxes[i] = *boxes[i];
            }
            NATIVE_RCAST->box_names = new struct pksav_gen2_pokemon_box_names;
            *NATIVE_RCAST->box_names = *box_names;
        }
        else
        {
            for(int i = 0; i < GEN2_NUM_BOXES; ++i)
            {
                NATIVE_RCAST->boxes[i] = boxes[i];
            }
            NATIVE_RCAST->box_names = box_names;
        }

        _our_mem = copy;

        _from_native();
    }

    pokemon_pc_gen2impl::~pokemon_pc_gen2impl()
    {
        boost::lock_guard<pokemon_pc_gen2impl> lock(*this);

        // _our_mem applies to the struct members, not the struct itself
        if(_our_mem)
        {
            for(int i = 0; i < GEN2_NUM_BOXES; ++i)
            {
                delete NATIVE_RCAST->boxes[i];
            }
            delete NATIVE_RCAST->box_names;
        }

        delete NATIVE_RCAST;
    }

    int pokemon_pc_gen2impl::get_num_boxes()
    {
        return GEN2_NUM_BOXES;
    }

    void pokemon_pc_gen2impl::_from_native()
    {
        _box_list.resize(GEN2_NUM_BOXES);

        for(int i = 0; i < GEN2_NUM_BOXES; ++i)
        {
            _box_list[i] = std::make_shared<pokemon_box_gen2impl>(
                               _game_id,
                               NATIVE_RCAST->boxes[i]
                           );

            char box_name[10] = {0};
            PKSAV_CALL(
                pksav_gen2_import_text(
                    NATIVE_RCAST->box_names->names[i],
                    box_name,
                    9
                );
            )
            _box_list[i]->set_name(box_name);
        }
    }

    void pokemon_pc_gen2impl::_update_box_names()
    {
        _box_names.resize(GEN2_NUM_BOXES);

        for(int i = 0; i < GEN2_NUM_BOXES; ++i)
        {
            _box_names[i] = _box_list[i]->get_name();

            PKSAV_CALL(
                pksav_gen2_export_text(
                    _box_names[i].c_str(),
                    NATIVE_RCAST->box_names->names[i],
                    9
                );
            )
        }
    }
}

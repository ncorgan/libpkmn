/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_pc_gbaimpl.hpp"
#include "pokemon_box_gbaimpl.hpp"

#include "pksav/pksav_call.hpp"

#include <pksav/gba/pokemon.h>
#include <pksav/gba/text.h>

#include <boost/thread/lock_guard.hpp>

#include <cstring>

#define NATIVE_RCAST (reinterpret_cast<struct pksav_gba_pokemon_pc*>(_native))

BOOST_STATIC_CONSTEXPR uint8_t GBA_TEXT_TERMINATOR = 0xFF;

namespace pkmn {

    pokemon_pc_gbaimpl::pokemon_pc_gbaimpl(
        int game_id
    ): pokemon_pc_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new struct pksav_gba_pokemon_pc);
        std::memset(_native, 0, sizeof(struct pksav_gba_pokemon_pc));
        std::memset(NATIVE_RCAST->box_names, GBA_TEXT_TERMINATOR, sizeof(NATIVE_RCAST->box_names));
        _our_mem = true;

        _from_native();
    }

    pokemon_pc_gbaimpl::pokemon_pc_gbaimpl(
        int game_id,
        struct pksav_gba_pokemon_pc* native
    ): pokemon_pc_impl(game_id)
    {
        _native = reinterpret_cast<void*>(native);
        _our_mem = false;

        _from_native();
    }

    pokemon_pc_gbaimpl::pokemon_pc_gbaimpl(
        int game_id,
        const struct pksav_gba_pokemon_pc &native
    ): pokemon_pc_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new struct pksav_gba_pokemon_pc);
        *NATIVE_RCAST = native;
        _our_mem = true;

        _from_native();
    }

    pokemon_pc_gbaimpl::~pokemon_pc_gbaimpl()
    {
        boost::lock_guard<pokemon_pc_gbaimpl> lock(*this);

        if(_our_mem)
        {
            delete NATIVE_RCAST;
        }
    }

    int pokemon_pc_gbaimpl::get_num_boxes() {
        return GBA_NUM_BOXES;
    }

    void pokemon_pc_gbaimpl::_from_native()
    {
        _box_list.resize(GBA_NUM_BOXES);

        for(int i = 0; i < GBA_NUM_BOXES; ++i)
        {
            _box_list[i] = std::make_shared<pokemon_box_gbaimpl>(
                               _game_id,
                               &NATIVE_RCAST->boxes[i]
                           );

            char box_name[9] = {0};
            PKSAV_CALL(
                pksav_gba_import_text(
                    NATIVE_RCAST->box_names[i],
                    box_name,
                    8
                );
            )
            _box_list[i]->set_name(box_name);
        }
    }

    void pokemon_pc_gbaimpl::_update_box_names()
    {
        _box_names.resize(GBA_NUM_BOXES);

        for(int i = 0; i < GBA_NUM_BOXES; ++i)
        {
            _box_names[i] = _box_list[i]->get_name();

            PKSAV_CALL(
                pksav_gba_export_text(
                    _box_names[i].c_str(),
                    NATIVE_RCAST->box_names[i],
                    8
                );
            )
        }
    }
}

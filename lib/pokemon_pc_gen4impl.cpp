/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_pc_gen4impl.hpp"
#include "pokemon_box_gen4impl.hpp"

#include "pksav/pksav_call.hpp"

#include <pksav/gen4/pokemon.h>
#include <pksav/gen4/text.h>

#include <cstring>

#define NATIVE_RCAST reinterpret_cast<pksav_gen4_pokemon_pc_t*>(_native)
#define DPPT_RCAST   reinterpret_cast<pksav_dppt_pokemon_pc_t*>(_native)
#define HGSS_RCAST   reinterpret_cast<pksav_hgss_pokemon_pc_t*>(_native)

#define NATIVE_BOX_NAMES (_hgss ? HGSS_RCAST->box_names : DPPT_RCAST->box_names)
#define NATIVE_SIZE      (_hgss ? sizeof(*DPPT_RCAST) : sizeof(*HGSS_RCAST))

BOOST_STATIC_CONSTEXPR uint8_t GEN4_TEXT_TERMINATOR_BYTE = 0xFF;

namespace pkmn {

    BOOST_STATIC_CONSTEXPR int HEARTGOLD  = 15;
    BOOST_STATIC_CONSTEXPR int SOULSILVER = 16;

    pokemon_pc_gen4impl::pokemon_pc_gen4impl(
        int game_id
    ): pokemon_pc_impl(game_id),
       _hgss(game_id == HEARTGOLD or game_id == SOULSILVER)
    {
        _native = reinterpret_cast<void*>(new pksav_gen4_pokemon_pc_t);
        std::memset(_native, 0, NATIVE_SIZE);
        std::memset(NATIVE_BOX_NAMES, GEN4_TEXT_TERMINATOR_BYTE, sizeof(pksav_gen4_box_name_t)*GEN4_NUM_BOXES);
        _our_mem = true;

        _from_native();
    }

    pokemon_pc_gen4impl::pokemon_pc_gen4impl(
        int game_id,
        pksav_gen4_pokemon_pc_t* native
    ): pokemon_pc_impl(game_id),
       _hgss(game_id == HEARTGOLD or game_id == SOULSILVER)
    {
        _native = reinterpret_cast<void*>(native);
        _our_mem = false;

        _from_native();
    }

    pokemon_pc_gen4impl::~pokemon_pc_gen4impl() {
        if(_our_mem) {
            delete NATIVE_RCAST;
        }
    }

    int pokemon_pc_gen4impl::get_num_boxes() {
        return GEN4_NUM_BOXES;
    }

    void pokemon_pc_gen4impl::_from_native() {
        _box_list.resize(GEN4_NUM_BOXES);

        for(int i = 0; i < GEN4_NUM_BOXES; ++i) {
            if(_hgss) {
                _box_list[i] = pkmn::make_shared<pokemon_box_gen4impl>(
                                   _game_id,
                                   &HGSS_RCAST->boxes[i]
                               );
            } else {
                _box_list[i] = pkmn::make_shared<pokemon_box_gen4impl>(
                                   _game_id,
                                   &DPPT_RCAST->boxes[i]
                               );
            }

            char box_name[9] = {0};
            PKSAV_CALL(
                pksav_text_from_gen4(
                    NATIVE_BOX_NAMES[i].name,
                    box_name,
                    10
                );
            )
            _box_list[i]->set_name(box_name);
        }
    }

    void pokemon_pc_gen4impl::_update_box_names() {
        _box_names.resize(GEN4_NUM_BOXES);

        for(int i = 0; i < GEN4_NUM_BOXES; ++i) {
            _box_names[i] = _box_list[i]->get_name();

            PKSAV_CALL(
                pksav_text_to_gen4(
                    _box_names[i].c_str(),
                    NATIVE_BOX_NAMES[i].name,
                    10
                );
            )
        }
    }
}

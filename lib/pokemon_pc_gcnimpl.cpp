/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_pc_gcnimpl.hpp"
#include "pokemon_box_gcnimpl.hpp"

#include "misc_common.hpp"

#define NATIVE_RCAST (reinterpret_cast<LibPkmGC::GC::PokemonBox**>(_native))
#define GCN_NUM_BOXES (_game_id == COLOSSEUM ? COLOSSEUM_NUM_BOXES : XD_NUM_BOXES)

namespace pkmn {

    pokemon_pc_gcnimpl::pokemon_pc_gcnimpl(
        int game_id
    ): pokemon_pc_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new LibPkmGC::GC::PokemonBox*[GCN_NUM_BOXES]);
        for(int i = 0; i < GCN_NUM_BOXES; ++i) {
            if(_game_id == COLOSSEUM) {
                NATIVE_RCAST[i] = new LibPkmGC::Colosseum::PokemonBox;
            } else {
                NATIVE_RCAST[i] = new LibPkmGC::XD::PokemonBox;
            }
        }
        _our_mem = true;

        _from_native();
    }

    pokemon_pc_gcnimpl::pokemon_pc_gcnimpl(
        int game_id,
        LibPkmGC::GC::PokemonBox** native
    ): pokemon_pc_impl(game_id)
    {
        _native = reinterpret_cast<void*>(native);
        _our_mem = false;

        _from_native();
    }

    pokemon_pc_gcnimpl::~pokemon_pc_gcnimpl() {
        if(_our_mem) {
            for(int i = 0; i < GCN_NUM_BOXES; ++i) {
                if(_game_id == COLOSSEUM) {
                    delete reinterpret_cast<LibPkmGC::Colosseum::PokemonBox*>(NATIVE_RCAST[i]);
                } else {
                    delete reinterpret_cast<LibPkmGC::XD::PokemonBox*>(NATIVE_RCAST[i]);
                }
            }
            delete[] NATIVE_RCAST;
        }
    }

    int pokemon_pc_gcnimpl::get_num_boxes() {
        return GCN_NUM_BOXES;
    }

    void pokemon_pc_gcnimpl::_from_native() {
        _box_list.resize(GCN_NUM_BOXES);

        for(int i = 0; i < GCN_NUM_BOXES; ++i) {
            _box_list[i] = std::make_shared<pokemon_box_gcnimpl>(
                               _game_id,
                               NATIVE_RCAST[i]
                           );
        }
        _update_box_names();
    }

    void pokemon_pc_gcnimpl::_update_box_names() {
        _box_names.resize(GCN_NUM_BOXES);

        for(int i = 0; i < GCN_NUM_BOXES; ++i) {
            _box_names[i] = _box_list[i]->get_name();
        }
    }
}

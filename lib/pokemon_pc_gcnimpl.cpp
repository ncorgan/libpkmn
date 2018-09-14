/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_pc_gcnimpl.hpp"
#include "pokemon_box_gcnimpl.hpp"

#include "utils/misc.hpp"

#define GCN_NUM_BOXES (_game_id == COLOSSEUM_ID ? COLOSSEUM_NUM_BOXES : XD_NUM_BOXES)

namespace pkmn {

    pokemon_pc_gcnimpl::pokemon_pc_gcnimpl(
        int game_id,
        LibPkmGC::GC::PokemonBox* const* pp_native
    ): pokemon_pc_impl(game_id)
    {
        _libpkmgc_box_uptrs.resize(GCN_NUM_BOXES);

        if(pp_native != nullptr)
        {
            for(size_t box_index = 0; box_index < GCN_NUM_BOXES; ++box_index)
            {
                _libpkmgc_box_uptrs[box_index].reset(
                    pp_native[box_index]->clone()
                );
            }
        }
        else
        {
            for(size_t box_index = 0; box_index < GCN_NUM_BOXES; ++box_index)
            {
                if(_game_id == COLOSSEUM_ID)
                {
                    _libpkmgc_box_uptrs[box_index].reset(
                        new LibPkmGC::Colosseum::PokemonBox
                    );
                }
                else
                {
                    _libpkmgc_box_uptrs[box_index].reset(
                        new LibPkmGC::XD::PokemonBox
                    );
                }
            }
        }

        _p_native = _libpkmgc_box_uptrs.data();

        _from_native();
    }

    int pokemon_pc_gcnimpl::get_num_boxes()
    {
        return GCN_NUM_BOXES;
    }

    void pokemon_pc_gcnimpl::_from_native()
    {
        _box_list.resize(GCN_NUM_BOXES);
        BOOST_ASSERT(_box_list.size() == _libpkmgc_box_uptrs.size());

        for(size_t box_index = 0; box_index < GCN_NUM_BOXES; ++box_index)
        {
            _box_list[box_index] = std::make_shared<pokemon_box_gcnimpl>(
                                       _game_id,
                                       _libpkmgc_box_uptrs[box_index].get()
                                   );
        }
        _update_box_names();
    }

    void pokemon_pc_gcnimpl::_to_native()
    {
        BOOST_ASSERT(_box_list.size() == _libpkmgc_box_uptrs.size());

        for(size_t box_index = 0; box_index < GCN_NUM_BOXES; ++box_index)
        {
            _libpkmgc_box_uptrs[box_index].reset(
                static_cast<LibPkmGC::GC::PokemonBox*>(
                    _box_list[box_index]->get_native()
                )->clone()
            );
        }
    }

    void pokemon_pc_gcnimpl::_update_box_names()
    {
        _box_names.resize(GCN_NUM_BOXES);

        for(size_t box_index = 0; box_index < GCN_NUM_BOXES; ++box_index)
        {
            _box_names[box_index] = _box_list[box_index]->get_name();
        }
    }

    void pokemon_pc_gcnimpl::_update_native_box_wallpapers()
    {
        BOOST_ASSERT_MSG(false, "No box wallpapers in Gamecube games, so this should never be called");
    }
}

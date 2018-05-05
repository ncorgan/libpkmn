/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_pc_gen1impl.hpp"

#include "pokemon_box_gbimpl.hpp"

#include <pkmn/exception.hpp>

#include <boost/thread/lock_guard.hpp>

#include <cstring>

#define NATIVE_RCAST (reinterpret_cast<struct pksav_gen1_pokemon_storage*>(_native))

namespace pkmn {

    static void _set_initial_pokemon_box_mem(
        struct pksav_gen1_pokemon_box* p_box
    )
    {
        BOOST_ASSERT(p_box != nullptr);

        std::memset(p_box, 0, sizeof(*p_box));
        std::memset(
            p_box->nicknames,
            PKSAV_GEN1_TEXT_TERMINATOR,
            sizeof(p_box->nicknames)
        );
        std::memset(
            p_box->otnames,
            PKSAV_GEN1_TEXT_TERMINATOR,
            sizeof(p_box->otnames)
        );
    }

    pokemon_pc_gen1impl::pokemon_pc_gen1impl(
        int game_id,
        struct pksav_gen1_pokemon_storage* p_native
    ): pokemon_pc_impl(game_id)
    {
        _our_mem = (p_native == nullptr);

        if(_our_mem)
        {
            _native = new struct pksav_gen1_pokemon_storage;

            for(size_t box_index = 0;
                box_index < PKSAV_GEN1_NUM_POKEMON_BOXES;
                ++box_index)
            {
                NATIVE_RCAST->pp_boxes[box_index] = new struct pksav_gen1_pokemon_box;
                _set_initial_pokemon_box_mem(NATIVE_RCAST->pp_boxes[box_index]);
            }

            NATIVE_RCAST->p_current_box = new struct pksav_gen1_pokemon_box;
            _set_initial_pokemon_box_mem(NATIVE_RCAST->p_current_box);

            NATIVE_RCAST->p_current_box_num = new uint8_t(0);
        }
        else
        {
            _native = p_native;

#ifndef NDEBUG
            for(size_t box_index = 0;
                box_index < PKSAV_GEN1_NUM_POKEMON_BOXES;
                ++box_index)
            {
                BOOST_ASSERT(NATIVE_RCAST->pp_boxes[box_index] != nullptr);
            }

            BOOST_ASSERT(NATIVE_RCAST->p_current_box != nullptr);
            BOOST_ASSERT(NATIVE_RCAST->p_current_box_num != nullptr);
#endif
        }

        _from_native();
    }

    pokemon_pc_gen1impl::~pokemon_pc_gen1impl()
    {
        if(_our_mem)
        {
            for(size_t box_index = 0;
                box_index < PKSAV_GEN1_NUM_POKEMON_BOXES;
                ++box_index)
            {
                delete NATIVE_RCAST->pp_boxes[box_index];
            }

            delete NATIVE_RCAST->p_current_box;
            delete NATIVE_RCAST->p_current_box_num;

            delete NATIVE_RCAST;
        }
    }

    int pokemon_pc_gen1impl::get_num_boxes()
    {
        return PKSAV_GEN1_NUM_POKEMON_BOXES;
    }

    void pokemon_pc_gen1impl::_from_native()
    {
        _box_list.resize(PKSAV_GEN1_NUM_POKEMON_BOXES);

        uint8_t current_box_num = *NATIVE_RCAST->p_current_box_num;
        current_box_num &= PKSAV_GEN1_CURRENT_POKEMON_BOX_NUM_MASK;
        BOOST_ASSERT(current_box_num < PKSAV_GEN1_NUM_POKEMON_BOXES);

        // In Generation I, the current box is stored in a specific memory
        // location, so for that number box, use that pointer instead of
        // what's in the array.
        for(size_t box_index = 0;
            box_index < PKSAV_GEN1_NUM_POKEMON_BOXES;
            ++box_index)
        {
            if(box_index == current_box_num)
            {
                _box_list[box_index] = std::make_shared<pokemon_box_gen1impl>(
                                           _game_id,
                                           NATIVE_RCAST->p_current_box
                                       );
            }
            else
            {
                _box_list[box_index] = std::make_shared<pokemon_box_gen1impl>(
                                           _game_id,
                                           NATIVE_RCAST->pp_boxes[box_index]
                                       );
            }
        }
    }

    void pokemon_pc_gen1impl::_update_box_names()
    {
        BOOST_ASSERT_MSG(false, "No box names in Generation I, so this should never be called");
    }

    void pokemon_pc_gen1impl::_update_native_box_wallpapers()
    {
        BOOST_ASSERT_MSG(false, "No box wallpapers in Generation I, so this should never be called");
    }
}

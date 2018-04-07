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
        struct pksav_gen1_pokemon_box* box_ptr
    )
    {
        BOOST_ASSERT(box_ptr != nullptr);

        std::memset(box_ptr, 0, sizeof(*box_ptr));
        std::memset(
            box_ptr->nicknames,
            PKSAV_GEN1_TEXT_TERMINATOR,
            sizeof(box_ptr->nicknames)
        );
        std::memset(
            box_ptr->otnames,
            PKSAV_GEN1_TEXT_TERMINATOR,
            sizeof(box_ptr->otnames)
        );
    }

    pokemon_pc_gen1impl::pokemon_pc_gen1impl(
        int game_id,
        struct pksav_gen1_pokemon_storage* native_ptr
    ): pokemon_pc_impl(game_id)
    {
        _our_mem = (native_ptr == nullptr);

        if(_our_mem)
        {
            _native = new struct pksav_gen1_pokemon_storage;

            for(size_t box_index = 0;
                box_index < PKSAV_GEN1_NUM_POKEMON_BOXES;
                ++box_index)
            {
                NATIVE_RCAST->box_ptrs[box_index] = new struct pksav_gen1_pokemon_box;
                _set_initial_pokemon_box_mem(NATIVE_RCAST->box_ptrs[box_index]);
            }

            NATIVE_RCAST->current_box_ptr = new struct pksav_gen1_pokemon_box;
            _set_initial_pokemon_box_mem(NATIVE_RCAST->current_box_ptr);

            NATIVE_RCAST->current_box_num_ptr = new uint8_t(0);
        }
        else
        {
            _native = native_ptr;

#ifndef NDEBUG
            for(size_t box_index = 0;
                box_index < PKSAV_GEN1_NUM_POKEMON_BOXES;
                ++box_index)
            {
                BOOST_ASSERT(NATIVE_RCAST->box_ptrs[box_index] != nullptr);
            }

            BOOST_ASSERT(NATIVE_RCAST->current_box_ptr != nullptr);
            BOOST_ASSERT(NATIVE_RCAST->current_box_num_ptr != nullptr);
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
                delete NATIVE_RCAST->box_ptrs[box_index];
            }

            delete NATIVE_RCAST->current_box_ptr;
            delete NATIVE_RCAST->current_box_num_ptr;

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

        uint8_t current_box_num = *NATIVE_RCAST->current_box_num_ptr;
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
                                           NATIVE_RCAST->current_box_ptr
                                       );
            }
            else
            {
                _box_list[box_index] = std::make_shared<pokemon_box_gen1impl>(
                                           _game_id,
                                           NATIVE_RCAST->box_ptrs[box_index]
                                       );
            }
        }
    }

    void pokemon_pc_gen1impl::_update_box_names()
    {
        throw pkmn::feature_not_in_game_error("Box names", "Generation I");
    }
}

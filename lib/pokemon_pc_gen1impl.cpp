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
        const struct pksav_gen1_pokemon_storage* p_native
    ): pokemon_pc_impl(game_id)
    {
        std::memset(
            &_pksav_storage,
            0,
            sizeof(_pksav_storage)
        );
        _native_boxes.resize(PKSAV_GEN1_NUM_POKEMON_BOXES);

        if(p_native != nullptr)
        {
            BOOST_ASSERT(p_native->p_current_box_num != nullptr);
            BOOST_ASSERT(p_native->p_current_box != nullptr);

            for(size_t box_index = 0;
                box_index < PKSAV_GEN1_NUM_POKEMON_BOXES;
                ++box_index)
            {
                BOOST_ASSERT(p_native->pp_boxes[box_index] != nullptr);

                _native_boxes[box_index] = *(p_native->pp_boxes[box_index]);
            }

            _current_box_num = *(p_native->p_current_box_num);
            _current_box = *(p_native->p_current_box);
        }
        else
        {
            for(size_t box_index = 0;
                box_index < PKSAV_GEN1_NUM_POKEMON_BOXES;
                ++box_index)
            {
                _set_initial_pokemon_box_mem(&_native_boxes[box_index]);
            }

            _current_box_num = 0;
            _set_initial_pokemon_box_mem(&_current_box);
        }

        _pksav_storage.p_current_box_num = &_current_box_num;
        _pksav_storage.p_current_box = &_current_box;
        for(size_t box_index = 0;
            box_index < PKSAV_GEN1_NUM_POKEMON_BOXES;
            ++box_index)
        {
            _pksav_storage.pp_boxes[box_index] = &_native_boxes[box_index];
        }

        _p_native = &_pksav_storage;

        _from_native();
    }

    int pokemon_pc_gen1impl::get_num_boxes()
    {
        return PKSAV_GEN1_NUM_POKEMON_BOXES;
    }

    void pokemon_pc_gen1impl::_from_native()
    {
        _box_list.resize(PKSAV_GEN1_NUM_POKEMON_BOXES);

        uint8_t current_box_num = _current_box_num & PKSAV_GEN1_CURRENT_POKEMON_BOX_NUM_MASK;
        bool is_current_box_num_valid = (current_box_num < PKSAV_GEN1_NUM_POKEMON_BOXES);

        // In Generation I, the current box is stored in a specific memory
        // location, so for that number box, use that pointer instead of
        // what's in the array.
        for(size_t box_index = 0;
            box_index < PKSAV_GEN1_NUM_POKEMON_BOXES;
            ++box_index)
        {
            if(is_current_box_num_valid && (box_index == current_box_num))
            {
                _box_list[box_index] = std::make_shared<pokemon_box_gen1impl>(
                                           _game_id,
                                           _pksav_storage.p_current_box
                                       );
            }
            else
            {
                _box_list[box_index] = std::make_shared<pokemon_box_gen1impl>(
                                           _game_id,
                                           _pksav_storage.pp_boxes[box_index]
                                       );
            }
        }
    }

    void pokemon_pc_gen1impl::_to_native()
    {
        uint8_t current_box_num = _current_box_num & PKSAV_GEN1_CURRENT_POKEMON_BOX_NUM_MASK;
        bool is_current_box_num_valid = (current_box_num < PKSAV_GEN1_NUM_POKEMON_BOXES);

        // In Generation I, the current box is stored in a specific memory
        // location, so for that number box, use that pointer instead of
        // what's in the array.
        for(size_t box_index = 0;
            box_index < PKSAV_GEN1_NUM_POKEMON_BOXES;
            ++box_index)
        {
            if(is_current_box_num_valid && (box_index == current_box_num))
            {
                pkmn::rcast_equal<struct pksav_gen1_pokemon_box>(
                    _box_list[box_index]->get_native(),
                    _pksav_storage.p_current_box
                );
            }
            else
            {
                pkmn::rcast_equal<struct pksav_gen1_pokemon_box>(
                    _box_list[box_index]->get_native(),
                    _pksav_storage.pp_boxes[box_index]
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

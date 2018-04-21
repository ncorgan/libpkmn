/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_pc_gen2impl.hpp"

#include "pokemon_box_gbimpl.hpp"

#include <pkmn/exception.hpp>

#include <boost/thread/lock_guard.hpp>

#include <cstring>

#define NATIVE_RCAST (reinterpret_cast<struct pksav_gen2_pokemon_storage*>(_native))

namespace pkmn {

    static void _set_initial_pokemon_box_mem(
        struct pksav_gen2_pokemon_box* box_ptr
    )
    {
        BOOST_ASSERT(box_ptr != nullptr);

        std::memset(box_ptr, 0, sizeof(*box_ptr));
        std::memset(
            box_ptr->nicknames,
            PKSAV_GEN2_TEXT_TERMINATOR,
            sizeof(box_ptr->nicknames)
        );
        std::memset(
            box_ptr->otnames,
            PKSAV_GEN2_TEXT_TERMINATOR,
            sizeof(box_ptr->otnames)
        );
    }

    pokemon_pc_gen2impl::pokemon_pc_gen2impl(
        int game_id,
        struct pksav_gen2_pokemon_storage* native_ptr
    ): pokemon_pc_impl(game_id)
    {
        _our_mem = (native_ptr == nullptr);

        if(_our_mem)
        {
            _native = new struct pksav_gen2_pokemon_storage;

            for(size_t box_index = 0;
                box_index < PKSAV_GEN2_NUM_POKEMON_BOXES;
                ++box_index)
            {
                NATIVE_RCAST->box_ptrs[box_index] = new struct pksav_gen2_pokemon_box;
                _set_initial_pokemon_box_mem(NATIVE_RCAST->box_ptrs[box_index]);
            }

            NATIVE_RCAST->box_names_ptr = new struct pksav_gen2_pokemon_box_names;
            std::memset(
                NATIVE_RCAST->box_names_ptr,
                PKSAV_GEN2_TEXT_TERMINATOR,
                sizeof(*NATIVE_RCAST->box_names_ptr)
            );

            NATIVE_RCAST->current_box_ptr = new struct pksav_gen2_pokemon_box;
            _set_initial_pokemon_box_mem(NATIVE_RCAST->current_box_ptr);

            NATIVE_RCAST->current_box_num_ptr = new uint8_t(0);
        }
        else
        {
            _native = native_ptr;

#ifndef NDEBUG
            for(size_t box_index = 0;
                box_index < PKSAV_GEN2_NUM_POKEMON_BOXES;
                ++box_index)
            {
                BOOST_ASSERT(NATIVE_RCAST->box_ptrs[box_index] != nullptr);
            }

            BOOST_ASSERT(NATIVE_RCAST->box_names_ptr != nullptr);
            BOOST_ASSERT(NATIVE_RCAST->current_box_ptr != nullptr);
            BOOST_ASSERT(NATIVE_RCAST->current_box_num_ptr != nullptr);
#endif
        }

        _from_native();
    }

    pokemon_pc_gen2impl::~pokemon_pc_gen2impl()
    {
        if(_our_mem)
        {
            for(size_t box_index = 0;
                box_index < PKSAV_GEN2_NUM_POKEMON_BOXES;
                ++box_index)
            {
                delete NATIVE_RCAST->box_ptrs[box_index];
            }

            delete NATIVE_RCAST->box_names_ptr;
            delete NATIVE_RCAST->current_box_ptr;
            delete NATIVE_RCAST->current_box_num_ptr;

            delete NATIVE_RCAST;
        }
    }

    int pokemon_pc_gen2impl::get_num_boxes()
    {
        return PKSAV_GEN2_NUM_POKEMON_BOXES;
    }

    void pokemon_pc_gen2impl::_from_native()
    {
        _box_list.resize(PKSAV_GEN2_NUM_POKEMON_BOXES);

        BOOST_ASSERT(*NATIVE_RCAST->current_box_num_ptr < PKSAV_GEN2_NUM_POKEMON_BOXES);

        // In Generation II, the current box is stored in a specific memory
        // location, so for that number box, use that pointer instead of
        // what's in the array.
        for(size_t box_index = 0;
            box_index < PKSAV_GEN2_NUM_POKEMON_BOXES;
            ++box_index)
        {
            if(box_index == *NATIVE_RCAST->current_box_num_ptr)
            {
                _box_list[box_index] = std::make_shared<pokemon_box_gen2impl>(
                                           _game_id,
                                           NATIVE_RCAST->current_box_ptr
                                       );
            }
            else
            {
                _box_list[box_index] = std::make_shared<pokemon_box_gen2impl>(
                                           _game_id,
                                           NATIVE_RCAST->box_ptrs[box_index]
                                       );
            }

            char box_name[PKSAV_GEN2_POKEMON_BOX_NAME_LENGTH + 1] = {0};
            PKSAV_CALL(
                pksav_gen2_import_text(
                    NATIVE_RCAST->box_names_ptr->names[box_index],
                    box_name,
                    PKSAV_GEN2_POKEMON_BOX_NAME_LENGTH
                );
            )
            _box_list[box_index]->set_name(box_name);
        }
    }

    void pokemon_pc_gen2impl::_update_box_names()
    {
        _box_names.resize(PKSAV_GEN2_NUM_POKEMON_BOXES);

        for(size_t box_index = 0;
            box_index < PKSAV_GEN2_NUM_POKEMON_BOXES;
            ++box_index)
        {
            _box_names[box_index] = _box_list[box_index]->get_name();
            BOOST_ASSERT(_box_names[box_index].size() <= PKSAV_GEN2_POKEMON_BOX_NAME_LENGTH);

            PKSAV_CALL(
                pksav_gen2_export_text(
                    _box_names[box_index].c_str(),
                    NATIVE_RCAST->box_names_ptr->names[box_index],
                    PKSAV_GEN2_POKEMON_BOX_NAME_LENGTH
                );
            )
        }
    }

    void pokemon_pc_gen2impl::_update_native_box_wallpapers()
    {
        BOOST_ASSERT_MSG(false, "No box wallpapers in Generation I, so this should never be called");
    }
}

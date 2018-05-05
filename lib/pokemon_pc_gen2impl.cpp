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
        struct pksav_gen2_pokemon_box* p_box
    )
    {
        BOOST_ASSERT(p_box != nullptr);

        std::memset(p_box, 0, sizeof(*p_box));
        std::memset(
            p_box->nicknames,
            PKSAV_GEN2_TEXT_TERMINATOR,
            sizeof(p_box->nicknames)
        );
        std::memset(
            p_box->otnames,
            PKSAV_GEN2_TEXT_TERMINATOR,
            sizeof(p_box->otnames)
        );
    }

    pokemon_pc_gen2impl::pokemon_pc_gen2impl(
        int game_id,
        struct pksav_gen2_pokemon_storage* p_native
    ): pokemon_pc_impl(game_id)
    {
        _our_mem = (p_native == nullptr);

        if(_our_mem)
        {
            _native = new struct pksav_gen2_pokemon_storage;

            for(size_t box_index = 0;
                box_index < PKSAV_GEN2_NUM_POKEMON_BOXES;
                ++box_index)
            {
                NATIVE_RCAST->pp_boxes[box_index] = new struct pksav_gen2_pokemon_box;
                _set_initial_pokemon_box_mem(NATIVE_RCAST->pp_boxes[box_index]);
            }

            NATIVE_RCAST->p_box_names = new struct pksav_gen2_pokemon_box_names;
            std::memset(
                NATIVE_RCAST->p_box_names,
                PKSAV_GEN2_TEXT_TERMINATOR,
                sizeof(*NATIVE_RCAST->p_box_names)
            );

            NATIVE_RCAST->p_current_box = new struct pksav_gen2_pokemon_box;
            _set_initial_pokemon_box_mem(NATIVE_RCAST->p_current_box);

            NATIVE_RCAST->p_current_box_num = new uint8_t(0);
        }
        else
        {
            _native = p_native;

#ifndef NDEBUG
            for(size_t box_index = 0;
                box_index < PKSAV_GEN2_NUM_POKEMON_BOXES;
                ++box_index)
            {
                BOOST_ASSERT(NATIVE_RCAST->pp_boxes[box_index] != nullptr);
            }

            BOOST_ASSERT(NATIVE_RCAST->p_box_names != nullptr);
            BOOST_ASSERT(NATIVE_RCAST->p_current_box != nullptr);
            BOOST_ASSERT(NATIVE_RCAST->p_current_box_num != nullptr);
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
                delete NATIVE_RCAST->pp_boxes[box_index];
            }

            delete NATIVE_RCAST->p_box_names;
            delete NATIVE_RCAST->p_current_box;
            delete NATIVE_RCAST->p_current_box_num;

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

        BOOST_ASSERT(*NATIVE_RCAST->p_current_box_num < PKSAV_GEN2_NUM_POKEMON_BOXES);

        // In Generation II, the current box is stored in a specific memory
        // location, so for that number box, use that pointer instead of
        // what's in the array.
        for(size_t box_index = 0;
            box_index < PKSAV_GEN2_NUM_POKEMON_BOXES;
            ++box_index)
        {
            if(box_index == *NATIVE_RCAST->p_current_box_num)
            {
                _box_list[box_index] = std::make_shared<pokemon_box_gen2impl>(
                                           _game_id,
                                           NATIVE_RCAST->p_current_box
                                       );
            }
            else
            {
                _box_list[box_index] = std::make_shared<pokemon_box_gen2impl>(
                                           _game_id,
                                           NATIVE_RCAST->pp_boxes[box_index]
                                       );
            }

            char box_name[PKSAV_GEN2_POKEMON_BOX_NAME_LENGTH + 1] = {0};
            PKSAV_CALL(
                pksav_gen2_import_text(
                    NATIVE_RCAST->p_box_names->names[box_index],
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
                    NATIVE_RCAST->p_box_names->names[box_index],
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

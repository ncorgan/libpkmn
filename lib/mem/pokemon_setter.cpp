/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "../pokemon_gen1impl.hpp"
#include "../pokemon_gen2impl.hpp"
#include "../pokemon_gbaimpl.hpp"

#include "../misc_common.hpp"
#include "pokemon_setter.hpp"
#include "scoped_lock.hpp"

#include <pkmn/exception.hpp>

#include <pksav/gen1/pokemon.h>
#include <pksav/gen2/pokemon.h>
#include <pksav/gba/pokemon.h>
#include <pksav/common/nds_pokemon.h>

#include <stdexcept>

#define RCAST_EQUAL(src_ptr,dst_ptr,type) \
    *reinterpret_cast<type*>(dst_ptr) = *reinterpret_cast<type*>(src_ptr);

#define RCAST_EQUAL_ALLOC(src_ptr,dst_ptr,type) { \
    dst_ptr = reinterpret_cast<void*>(new type); \
    *reinterpret_cast<type*>(dst_ptr) = *reinterpret_cast<type*>(src_ptr); \
}

namespace pkmn { namespace mem {

    void set_pokemon_in_box(
        pokemon_impl* new_pokemon,
        pokemon_box_impl* box,
        int index
    ) {
        pokemon_impl* box_pokemon = dynamic_cast<pokemon_impl*>(
                                        box->_pokemon_list.at(index).get()
                                    );

        // Make sure everything is ready
        pokemon_impl::pokemon_scoped_lock new_pokemon_lock(new_pokemon);
        pokemon_box_impl::pokemon_box_scoped_lock pokemon_box_lock(box);
        pokemon_impl::pokemon_scoped_lock box_pokemon_lock(box_pokemon);

        int game_id = new_pokemon->_database_entry.get_game_id();
        if(game_id != box->_game_id) {
            throw std::invalid_argument("The Pokémon and the box must be from the same game.");
        }

        void* box_ptr = box_pokemon->_native_pc;
        void* box_pc_copy = nullptr;
        pkmn::pokemon::sptr libpkmn_copy;

        switch(new_pokemon->_generation) {
            case 1:
                RCAST_EQUAL_ALLOC(box_ptr, box_pc_copy, pksav_gen1_pc_pokemon_t);
                RCAST_EQUAL(new_pokemon->_native_pc, box_ptr, pksav_gen1_pc_pokemon_t);
                box->_pokemon_list[index] = pkmn::make_shared<pokemon_gen1impl>(
                                                reinterpret_cast<pksav_gen1_pc_pokemon_t*>(box_ptr),
                                                game_id
                                            );
                box->_pokemon_list[index]->set_nickname(new_pokemon->get_nickname());
                box->_pokemon_list[index]->set_trainer_name(new_pokemon->get_trainer_name());
                break;

            case 2:
                RCAST_EQUAL_ALLOC(box_ptr, box_pc_copy, pksav_gen2_pc_pokemon_t);
                RCAST_EQUAL(new_pokemon->_native_pc, box_ptr, pksav_gen2_pc_pokemon_t);
                box->_pokemon_list[index] = pkmn::make_shared<pokemon_gen2impl>(
                                                reinterpret_cast<pksav_gen2_pc_pokemon_t*>(box_ptr),
                                                game_id
                                            );
                box->_pokemon_list[index]->set_nickname(new_pokemon->get_nickname());
                box->_pokemon_list[index]->set_trainer_name(new_pokemon->get_trainer_name());
                break;

            case 3:
                if(game_is_gamecube(box_pokemon->_database_entry.get_game_id())) {
                    throw pkmn::unimplemented_error();
                } else {
                    RCAST_EQUAL_ALLOC(box_ptr, box_pc_copy, pksav_gba_pc_pokemon_t);
                    RCAST_EQUAL(new_pokemon->_native_pc, box_ptr, pksav_gba_pc_pokemon_t);
                    box->_pokemon_list[index] = pkmn::make_shared<pokemon_gbaimpl>(
                                                    reinterpret_cast<pksav_gba_pc_pokemon_t*>(box_ptr),
                                                    game_id
                                                );
                }
                break;

            default:
                throw pkmn::unimplemented_error();
        }

        box_pokemon->_native_pc = box_pc_copy;
        box_pokemon->_our_pc_mem = true;

        new_pokemon->_our_pc_mem = false;
    }

}}

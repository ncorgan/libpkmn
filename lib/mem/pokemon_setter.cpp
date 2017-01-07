/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "../misc_common.hpp"
#include "pokemon_setter.hpp"
#include "scoped_lock.hpp"

#include <pkmn/exception.hpp>

#include <pksav/gen1/pokemon.h>
#include <pksav/gen2/pokemon.h>
#include <pksav/gba/pokemon.h>
#include <pksav/common/nds_pokemon.h>

#include <stdexcept>

namespace pkmn { namespace mem {

    /*
     * Copy the memory of the Pokémon currently in the box into a newly
     * allocated struct and set its internal memory to this new location.
     *
     * Copy the new Pokémon's memory into the box and set the Pokémon's
     * internal memory to this new location.
     *
     * At the end of this, both Pokémon instances will point to different
     * underlying memory, but their data should be the same. This entire
     * process should be entirely transparent to the user.
     */
    template<typename native_pc>
    static void _set_pokemon_in_box(
        native_pc** box_pokemon_ptr,
        native_pc** new_pc_pokemon_ptr
    ) {
        /*
         * PC Pokémon have no party data, so we know the box
         * Pokémon owns its own party data, so don't touch that,
         * just copy the PC data.
         */
        native_pc* box_pc = *box_pokemon_ptr;
        native_pc* new_box_pc = new native_pc;
        *new_box_pc = **box_pokemon_ptr;
        *box_pokemon_ptr = new_box_pc;

        *box_pc = **new_pc_pokemon_ptr;
        delete *new_pc_pokemon_ptr;
        *new_pc_pokemon_ptr = box_pc;
    }

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

        // We can't steal the new Pokémon from someone else's memory
        if(not (new_pokemon->_our_pc_mem and new_pokemon->_our_party_mem)) {
            throw std::runtime_error("This Pokémon's memory is owned by another party or box.");
        }

        if(new_pokemon->_database_entry.get_game_id() != box->_game_id) {
            throw std::invalid_argument("The Pokémon and the box must be from the same game.");
        }

        switch(new_pokemon->_generation) {
            case 1:
                _set_pokemon_in_box<pksav_gen1_pc_pokemon_t>(
                    reinterpret_cast<pksav_gen1_pc_pokemon_t**>(&(box_pokemon->_native_pc)),
                    reinterpret_cast<pksav_gen1_pc_pokemon_t**>(&(new_pokemon->_native_pc))
                );
                break;

            case 2:
                _set_pokemon_in_box<pksav_gen2_pc_pokemon_t>(
                    reinterpret_cast<pksav_gen2_pc_pokemon_t**>(&(box_pokemon->_native_pc)),
                    reinterpret_cast<pksav_gen2_pc_pokemon_t**>(&(new_pokemon->_native_pc))
                );
                break;

            case 3:
                if(game_is_gamecube(new_pokemon->_database_entry.get_game_id())) {
                    throw pkmn::unimplemented_error();
                } else {
                    _set_pokemon_in_box<pksav_gba_pc_pokemon_t>(
                        reinterpret_cast<pksav_gba_pc_pokemon_t**>(&(box_pokemon->_native_pc)),
                        reinterpret_cast<pksav_gba_pc_pokemon_t**>(&(new_pokemon->_native_pc))
                    );
                }
                break;

            case 4:
            case 5:
                _set_pokemon_in_box<pksav_nds_pc_pokemon_t>(
                    reinterpret_cast<pksav_nds_pc_pokemon_t**>(&(box_pokemon->_native_pc)),
                    reinterpret_cast<pksav_nds_pc_pokemon_t**>(&(new_pokemon->_native_pc))
                );
                break;

            case 6:
                throw pkmn::unimplemented_error();

            default:
                throw std::runtime_error("Invalid game.");
        }

        box_pokemon->_our_pc_mem = true;
        new_pokemon->_our_pc_mem = false;
    }

}}

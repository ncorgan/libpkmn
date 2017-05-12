/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_POKEMON_PARTY_GBAIMPL_IPP
#define PKMN_POKEMON_PARTY_GBAIMPL_IPP

#include "pokemon_gbaimpl.hpp"
#include "pokemon_party_gbaimpl.hpp"

#include <pksav/math/endian.h>

#include <pkmn/exception.hpp>

#include <cstring>
#include <iostream>
#include <stdexcept>

#define NATIVE_LIST_RCAST reinterpret_cast<pksav_gba_pokemon_party_t*>(_native)

namespace pkmn {

    pokemon_party_gbaimpl::pokemon_party_gbaimpl(
        int game_id
    ): pokemon_party_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new pksav_gba_pokemon_party_t);
        std::memset(_native, 0, sizeof(pksav_gba_pokemon_party_t));
        _our_mem = true;

        _from_native();
    }

    pokemon_party_gbaimpl::pokemon_party_gbaimpl(
        int game_id,
        pksav_gba_pokemon_party_t* native
    ): pokemon_party_impl(game_id)
    {
        _native = reinterpret_cast<void*>(native);
        _our_mem = false;

        _from_native();
    }

    pokemon_party_gbaimpl::~pokemon_party_gbaimpl() {
        if(_our_mem) {
            delete NATIVE_LIST_RCAST;
        }
    }

    int pokemon_party_gbaimpl::get_num_pokemon() {
        return int(pksav_littleendian32(NATIVE_LIST_RCAST->count));
    }

    void pokemon_party_gbaimpl::set_pokemon(
        int index,
        pkmn::pokemon::sptr new_pokemon
    ) {
        int num_pokemon = get_num_pokemon();
        int max_index = std::min<int>(PARTY_SIZE-1, num_pokemon);

        if(index < 0 or index > max_index) {
            pkmn::throw_out_of_range("index", 0, max_index);
        } else if(_pokemon_list.at(index)->get_native_pc_data() == new_pokemon->get_native_pc_data()) {
            throw std::invalid_argument("Cannot set a Pokémon to itself.");
        } else if(index < (num_pokemon-1) and new_pokemon->get_species() == "None") {
            throw std::invalid_argument("Parties store Pokémon contiguously.");
        }

        // Copy the underlying memory to the party.
        pkmn::mem::set_pokemon_in_party(
            dynamic_cast<pokemon_impl*>(new_pokemon.get()),
            this,
            index
        );

        // Update the number of Pokémon in the party if needed.
        std::string new_species = new_pokemon->get_species();
        if(index == num_pokemon) {
            std::string new_species = new_pokemon->get_species();
            if(pksav_littleendian16(NATIVE_LIST_RCAST->party[index].pc.blocks.growth.species) > 0 and new_species != "None") {
                NATIVE_LIST_RCAST->count = pksav_littleendian32(pksav_littleendian32(NATIVE_LIST_RCAST->count)+1);
            }
        } else if(index == (num_pokemon-1)) {
            if(pksav_littleendian16(NATIVE_LIST_RCAST->party[index].pc.blocks.growth.species) == 0 and new_species == "None") {
                NATIVE_LIST_RCAST->count = pksav_littleendian32(pksav_littleendian32(NATIVE_LIST_RCAST->count)-1);
            }
        }
    }

    void pokemon_party_gbaimpl::_from_native() {
        // This shouldn't resize if the vector is populated.
        _pokemon_list.resize(PARTY_SIZE);

        int num_pokemon = get_num_pokemon();

        for(int i = 0; i < PARTY_SIZE; ++i) {
            /*
             * Memory is not necessarily zeroed-out past the num_pokemon point,
             * so we'll do it ourselves.
             */
            if(i >= num_pokemon and pksav_littleendian16(NATIVE_LIST_RCAST->party[i].pc.blocks.growth.species) > 0) {
                std::memset(&NATIVE_LIST_RCAST->party[i], 0, sizeof(pksav_gba_party_pokemon_t));
            }

            _pokemon_list[i] = pkmn::make_shared<pokemon_gbaimpl>(
                                   &NATIVE_LIST_RCAST->party[i],
                                   _game_id
                               );
        }
    }
}

#endif /* PKMN_POKEMON_PARTY_GBAIMPL_IPP */

/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_gcnimpl.hpp"
#include "pokemon_party_gcnimpl.hpp"

#include <pksav/math/endian.h>

#include <pkmn/exception.hpp>

#include <cstring>
#include <iostream>
#include <stdexcept>

#define NATIVE_RCAST (reinterpret_cast<pkmn::gcn_pokemon_party_t*>(_native))

#define GC_POKEMON(i) (NATIVE_RCAST->pokemon[i])
#define COLO_POKEMON(i) (dynamic_cast<LibPkmGC::Colosseum::Pokemon*>(NATIVE_RCAST->pokemon[i]))
#define XD_POKEMON(i) (dynamic_cast<LibPkmGC::XD::Pokemon*>(NATIVE_RCAST->pokemon[i]))

namespace pkmn {

    pokemon_party_gcnimpl::pokemon_party_gcnimpl(
        int game_id
    ): pokemon_party_impl(game_id),
       _num_pokemon(0)
    {
        _native = reinterpret_cast<void*>(new pkmn::gcn_pokemon_party_t);

        // TODO: how does LibPKMN parse this?
        for(size_t i = 0; i < PARTY_SIZE; ++i) {
            if(_game_id == COLOSSEUM) {
                NATIVE_RCAST->pokemon[i] = new LibPkmGC::Colosseum::Pokemon;
            } else {
                NATIVE_RCAST->pokemon[i] = new LibPkmGC::XD::Pokemon;
            }
        }

        _our_mem = true;

        _from_native();
    }

    pokemon_party_gcnimpl::pokemon_party_gcnimpl(
        int game_id,
        LibPkmGC::GC::Pokemon** native
    ): pokemon_party_impl(game_id),
       _num_pokemon(0)
    {
        _native = reinterpret_cast<void*>(new pkmn::gcn_pokemon_party_t);
        for(size_t i = 0; i < PARTY_SIZE; ++i) {
            NATIVE_RCAST->pokemon[i] = native[i];
            if(NATIVE_RCAST->pokemon[i]->species > LibPkmGC::NoSpecies) {
                ++_num_pokemon;
            }
        }

        _our_mem = false;

        _from_native();
    }

    pokemon_party_gcnimpl::~pokemon_party_gcnimpl() {
        // _our_mem applies to the struct members, not the struct itself
        if(_our_mem) {
            for(size_t i = 0; i < PARTY_SIZE; ++i) {
                if(_game_id == COLOSSEUM) {
                    delete COLO_POKEMON(i);
                } else {
                    delete XD_POKEMON(i);
                }
            }
        }

        delete NATIVE_RCAST;
    }

    int pokemon_party_gcnimpl::get_num_pokemon() {
        return _num_pokemon;
    }

    void pokemon_party_gcnimpl::set_pokemon(
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
            if(NATIVE_RCAST->pokemon[index]->species > LibPkmGC::NoSpecies and new_species != "None") {
                ++_num_pokemon;
            }
        } else if(index == (num_pokemon-1)) {
            if(NATIVE_RCAST->pokemon[index]->species == LibPkmGC::NoSpecies and new_species == "None") {
                --_num_pokemon;
            }
        }
    }

    void pokemon_party_gcnimpl::_from_native() {
        // This shouldn't resize if the vector is populated.
        _pokemon_list.resize(PARTY_SIZE);

        for(int i = 0; i < PARTY_SIZE; ++i) {
            _pokemon_list[i] = pkmn::make_shared<pokemon_gcnimpl>(
                                   NATIVE_RCAST->pokemon[i],
                                   _game_id
                               );
        }
    }
}

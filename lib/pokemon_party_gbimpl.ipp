/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_POKEMON_PARTY_GBIMPL_IPP
#define PKMN_POKEMON_PARTY_GBIMPL_IPP

#include "pksav/pksav_call.hpp"

#include <pksav/gen1/text.h>
#include <pksav/gen2/text.h>

#include <cstring>
#include <stdexcept>
#include <type_traits>

#define NATIVE_LIST_RCAST reinterpret_cast<list_type*>(_native)
#define NUM_LIST_SPECIES ((sizeof(NATIVE_LIST_RCAST->species)/sizeof(NATIVE_LIST_RCAST->species[0]))-1)

namespace pkmn {

    template <typename list_type, typename pksav_pokemon_type, typename libpkmn_pokemon_type>
    pokemon_party_gbimpl<list_type, pksav_pokemon_type, libpkmn_pokemon_type>::pokemon_party_gbimpl(
        int game_id
    ): pokemon_party_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new list_type);
        std::memset(_native, 0, sizeof(list_type));
        NATIVE_LIST_RCAST->species[NUM_LIST_SPECIES] = 0xFF;
        _our_mem = true;

        _from_native();
    }

    template <typename list_type, typename pksav_pokemon_type, typename libpkmn_pokemon_type>
    pokemon_party_gbimpl<list_type, pksav_pokemon_type, libpkmn_pokemon_type>::pokemon_party_gbimpl(
        int game_id,
        list_type* native
    ): pokemon_party_impl(game_id)
    {
        _native = reinterpret_cast<void*>(native);
        _our_mem = false;

        _from_native();
    }

    template <typename list_type, typename pksav_pokemon_type, typename libpkmn_pokemon_type>
    pokemon_party_gbimpl<list_type, pksav_pokemon_type, libpkmn_pokemon_type>::~pokemon_party_gbimpl() {
        if(_our_mem) {
            delete NATIVE_LIST_RCAST;
        }
    }

    template <typename list_type, typename pksav_pokemon_type, typename libpkmn_pokemon_type>
    int pokemon_party_gbimpl<list_type, pksav_pokemon_type, libpkmn_pokemon_type>::get_num_pokemon() {
        return int(NATIVE_LIST_RCAST->count);
    }

    template <typename list_type, typename pksav_pokemon_type, typename libpkmn_pokemon_type>
    void pokemon_party_gbimpl<list_type, pksav_pokemon_type, libpkmn_pokemon_type>::set_pokemon(
        int index,
        pkmn::pokemon::sptr new_pokemon
    ) {
        int num_pokemon = get_num_pokemon();
        int max_index = std::min<int>(PARTY_SIZE-1, num_pokemon);

        if(index < 0 or index > max_index) {
            throw pkmn::range_error("index", 0, max_index);
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
            if(NATIVE_LIST_RCAST->species[index] == 0 and new_species != "None") {
                ++(NATIVE_LIST_RCAST->count);
            }
        } else if(index == (num_pokemon-1)) {
            if(NATIVE_LIST_RCAST->species[index] > 0 and new_species == "None") {
                --(NATIVE_LIST_RCAST->count);
            }
        }

        // Set the entry in the species list.
        NATIVE_LIST_RCAST->species[index] = uint8_t(new_pokemon->get_database_entry().get_pokemon_index());

        if(_generation == 1) {
            PKSAV_CALL(
                pksav_text_to_gen1(
                    new_pokemon->get_nickname().c_str(),
                    NATIVE_LIST_RCAST->nicknames[index],
                    10
                );
            )
            PKSAV_CALL(
                pksav_text_to_gen1(
                    new_pokemon->get_trainer_name().c_str(),
                    NATIVE_LIST_RCAST->otnames[index],
                    7
                );
            )
        } else {
            PKSAV_CALL(
                pksav_text_to_gen2(
                    new_pokemon->get_nickname().c_str(),
                    NATIVE_LIST_RCAST->nicknames[index],
                    10
                );
            )
            PKSAV_CALL(
                pksav_text_to_gen2(
                    new_pokemon->get_trainer_name().c_str(),
                    NATIVE_LIST_RCAST->otnames[index],
                    7
                );
            )
        }
    }

    template <typename list_type, typename pksav_pokemon_type, typename libpkmn_pokemon_type>
    void pokemon_party_gbimpl<list_type, pksav_pokemon_type, libpkmn_pokemon_type>::_from_native() {
        // This shouldn't resize if the vector is populated.
        _pokemon_list.resize(PARTY_SIZE);

        char nickname[11] = {0};
        char otname[8] = {0};

        for(int i = 0; i < PARTY_SIZE; ++i) {
            _pokemon_list[i] = pkmn::make_shared<libpkmn_pokemon_type>(
                                   &NATIVE_LIST_RCAST->party[i],
                                   _game_id
                               );

            PKSAV_CALL(
                pksav_text_from_gen1(
                    NATIVE_LIST_RCAST->nicknames[i],
                    nickname,
                    10
                );
            )
            if(std::strlen(nickname) > 0) {
                _pokemon_list[i]->set_nickname(nickname);
            }

            PKSAV_CALL(
                pksav_text_from_gen1(
                    NATIVE_LIST_RCAST->otnames[i],
                    otname,
                    7
                );
            )
            if(std::strlen(otname) > 0) {
                _pokemon_list[i]->set_trainer_name(otname);
            }
        }
    }
}

#endif /* PKMN_POKEMON_PARTY_GBIMPL_IPP */

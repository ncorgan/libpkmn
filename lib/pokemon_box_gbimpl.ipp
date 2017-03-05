/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_POKEMON_BOX_GBIMPL_IPP
#define PKMN_POKEMON_BOX_GBIMPL_IPP

#include "pksav/pksav_call.hpp"

#include <pksav/gen1/text.h>
#include <pksav/gen2/text.h>
#include <pksav/math/endian.h>

#include <cstring>
#include <stdexcept>
#include <type_traits>

#define NATIVE_LIST_RCAST reinterpret_cast<list_type*>(_native)
#define NUM_LIST_SPECIES ((sizeof(NATIVE_LIST_RCAST->species)/sizeof(NATIVE_LIST_RCAST->species[0]))-1)

namespace pkmn {

    template <typename list_type, typename pksav_pokemon_type, typename libpkmn_pokemon_type>
    pokemon_box_gbimpl<list_type, pksav_pokemon_type, libpkmn_pokemon_type>::pokemon_box_gbimpl(
        int game_id
    ): pokemon_box_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new list_type);
        std::memset(_native, 0, sizeof(list_type));
        NATIVE_LIST_RCAST->species[NUM_LIST_SPECIES] = 0xFF;
        _our_mem = true;

        _from_native();
    }

    template <typename list_type, typename pksav_pokemon_type, typename libpkmn_pokemon_type>
    pokemon_box_gbimpl<list_type, pksav_pokemon_type, libpkmn_pokemon_type>::pokemon_box_gbimpl(
        int game_id,
        list_type* native
    ): pokemon_box_impl(game_id)
    {
        _native = reinterpret_cast<void*>(native);
        _our_mem = false;

        _from_native();
    }

    template <typename list_type, typename pksav_pokemon_type, typename libpkmn_pokemon_type>
    pokemon_box_gbimpl<list_type, pksav_pokemon_type, libpkmn_pokemon_type>::~pokemon_box_gbimpl() {
        if(_our_mem) {
            delete NATIVE_LIST_RCAST;
        }
    }

    template <typename list_type, typename pksav_pokemon_type, typename libpkmn_pokemon_type>
    std::string pokemon_box_gbimpl<list_type, pksav_pokemon_type, libpkmn_pokemon_type>::get_name() {
        if(std::is_same<list_type, pksav_gen1_pokemon_box_t>::value) {
            throw pkmn::feature_not_in_game_error("Box names", "Generation I");
        } else {
            return _box_name;
        }
    }

    template <typename list_type, typename pksav_pokemon_type, typename libpkmn_pokemon_type>
    void pokemon_box_gbimpl<list_type, pksav_pokemon_type, libpkmn_pokemon_type>::set_name(
        const std::string &name
    ) {
        if(std::is_same<list_type, pksav_gen1_pokemon_box_t>::value) {
            throw pkmn::feature_not_in_game_error("Box names", "Generation I");
        } else {
            if(name.size() > 8) {
                throw std::invalid_argument("Generation II box names have a maximum length of 8.");
            }

            _box_name = name;
        }
    }

    template <typename list_type, typename pksav_pokemon_type, typename libpkmn_pokemon_type>
    int pokemon_box_gbimpl<list_type, pksav_pokemon_type, libpkmn_pokemon_type>::get_num_pokemon() {
        return int(NATIVE_LIST_RCAST->count);
    }

    template <typename list_type, typename pksav_pokemon_type, typename libpkmn_pokemon_type>
    int pokemon_box_gbimpl<list_type, pksav_pokemon_type, libpkmn_pokemon_type>::get_capacity() {
        return int(sizeof(NATIVE_LIST_RCAST->entries)/sizeof(NATIVE_LIST_RCAST->entries[0]));
    }

    template <typename list_type, typename pksav_pokemon_type, typename libpkmn_pokemon_type>
    void pokemon_box_gbimpl<list_type, pksav_pokemon_type, libpkmn_pokemon_type>::set_pokemon(
        int index,
        pkmn::pokemon::sptr new_pokemon
    ) {
        int capacity = get_capacity();
        int num_pokemon = get_num_pokemon();
        int max_index = std::min<int>(capacity-1, num_pokemon);

        if(index < 0 or index > max_index) {
            throw pkmn::range_error("index", 0, max_index);
        } else if(_pokemon_list.at(index)->get_native_pc_data() == new_pokemon->get_native_pc_data()) {
            throw std::invalid_argument("Cannot set a Pokémon to itself.");
        } else if(index < (num_pokemon-1) and new_pokemon->get_species() == "None") {
            throw std::invalid_argument("Generation I-II boxes store Pokémon contiguously.");
        }

        // Copy the underlying memory to the box.
        pkmn::mem::set_pokemon_in_box(
            dynamic_cast<pokemon_impl*>(new_pokemon.get()),
            this,
            index
        );

        // Update the number of Pokémon in the box if needed.
        std::string new_species = new_pokemon->get_species();
        if(index == num_pokemon and new_species != "None") {
            ++(NATIVE_LIST_RCAST->count);
        } else if(index == (num_pokemon-1) and new_species == "None") {
            --(NATIVE_LIST_RCAST->count);
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
    void pokemon_box_gbimpl<list_type, pksav_pokemon_type, libpkmn_pokemon_type>::_from_native() {
        int capacity = get_capacity();

        // This shouldn't resize if the vector is populated.
        _pokemon_list.resize(capacity);

        char nickname[11] = {0};
        char otname[8] = {0};

        int num_pokemon = get_num_pokemon();

        /*
         * Unfortuately, the count field may not be reliable, so we need to check
         * ourselves and fix it if it's wrong.
         */
        if(NATIVE_LIST_RCAST->entries[num_pokemon-1].species == 0) {
            for(int i = 0; i < num_pokemon; ++i) {
                if(NATIVE_LIST_RCAST->entries[i].species == 0) {
                    NATIVE_LIST_RCAST->count = i;
                    break;
                }
            }
        }

        for(int i = 0; i < capacity; ++i) {
            /*
             * Memory is not necessarily zeroed-out past the num_pokemon point,
             * so we'll do it ourselves.
             */
            if(i >= num_pokemon and NATIVE_LIST_RCAST->entries[i].species > 0) {
                NATIVE_LIST_RCAST->species[i] = 0;
                std::memset(&NATIVE_LIST_RCAST->entries[i], 0, sizeof(pksav_pokemon_type));
                std::memset(NATIVE_LIST_RCAST->nicknames[i], 0x50, sizeof(NATIVE_LIST_RCAST->nicknames[i]));
                std::memset(NATIVE_LIST_RCAST->otnames[i], 0x50, sizeof(NATIVE_LIST_RCAST->otnames[i]));
            }

            _pokemon_list[i] = pkmn::make_shared<libpkmn_pokemon_type>(
                                   &NATIVE_LIST_RCAST->entries[i],
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

#endif /* PKMN_POKEMON_BOX_GBIMPL_IPP */

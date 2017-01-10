/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_POKEMON_GB_LIST_IPP
#define PKMN_POKEMON_GB_LIST_IPP

#include "pokemon_gb_list.hpp"

#include "pksav/pksav_call.hpp"

#include <pksav/gen1/text.h>

#include <cstring>
#include <stdexcept>
#include <type_traits>

#define NATIVE_LIST_RCAST reinterpret_cast<list_type*>(_native)
#define NUM_LIST_SPECIES ((sizeof(NATIVE_LIST_RCAST->species)/sizeof(NATIVE_LIST_RCAST->species[0]))-1)

namespace pkmn {

    template <typename list_type, typename pksav_pokemon_type, typename libpkmn_pokemon_type>
    pokemon_gb_list<list_type, pksav_pokemon_type, libpkmn_pokemon_type>::pokemon_gb_list(
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
    pokemon_gb_list<list_type, pksav_pokemon_type, libpkmn_pokemon_type>::pokemon_gb_list(
        int game_id,
        list_type* native
    ): pokemon_box_impl(game_id)
    {
        _native = reinterpret_cast<void*>(native);
        _our_mem = false;

        _from_native();
    }

    template <typename list_type, typename pksav_pokemon_type, typename libpkmn_pokemon_type>
    pokemon_gb_list<list_type, pksav_pokemon_type, libpkmn_pokemon_type>::pokemon_gb_list(
        int game_id,
        const list_type &native
    ): pokemon_box_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new list_type);
        *NATIVE_LIST_RCAST = native;
        _our_mem = true;

        _from_native();
    }

    template <typename list_type, typename pksav_pokemon_type, typename libpkmn_pokemon_type>
    pokemon_gb_list<list_type, pksav_pokemon_type, libpkmn_pokemon_type>::~pokemon_gb_list() {
        if(_our_mem) {
            delete NATIVE_LIST_RCAST;
        }
    }

    template <typename list_type, typename pksav_pokemon_type, typename libpkmn_pokemon_type>
    std::string pokemon_gb_list<list_type, pksav_pokemon_type, libpkmn_pokemon_type>::get_name() {
        if(std::is_same<list_type, pksav_gen1_pokemon_box_t>::value) {
            throw pkmn::feature_not_in_game_error("Box names", "Generation I");
        } else {
            return _box_name;
        }
    }

    template <typename list_type, typename pksav_pokemon_type, typename libpkmn_pokemon_type>
    void pokemon_gb_list<list_type, pksav_pokemon_type, libpkmn_pokemon_type>::set_name(
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
    int pokemon_gb_list<list_type, pksav_pokemon_type, libpkmn_pokemon_type>::get_capacity() {
        return int(sizeof(NATIVE_LIST_RCAST->entries)/sizeof(NATIVE_LIST_RCAST->entries[0]));
    }

    template <typename list_type, typename pksav_pokemon_type, typename libpkmn_pokemon_type>
    void pokemon_gb_list<list_type, pksav_pokemon_type, libpkmn_pokemon_type>::_from_native() {
        int capacity = get_capacity();

        // This shouldn't resize if the vector is populated.
        _pokemon_list.resize(capacity);

        char nickname[11] = {0};
        char otname[8] = {0};

        for(int i = 0; i < capacity; ++i) {
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

#endif /* PKMN_POKEMON_GB_LIST_IPP */

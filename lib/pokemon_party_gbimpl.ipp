/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_POKEMON_PARTY_GBIMPL_IPP
#define PKMN_POKEMON_PARTY_GBIMPL_IPP

#include "exception_internal.hpp"
#include "pksav/pksav_call.hpp"

#include <pksav/gen1/text.h>
#include <pksav/gen2/text.h>

#include <cstring>
#include <stdexcept>

#define NATIVE_LIST_RCAST (reinterpret_cast<list_type*>(_native))

namespace pkmn {

    POKEMON_PARTY_GBIMPL_TEMPLATE
    POKEMON_PARTY_GBIMPL_CLASS::pokemon_party_gbimpl(
        int game_id
    ): pokemon_party_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new list_type);
        std::memset(_native, 0, sizeof(list_type));
        std::memset(NATIVE_LIST_RCAST->nicknames, 0x50, sizeof(NATIVE_LIST_RCAST->nicknames));
        std::memset(NATIVE_LIST_RCAST->otnames, 0x50, sizeof(NATIVE_LIST_RCAST->otnames));
        NATIVE_LIST_RCAST->species[6] = 0xFF;
        _our_mem = true;

        _from_native();
    }

    POKEMON_PARTY_GBIMPL_TEMPLATE
    POKEMON_PARTY_GBIMPL_CLASS::pokemon_party_gbimpl(
        int game_id,
        list_type* native
    ): pokemon_party_impl(game_id)
    {
        _native = reinterpret_cast<void*>(native);
        _our_mem = false;

        _from_native();
    }

    POKEMON_PARTY_GBIMPL_TEMPLATE
    POKEMON_PARTY_GBIMPL_CLASS::~pokemon_party_gbimpl() {
        if(_our_mem) {
            delete NATIVE_LIST_RCAST;
        }
    }

    POKEMON_PARTY_GBIMPL_TEMPLATE
    int POKEMON_PARTY_GBIMPL_CLASS::get_num_pokemon() {
        return int(NATIVE_LIST_RCAST->count);
    }

    POKEMON_PARTY_GBIMPL_TEMPLATE
    void POKEMON_PARTY_GBIMPL_CLASS::set_pokemon(
        int index,
        pkmn::pokemon::sptr new_pokemon
    ) {
        int num_pokemon = get_num_pokemon();
        int max_index = std::min<int>(PARTY_SIZE-1, num_pokemon);

        pkmn::enforce_bounds("Party index", index, 0, max_index);

        if(_pokemon_list.at(index)->get_native_pc_data() == new_pokemon->get_native_pc_data())
        {
            throw std::invalid_argument("Cannot set a Pokémon to itself.");
        }
        else if(index < (num_pokemon-1) and new_pokemon->get_species() == "None")
        {
            throw std::invalid_argument("Parties store Pokémon contiguously.");
        }
        else if(_game_id != new_pokemon->get_database_entry().get_game_id())
        {
            throw std::invalid_argument("The given Pokémon must be from the same game as the party.");
        }

        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        pokemon_impl* new_pokemon_impl_ptr = dynamic_cast<pokemon_impl*>(new_pokemon.get());
        pokemon_impl* old_party_pokemon_impl_ptr = dynamic_cast<pokemon_impl*>(_pokemon_list[index].get());

        // Make sure no one else is using the Pokémon variables.
        boost::mutex::scoped_lock new_pokemon_lock(new_pokemon_impl_ptr->_mem_mutex);
        old_party_pokemon_impl_ptr->_mem_mutex.lock();

        // Copy the underlying memory to the party. At the end of this process,
        // all existing variables will correspond to the same Pokémon, even if
        // their underlying memory has changed.

        // Make a copy of the current Pokémon in the given party slot so it can be preserved in an sptr
        // that owns its own memory.
        copy_party_pokemon<pksav_pc_pokemon_type, pksav_pokemon_party_data_type>(index);

        // Copy the new Pokémon's internals into the party's internals and create a new sptr.
        void* new_pokemon_native_pc_ptr = new_pokemon_impl_ptr->_native_pc;
        void* new_pokemon_native_party_ptr = new_pokemon_impl_ptr->_native_party;

        // Unlock the old Pokémon's mutex is unlocked before it's destructor is called.
        old_party_pokemon_impl_ptr->_mem_mutex.unlock();

        // Set the entry in the species list.
        NATIVE_LIST_RCAST->party[index].pc = *reinterpret_cast<pksav_pc_pokemon_type*>(new_pokemon_native_pc_ptr);
        NATIVE_LIST_RCAST->party[index].party_data = *reinterpret_cast<pksav_pokemon_party_data_type*>(new_pokemon_native_party_ptr);
        _pokemon_list[index] = pkmn::make_shared<libpkmn_pokemon_type>(
                                   &NATIVE_LIST_RCAST->party[index],
                                   _game_id
                               );

        // Don't set empty names.
        std::string new_pokemon_nickname = new_pokemon->get_nickname();
        std::string new_pokemon_trainer_name = new_pokemon->get_trainer_name();
        if(new_pokemon_nickname.size() == 0)
        {
            new_pokemon_nickname = "None";
        }
        if(new_pokemon_trainer_name.size() == 0)
        {
            new_pokemon_trainer_name = "None";
        }

        _pokemon_list[index]->set_nickname(new_pokemon_nickname);
        _pokemon_list[index]->set_trainer_name(new_pokemon_trainer_name);

        NATIVE_LIST_RCAST->species[index] = uint8_t(new_pokemon->get_database_entry().get_pokemon_index());

        // Update the number of Pokémon in the party if needed.
        std::string new_species = new_pokemon->get_species();
        if(index == num_pokemon and new_species != "None") {
            ++(NATIVE_LIST_RCAST->count);
        } else if(index == (num_pokemon-1) and new_species == "None") {
            --(NATIVE_LIST_RCAST->count);
        }

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

    POKEMON_PARTY_GBIMPL_TEMPLATE
    void POKEMON_PARTY_GBIMPL_CLASS::_from_native() {
        // This shouldn't resize if the vector is populated.
        _pokemon_list.resize(PARTY_SIZE);

        char nickname[11] = {0};
        char otname[8] = {0};

        int num_pokemon = get_num_pokemon();

        for(int i = 0; i < PARTY_SIZE; ++i) {
            /*
             * Memory is not necessarily zeroed-out past the num_pokemon point,
             * so we'll do it ourselves.
             */
            if(i >= num_pokemon and NATIVE_LIST_RCAST->party[i].pc.species > 0) {
                NATIVE_LIST_RCAST->species[i] = 0;
                std::memset(&NATIVE_LIST_RCAST->party[i], 0, sizeof(pksav_party_pokemon_type));
                std::memset(NATIVE_LIST_RCAST->nicknames[i], 0x50, sizeof(NATIVE_LIST_RCAST->nicknames[i]));
                std::memset(NATIVE_LIST_RCAST->otnames[i], 0x50, sizeof(NATIVE_LIST_RCAST->otnames[i]));
            }

            _pokemon_list[i] = pkmn::make_shared<libpkmn_pokemon_type>(
                                   &NATIVE_LIST_RCAST->party[i],
                                   _game_id
                               );

            if(_generation == 1) {
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
            } else {
                PKSAV_CALL(
                    pksav_text_from_gen2(
                        NATIVE_LIST_RCAST->nicknames[i],
                        nickname,
                        10
                    );
                )
                if(std::strlen(nickname) > 0) {
                    _pokemon_list[i]->set_nickname(nickname);
                }

                PKSAV_CALL(
                    pksav_text_from_gen2(
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
}

#endif /* PKMN_POKEMON_PARTY_GBIMPL_IPP */

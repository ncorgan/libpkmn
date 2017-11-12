/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_gcnimpl.hpp"
#include "pokemon_party_gcnimpl.hpp"

#include <pkmn/exception.hpp>

#include <boost/thread/lock_guard.hpp>

#include <cstring>
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

        if(index < 0 or index > max_index)
        {
            pkmn::throw_out_of_range("index", 0, max_index);
        }
        else if(_pokemon_list.at(index)->get_native_pc_data() == new_pokemon->get_native_pc_data())
        {
            throw std::invalid_argument("Cannot set a Pokémon to itself.");
        }
        else if(index < (num_pokemon-1) and new_pokemon->get_species() == "None")
        {
            throw std::invalid_argument("Parties store Pokémon contiguously.");
        }

        boost::mutex::scoped_lock(_mem_mutex);

        // If the given Pokémon isn't from this party's game, convert it if we can.
        pkmn::pokemon::sptr actual_new_pokemon;
        if(_game_id == new_pokemon->get_database_entry().get_game_id())
        {
            actual_new_pokemon = new_pokemon;
        }
        else
        {
            actual_new_pokemon = new_pokemon->to_game(get_game());
        }

        pokemon_impl* new_pokemon_impl_ptr = dynamic_cast<pokemon_impl*>(actual_new_pokemon.get());
        pokemon_impl* old_party_pokemon_impl_ptr = dynamic_cast<pokemon_impl*>(_pokemon_list[index].get());

        // Make sure no one else is using the Pokémon variables.
        boost::lock_guard<pokemon_impl> new_pokemon_lock(*new_pokemon_impl_ptr);
        old_party_pokemon_impl_ptr->lock();

        // Copy the underlying memory to the party. At the end of this process,
        // all existing variables will correspond to the same Pokémon, even if
        // their underlying memory has changed.

        void* old_party_pokemon_native_ptr = old_party_pokemon_impl_ptr->_native_pc;
        void* new_pokemon_native_ptr = new_pokemon_impl_ptr->_native_pc;

        // Make a copy of the current Pokémon in the given party slot so it can be preserved in an sptr
        // that owns its own memory.
        LibPkmGC::GC::Pokemon* party_pokemon_copy_ptr = reinterpret_cast<LibPkmGC::GC::Pokemon*>(
                                                            old_party_pokemon_impl_ptr->_native_pc
                                                        )->clone();

        if(_game_id == COLOSSEUM)
        {
            rcast_equal<LibPkmGC::Colosseum::Pokemon>(
                old_party_pokemon_native_ptr,
                dynamic_cast<LibPkmGC::Colosseum::Pokemon*>(party_pokemon_copy_ptr)
            );
            rcast_equal<LibPkmGC::Colosseum::Pokemon>(
                new_pokemon_native_ptr,
                old_party_pokemon_native_ptr
            );
        }
        else
        {
            rcast_equal<LibPkmGC::XD::Pokemon>(
                old_party_pokemon_native_ptr,
                dynamic_cast<LibPkmGC::XD::Pokemon*>(party_pokemon_copy_ptr)
            );
            rcast_equal<LibPkmGC::XD::Pokemon>(
                new_pokemon_native_ptr,
                old_party_pokemon_native_ptr
            );
        }

        old_party_pokemon_impl_ptr->_native_pc = reinterpret_cast<void*>(party_pokemon_copy_ptr);
        old_party_pokemon_impl_ptr->_our_pc_mem = true;

        // Unlock the old Pokémon's mutex is unlocked before it's destructor is called.
        old_party_pokemon_impl_ptr->unlock();

        _pokemon_list[index] = pkmn::make_shared<pokemon_gcnimpl>(
                                   NATIVE_RCAST->pokemon[index],
                                   _game_id
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

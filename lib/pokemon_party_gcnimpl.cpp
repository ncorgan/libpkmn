/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "pokemon_gcnimpl.hpp"
#include "pokemon_party_gcnimpl.hpp"

#include <pkmn/exception.hpp>

#include <boost/thread/lock_guard.hpp>

#include <cstring>
#include <stdexcept>

namespace pkmn {

    pokemon_party_gcnimpl::pokemon_party_gcnimpl(
        int game_id,
        LibPkmGC::GC::Pokemon** pp_libpkmgc_native
    ): pokemon_party_impl(game_id),
       _num_pokemon(0)
    {
        _libpkmgc_pokemon_uptrs.resize(PARTY_SIZE);

        if(pp_libpkmgc_native != nullptr)
        {
            for(size_t party_index = 0; party_index < PARTY_SIZE; ++party_index)
            {
                _libpkmgc_pokemon_uptrs[party_index].reset(
                    pp_libpkmgc_native[party_index]->clone()
                );
            }
        }
        else
        {
            for(size_t party_index = 0; party_index < PARTY_SIZE; ++party_index)
            {
                if(game_id == COLOSSEUM_ID)
                {
                    _libpkmgc_pokemon_uptrs[party_index].reset(
                        new LibPkmGC::Colosseum::Pokemon
                    );
                }
                else
                {
                    _libpkmgc_pokemon_uptrs[party_index].reset(
                        new LibPkmGC::XD::Pokemon
                    );
                }

            }
        }

        _from_native();
    }

    int pokemon_party_gcnimpl::get_num_pokemon()
    {
        boost::lock_guard<pokemon_party_gcnimpl> lock(*this);

        return _num_pokemon;
    }

    void pokemon_party_gcnimpl::set_pokemon(
        int index,
        const pkmn::pokemon::sptr& new_pokemon
    )
    {
        int num_pokemon = get_num_pokemon();
        int max_index = std::min<int>(PARTY_SIZE-1, num_pokemon);

        pkmn::enforce_bounds("Party index", index, 0, max_index);

        if(index < (num_pokemon-1) and new_pokemon->get_species() == "None")
        {
            throw std::invalid_argument("Parties store Pokémon contiguously.");
        }

        boost::lock_guard<pokemon_party_gcnimpl> lock(*this);

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

        if(_game_id == COLOSSEUM_ID)
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

        _pokemon_list[index] = std::make_shared<pokemon_gcnimpl>(
                                   _libpkmgc_pokemon_uptrs[index].get(),
                                   _game_id
                               );

        // Update the number of Pokémon in the party if needed.
        std::string new_species = new_pokemon->get_species();
        if(index == num_pokemon)
        {
            if((_libpkmgc_pokemon_uptrs[index]->species > LibPkmGC::NoSpecies) && (new_species != "None"))
            {
                ++_num_pokemon;
            }
        }
        else if(index == (num_pokemon-1))
        {
            if((_libpkmgc_pokemon_uptrs[index]->species == LibPkmGC::NoSpecies) && (new_species == "None"))
            {
                --_num_pokemon;
            }
        }
    }

    void pokemon_party_gcnimpl::_from_native()
    {
        // This shouldn't resize if the vector is populated.
        _pokemon_list.resize(PARTY_SIZE);

        for(int party_index = 0; party_index < PARTY_SIZE; ++party_index)
        {
            _pokemon_list[party_index] = std::make_shared<pokemon_gcnimpl>(
                                            _libpkmgc_pokemon_uptrs[party_index].get(),
                                            _game_id
                                        );
        }
    }
}

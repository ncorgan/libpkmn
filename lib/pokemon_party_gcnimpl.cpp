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
        LibPkmGC::GC::Pokemon* const* pp_libpkmgc_native
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

        _p_native = _libpkmgc_pokemon_uptrs.data();

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

        if((index < (num_pokemon-1)) && (new_pokemon->get_species() == "None"))
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

        // Make sure no one else is using the new Pokémon variable.
        pokemon_gcnimpl* p_new_pokemon = dynamic_cast<pokemon_gcnimpl*>(
                                             actual_new_pokemon.get()
                                         );
        BOOST_ASSERT(p_new_pokemon != nullptr);
        boost::lock_guard<pokemon_gcnimpl> new_pokemon_lock(*p_new_pokemon);

        // Copy the underlying memory to the party. At the end of this process,
        // all existing variables will correspond to the same Pokémon, even if
        // their underlying memory has changed.
        //
        // Note: as we control the implementation, we know the PC data points
        // to the whole Pokémon data structure.
        _libpkmgc_pokemon_uptrs[index].reset(
            static_cast<LibPkmGC::GC::Pokemon*>(
                actual_new_pokemon->get_native_pc_data()
            )->clone()
        );
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

        _num_pokemon = 0;

        for(int party_index = 0; party_index < PARTY_SIZE; ++party_index)
        {
            _pokemon_list[party_index] = std::make_shared<pokemon_gcnimpl>(
                                            _libpkmgc_pokemon_uptrs[party_index].get(),
                                            _game_id
                                        );
            if(_pokemon_list[party_index]->get_species() != "None")
            {
                ++_num_pokemon;
            }
        }
    }

    void pokemon_party_gcnimpl::_to_native()
    {
        BOOST_ASSERT(_pokemon_list.size() == PARTY_SIZE);

        for(int party_index = 0;
            party_index < PARTY_SIZE;
            ++party_index)
        {
            _libpkmgc_pokemon_uptrs[party_index].reset(
                static_cast<LibPkmGC::GC::Pokemon*>(
                    _pokemon_list[party_index]->get_native_pc_data()
                )->clone()
            );
        }
    }
}

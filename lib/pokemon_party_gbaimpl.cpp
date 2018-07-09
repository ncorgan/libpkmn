/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "pokemon_gbaimpl.hpp"
#include "pokemon_party_gbaimpl.hpp"
#include "utils/misc.hpp"

#include <pkmn/exception.hpp>

#include <pksav/gba/pokemon.h>
#include <pksav/math/endian.h>

#include <boost/thread/lock_guard.hpp>

#include <cstring>
#include <stdexcept>

namespace pkmn {

    pokemon_party_gbaimpl::pokemon_party_gbaimpl(
        int game_id,
        struct pksav_gba_pokemon_party* p_native
    ): pokemon_party_impl(game_id)
    {
        if(p_native != nullptr)
        {
            _pksav_party = *p_native;
        }
        else
        {
            std::memset(
                &_pksav_party,
                0,
                sizeof(_pksav_party)
            );
        }

        _p_native = &_pksav_party;

        _from_native();
    }

    int pokemon_party_gbaimpl::get_num_pokemon()
    {
        boost::lock_guard<pokemon_party_gbaimpl> lock(*this);

        return int(pksav_littleendian32(_pksav_party.count));
    }

    void pokemon_party_gbaimpl::set_pokemon(
        int index,
        const pkmn::pokemon::sptr& new_pokemon
    )
    {
        int num_pokemon = get_num_pokemon();
        int max_index = std::min<int>(PKSAV_GBA_PARTY_NUM_POKEMON-1, num_pokemon);

        pkmn::enforce_bounds("Party index", index, 0, max_index);

        if(index < (num_pokemon-1) and new_pokemon->get_species() == "None")
        {
            throw std::invalid_argument("Parties store Pokémon contiguously.");
        }

        boost::lock_guard<pokemon_party_gbaimpl> lock(*this);

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
        pokemon_gbaimpl* p_new_pokemon = dynamic_cast<pokemon_gbaimpl*>(
                                             actual_new_pokemon.get()
                                         );
        BOOST_ASSERT(p_new_pokemon != nullptr);
        boost::lock_guard<pokemon_gbaimpl> new_pokemon_lock(*p_new_pokemon);

        // Copy the underlying memory to the party. At the end of this process,
        // all existing variables will correspond to the same Pokémon, even if
        // their underlying memory has changed.
        //
        // Note: as we control the implementation, we know the PC data points
        // to the whole Pokémon data structure.
        rcast_equal<struct pksav_gba_party_pokemon>(
            actual_new_pokemon->get_native_pc_data(),
            &_pksav_party.party[index]
        );
        _pokemon_list[index] = std::make_shared<pokemon_gbaimpl>(
                                   &_pksav_party.party[index],
                                   _game_id
                               );

        // Update the number of Pokémon in the party if needed.
        std::string new_species = new_pokemon->get_species();
        if(index == num_pokemon)
        {
            if((pksav_littleendian16(_pksav_party.party[index].pc_data.blocks.growth.species) > 0) && (new_species != "None"))
            {
                _pksav_party.count = pksav_littleendian32(_pksav_party.count)+1;
            }
        }
        else if(index == (num_pokemon-1))
        {
            if((pksav_littleendian16(_pksav_party.party[index].pc_data.blocks.growth.species) == 0) && (new_species == "None"))
            {
                _pksav_party.count = pksav_littleendian32(_pksav_party.count)-1;
            }
        }

        // If this party is part of a save, set the Pokédex to have both
        // seen and caught the Pokémon.
        if(_pokedex.get())
        {
            std::string species = new_pokemon->get_species();

            if((species != "None") && !new_pokemon->is_egg())
            {
                _pokedex->set_has_seen(species, true);
                _pokedex->set_has_caught(species, true);
            }
        }
    }

    void pokemon_party_gbaimpl::_from_native()
    {
        // This shouldn't resize if the vector is populated.
        _pokemon_list.resize(PKSAV_GBA_PARTY_NUM_POKEMON);

        int num_pokemon = get_num_pokemon();

        for(int i = 0; i < PKSAV_GBA_PARTY_NUM_POKEMON; ++i)
        {
            /*
             * Memory is not necessarily zeroed-out past the num_pokemon point,
             * so we'll do it ourselves.
             */
            if(i >= num_pokemon and pksav_littleendian16(_pksav_party.party[i].pc_data.blocks.growth.species) > 0)
            {
                std::memset(&_pksav_party.party[i], 0, sizeof(struct pksav_gba_party_pokemon));
            }

            _pokemon_list[i] = std::make_shared<pokemon_gbaimpl>(
                                   &_pksav_party.party[i],
                                   _game_id
                               );
        }
    }

    void pokemon_party_gbaimpl::_to_native()
    {
        BOOST_ASSERT(_pokemon_list.size() == PKSAV_GBA_PARTY_NUM_POKEMON);

        for(size_t party_index = 0;
            party_index < _pokemon_list.size();
            ++party_index)
        {
            // As we control the implementation, we know the PC data pointer
            // points to the whole structure.
            pkmn::rcast_equal<struct pksav_gba_party_pokemon>(
                _pokemon_list[party_index]->get_native_pc_data(),
                &_pksav_party.party[party_index]
            );
        }
    }
}

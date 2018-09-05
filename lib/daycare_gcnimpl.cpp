/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "daycare_gcnimpl.hpp"
#include "pokemon_gcnimpl.hpp"

#include "exception_internal.hpp"

#include "pksav/pksav_call.hpp"

#include <LibPkmGC/Colosseum/Common/DaycareData.h>
#include <LibPkmGC/GC/Common/DaycareData.h>
#include <LibPkmGC/XD/Common/DaycareData.h>

#include <boost/assert.hpp>
#include <boost/thread/lock_guard.hpp>

#include <string.h>

#define GC_RCAST(ptr)   (reinterpret_cast<LibPkmGC::GC::DaycareData*>(ptr))
#define COLO_RCAST(ptr) (reinterpret_cast<LibPkmGC::Colosseum::DaycareData*>(ptr))
#define XD_RCAST(ptr)   (reinterpret_cast<LibPkmGC::XD::DaycareData*>(ptr))

namespace pkmn {

    daycare_gcnimpl::daycare_gcnimpl(
        int game_id,
        void* p_native
    ): daycare_impl(game_id)
    {
        if(p_native == nullptr)
        {
            if(_game_id == COLOSSEUM_ID)
            {
                _p_native = new LibPkmGC::Colosseum::DaycareData;
            }
            else
            {
                _p_native = new LibPkmGC::XD::DaycareData;
            }

            _is_our_mem = true;
        }
        else
        {
            _p_native = p_native;
            _is_our_mem = false;
        }

        this->_from_native_levelup();
    }

    daycare_gcnimpl::~daycare_gcnimpl()
    {
        if(_is_our_mem)
        {
            if(_game_id == COLOSSEUM_ID)
            {
                delete COLO_RCAST(_p_native);
            }
            else
            {
                delete XD_RCAST(_p_native);
            }
        }
    }

    int daycare_gcnimpl::get_levelup_pokemon_capacity()
    {
        return LEVELUP_CAPACITY;
    }

    bool daycare_gcnimpl::can_breed_pokemon()
    {
        return CAN_BREED_POKEMON;
    }

    int daycare_gcnimpl::get_breeding_pokemon_capacity()
    {
        return BREEDING_CAPACITY;
    }

    void daycare_gcnimpl::_set_levelup_pokemon(
        int position,
        const pkmn::pokemon::sptr& new_pokemon
    )
    {
        pkmn::enforce_comparator(
            "Position",
            position,
            0,
            value_comparator::EQ
        );

        boost::lock_guard<daycare_gcnimpl> lock(*this);

        pkmn::pokemon_list_t& r_levelup_pokemon = this->_get_levelup_pokemon_ref();

        // Make sure no one else is using the new Pokémon variable.
        pokemon_gcnimpl* p_new_pokemon = dynamic_cast<pokemon_gcnimpl*>(
                                             new_pokemon.get()
                                         );
        BOOST_ASSERT(p_new_pokemon != nullptr);
        boost::lock_guard<pokemon_gcnimpl> new_pokemon_lock(*p_new_pokemon);

        // Copy the underlying memory to the party. At the end of this process,
        // all existing variables will correspond to the same Pokémon, even if
        // their underlying memory has changed.
        //
        // Note: as we control the implementation, we know the PC data points
        // to the whole Pokémon data structure.
        if(_game_id == COLOSSEUM_ID)
        {
            delete dynamic_cast<LibPkmGC::Colosseum::Pokemon*>(
                       GC_RCAST(_p_native)->pkm
                   );
        }
        else
        {
            delete dynamic_cast<LibPkmGC::XD::Pokemon*>(
                       GC_RCAST(_p_native)->pkm
                   );
        }

        GC_RCAST(_p_native)->pkm =
            static_cast<const LibPkmGC::GC::Pokemon*>(
                new_pokemon->get_native_pc_data()
            )->clone();
        r_levelup_pokemon[0] = std::make_shared<pokemon_gcnimpl>(
                                   dynamic_cast<LibPkmGC::GC::Pokemon*>(
                                       GC_RCAST(_p_native)->pkm
                                   ),
                                   _game_id
                               );
    }

    void daycare_gcnimpl::_set_breeding_pokemon(
        int,
        const pkmn::pokemon::sptr&
    )
    {
        throw pkmn::feature_not_in_game_error(
                  "Breeding",
                  this->get_game()
              );
    }

    void daycare_gcnimpl::_from_native_levelup()
    {
        pkmn::pokemon_list_t& r_levelup_pokemon = this->_get_levelup_pokemon_ref();

        r_levelup_pokemon.resize(LEVELUP_CAPACITY);
        r_levelup_pokemon[0] = std::make_shared<pokemon_gcnimpl>(
                                   GC_RCAST(_p_native)->pkm,
                                   _game_id
                               );
    }

    void daycare_gcnimpl::_to_native_levelup() {}
    void daycare_gcnimpl::_from_native_breeding() {}
    void daycare_gcnimpl::_to_native_breeding() {}
}

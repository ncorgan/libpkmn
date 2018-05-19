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

        _can_breed = false;

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

    void daycare_gcnimpl::set_levelup_pokemon(
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

        // If the given Pokémon isn't from this box's game, convert it if we can.
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
        pokemon_impl* old_box_pokemon_impl_ptr = dynamic_cast<pokemon_impl*>(r_levelup_pokemon[0].get());

        // Make sure no one else is using the Pokémon variables.
        boost::lock_guard<pokemon_impl> new_pokemon_lock(*new_pokemon_impl_ptr);
        old_box_pokemon_impl_ptr->lock();

        // Copy the underlying memory to the party. At the end of this process,
        // all existing variables will correspond to the same Pokémon, even if
        // their underlying memory has changed.

        void* old_box_pokemon_native_ptr = old_box_pokemon_impl_ptr->_native_pc;
        void* new_pokemon_native_ptr = new_pokemon_impl_ptr->_native_pc;

        // Make a copy of the current Pokémon in the given party slot so it can be preserved in an sptr
        // that owns its own memory.
        LibPkmGC::GC::Pokemon* box_pokemon_copy_ptr = reinterpret_cast<LibPkmGC::GC::Pokemon*>(
                                                          old_box_pokemon_impl_ptr->_native_pc
                                                      )->clone();

        if(_game_id == COLOSSEUM_ID)
        {
            rcast_equal<LibPkmGC::Colosseum::Pokemon>(
                old_box_pokemon_native_ptr,
                dynamic_cast<LibPkmGC::Colosseum::Pokemon*>(box_pokemon_copy_ptr)
            );
            rcast_equal<LibPkmGC::Colosseum::Pokemon>(
                new_pokemon_native_ptr,
                old_box_pokemon_native_ptr
            );
        }
        else
        {
            rcast_equal<LibPkmGC::XD::Pokemon>(
                old_box_pokemon_native_ptr,
                dynamic_cast<LibPkmGC::XD::Pokemon*>(box_pokemon_copy_ptr)
            );
            rcast_equal<LibPkmGC::XD::Pokemon>(
                new_pokemon_native_ptr,
                old_box_pokemon_native_ptr
            );
        }

        old_box_pokemon_impl_ptr->_native_pc = reinterpret_cast<void*>(box_pokemon_copy_ptr);
        old_box_pokemon_impl_ptr->_our_pc_mem = true;

        // Unlock the old Pokémon's mutex is unlocked before it's destructor is called.
        old_box_pokemon_impl_ptr->unlock();

        r_levelup_pokemon[0] = std::make_shared<pokemon_gcnimpl>(
                                   dynamic_cast<LibPkmGC::GC::Pokemon*>(GC_RCAST(_p_native)),
                                   _game_id
                               );
    }

    int daycare_gcnimpl::get_levelup_pokemon_capacity()
    {
        return LEVELUP_CAPACITY;
    }

    void daycare_gcnimpl::set_breeding_pokemon(
        int,
        const pkmn::pokemon::sptr&
    )
    {
        throw pkmn::feature_not_in_game_error(
                  "Breeding",
                  this->get_game()
              );
    }

    int daycare_gcnimpl::get_breeding_pokemon_capacity()
    {
        return BREEDING_CAPACITY;
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

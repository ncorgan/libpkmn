/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "daycare_gen1impl.hpp"
#include "pokemon_gen1impl.hpp"

#include "exception_internal.hpp"

#include "pksav/pksav_call.hpp"

#include <pksav/gen1/daycare_data.h>
#include <pksav/gen1/pokemon.h>
#include <pksav/gen1/text.h>

#include <boost/assert.hpp>
#include <boost/thread/lock_guard.hpp>

#include <string.h>

#define NATIVE_RCAST(ptr) (reinterpret_cast<struct pksav_gen1_daycare_data*>(ptr))

namespace pkmn {

    daycare_gen1impl::daycare_gen1impl(
        int game_id,
        void* p_native
    ): daycare_impl(game_id)
    {
        if(p_native == nullptr)
        {
            _p_native = new struct pksav_gen1_daycare_data;

            // This will cause _from_native_levelup() to zero out the memory.
            NATIVE_RCAST(_p_native)->is_daycare_in_use = PKSAV_GEN1_DAYCARE_NOT_IN_USE;

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

    daycare_gen1impl::~daycare_gen1impl()
    {
        if(_is_our_mem)
        {
            delete NATIVE_RCAST(_p_native);
        }
    }

    void daycare_gen1impl::set_levelup_pokemon(
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

        boost::lock_guard<daycare_gen1impl> lock(*this);

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
        pokemon_impl* old_box_pokemon_impl_ptr = dynamic_cast<pokemon_impl*>(r_levelup_pokemon[position].get());

        // Make sure no one else is using the Pokémon variables.
        boost::lock_guard<pokemon_impl> new_pokemon_lock(*new_pokemon_impl_ptr);
        old_box_pokemon_impl_ptr->lock();

        // Copy the underlying memory to the box. At the end of this process,
        // all existing variables will correspond to the same Pokémon, even if
        // their underlying memory has changed.

        // Make a copy of the current Pokémon in the given box slot so it can be preserved in an sptr
        // that owns its own memory.
        copy_box_pokemon<struct pksav_gen1_pc_pokemon, struct pksav_gen1_pokemon_party_data>(
            r_levelup_pokemon,
            position
        );

        // Copy the new Pokémon's internals into the box's internals and create a new sptr.
        void* new_pokemon_native_pc_ptr = new_pokemon_impl_ptr->_native_pc;

        // Unlock the old Pokémon's mutex is unlocked before it's destructor is called.
        old_box_pokemon_impl_ptr->unlock();

        // Set the entry in the native struct.
        NATIVE_RCAST(_p_native)->stored_pokemon = *reinterpret_cast<struct pksav_gen1_pc_pokemon*>(
                                                       new_pokemon_native_pc_ptr
                                                   );
        r_levelup_pokemon[position] = std::make_shared<pokemon_gen1impl>(
                                          &NATIVE_RCAST(_p_native)->stored_pokemon,
                                          _game_id
                                      );

        // Don't set empty names.
        std::string new_pokemon_nickname = actual_new_pokemon->get_nickname();
        std::string new_pokemon_trainer_name = actual_new_pokemon->get_original_trainer_name();
        if(new_pokemon_nickname.size() == 0)
        {
            new_pokemon_nickname = "None";
        }
        if(new_pokemon_trainer_name.size() == 0)
        {
            new_pokemon_trainer_name = "None";
        }

        r_levelup_pokemon[position]->set_nickname(new_pokemon_nickname);
        r_levelup_pokemon[position]->set_original_trainer_name(new_pokemon_trainer_name);

        // Set the names in the struct.
        this->_to_native_levelup();
    }

    int daycare_gen1impl::get_levelup_pokemon_capacity()
    {
        return LEVELUP_CAPACITY;
    }

    void daycare_gen1impl::set_breeding_pokemon(
        int,
        const pkmn::pokemon::sptr&
    )
    {
        throw pkmn::feature_not_in_game_error(
                  "Breeding",
                  this->get_game()
              );
    }

    int daycare_gen1impl::get_breeding_pokemon_capacity()
    {
        return BREEDING_CAPACITY;
    }

    void daycare_gen1impl::_from_native_levelup()
    {
        // For the sake of speed, when a Pokémon is taken out of daycare,
        // the game doesn't zero out the memory and just uses the "in use"
        // flag to dictate whether the daycare is in use. However, LibPKMN
        // needs the underlying memory to reflect the lack of Pokémon, so
        // we'll zero out the memory ourselves.
        if(not NATIVE_RCAST(_p_native)->is_daycare_in_use)
        {
            std::memset(
                NATIVE_RCAST(_p_native)->stored_pokemon_nickname,
                PKSAV_GEN1_TEXT_TERMINATOR,
                sizeof(NATIVE_RCAST(_p_native)->stored_pokemon_nickname)
            );
            std::memset(
                NATIVE_RCAST(_p_native)->stored_pokemon_otname,
                PKSAV_GEN1_TEXT_TERMINATOR,
                sizeof(NATIVE_RCAST(_p_native)->stored_pokemon_otname)
            );
            std::memset(
                &NATIVE_RCAST(_p_native)->stored_pokemon,
                0,
                sizeof(NATIVE_RCAST(_p_native)->stored_pokemon)
            );
        }

        pkmn::pokemon_list_t& r_levelup_pokemon = this->_get_levelup_pokemon_ref();

        r_levelup_pokemon.resize(LEVELUP_CAPACITY);
        r_levelup_pokemon[0].reset(
            new pokemon_gen1impl(
                &NATIVE_RCAST(_p_native)->stored_pokemon,
                _game_id
            )
        );

        char nickname[PKSAV_GEN1_POKEMON_NICKNAME_LENGTH + 1] = {0};
        PKSAV_CALL(
            pksav_gen1_import_text(
                NATIVE_RCAST(_p_native)->stored_pokemon_nickname,
                nickname,
                PKSAV_GEN1_POKEMON_NICKNAME_LENGTH
            );
        )
        if(std::strlen(nickname) > 0)
        {
            r_levelup_pokemon[0]->set_nickname(nickname);
        }

        char otname[PKSAV_GEN1_POKEMON_OTNAME_LENGTH + 1] = {0};
        PKSAV_CALL(
            pksav_gen1_import_text(
                NATIVE_RCAST(_p_native)->stored_pokemon_otname,
                otname,
                PKSAV_GEN1_POKEMON_OTNAME_LENGTH
            );
        )
        if(std::strlen(otname) > 0)
        {
            r_levelup_pokemon[0]->set_original_trainer_name(otname);
        }
    }

    void daycare_gen1impl::_to_native_levelup()
    {
        pkmn::pokemon_list_t& r_levelup_pokemon = this->_get_levelup_pokemon_ref();
        BOOST_ASSERT(!r_levelup_pokemon.empty());

        if(r_levelup_pokemon[0]->get_database_entry().get_species_id() == 0)
        {
            NATIVE_RCAST(_p_native)->is_daycare_in_use = PKSAV_GEN1_DAYCARE_NOT_IN_USE;
        }
        else
        {
            NATIVE_RCAST(_p_native)->is_daycare_in_use = PKSAV_GEN1_DAYCARE_IN_USE;
        }

        PKSAV_CALL(
            pksav_gen1_export_text(
                r_levelup_pokemon[0]->get_nickname().c_str(),
                NATIVE_RCAST(_p_native)->stored_pokemon_nickname,
                PKSAV_GEN1_POKEMON_NICKNAME_LENGTH
            );
        )
        PKSAV_CALL(
            pksav_gen1_export_text(
                r_levelup_pokemon[0]->get_original_trainer_name().c_str(),
                NATIVE_RCAST(_p_native)->stored_pokemon_otname,
                PKSAV_GEN1_POKEMON_OTNAME_STORAGE_LENGTH
            );
        )
    }

    void daycare_gen1impl::_from_native_breeding() {}
    void daycare_gen1impl::_to_native_breeding() {}
}

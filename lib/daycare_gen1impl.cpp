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

        this->_from_native_levelup();
    }

    daycare_gen1impl::~daycare_gen1impl()
    {
        if(_is_our_mem)
        {
            delete NATIVE_RCAST(_p_native);
        }
    }

    int daycare_gen1impl::get_levelup_pokemon_capacity()
    {
        return LEVELUP_CAPACITY;
    }

    bool daycare_gen1impl::can_breed_pokemon()
    {
        return CAN_BREED_POKEMON;
    }

    int daycare_gen1impl::get_breeding_pokemon_capacity()
    {
        return BREEDING_CAPACITY;
    }

    void daycare_gen1impl::_set_levelup_pokemon(
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

        // Make sure no one else is using the new Pokémon variable.
        pokemon_gen1impl* p_new_pokemon = dynamic_cast<pokemon_gen1impl*>(
                                              new_pokemon.get()
                                          );
        BOOST_ASSERT(p_new_pokemon != nullptr);
        boost::lock_guard<pokemon_gen1impl> new_pokemon_lock(*p_new_pokemon);

        // Copy the underlying memory.
        //
        // Note: as we control the implementation, we know the PC data points
        // to the whole Pokémon data structure.
        rcast_equal<struct pksav_gen1_pc_pokemon>(
            new_pokemon->get_native(),
            &NATIVE_RCAST(_p_native)->stored_pokemon
        );
        r_levelup_pokemon[position] = std::make_shared<pokemon_gen1impl>(
                                          &NATIVE_RCAST(_p_native)->stored_pokemon,
                                          _game_id
                                      );

        // Don't set empty names.
        std::string new_pokemon_nickname = new_pokemon->get_nickname();
        std::string new_pokemon_trainer_name = new_pokemon->get_original_trainer_name();
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

    void daycare_gen1impl::_set_breeding_pokemon(
        int,
        const pkmn::pokemon::sptr&
    )
    {
        throw pkmn::feature_not_in_game_error(
                  "Breeding",
                  this->get_game()
              );
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

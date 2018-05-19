/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "daycare_gen2impl.hpp"
#include "pokemon_gen2impl.hpp"

#include "exception_internal.hpp"

#include "pksav/pksav_call.hpp"

#include <pksav/gen2/daycare_data.h>
#include <pksav/gen2/pokemon.h>
#include <pksav/gen2/text.h>

#include <boost/assert.hpp>
#include <boost/thread/lock_guard.hpp>

#define NATIVE_RCAST(ptr) (reinterpret_cast<struct pksav_gen2_daycare_data*>(ptr))

namespace pkmn {

    static void import_native_pokemon_data(
        pksav_gen2_daycare_pokemon_data* p_native_pokemon_data,
        pkmn::pokemon::sptr& r_pokemon,
        int game_id
    )
    {
        BOOST_ASSERT(p_native_pokemon_data != nullptr);

        r_pokemon.reset(
            new pokemon_gen2impl(
                    &p_native_pokemon_data->pokemon,
                    game_id
                )
        );

        char nickname[PKSAV_GEN2_POKEMON_NICKNAME_LENGTH + 1] = {0};
        PKSAV_CALL(
            pksav_gen2_import_text(
                p_native_pokemon_data->nickname,
                nickname,
                PKSAV_GEN2_POKEMON_NICKNAME_LENGTH
            );
        )
        r_pokemon->set_nickname(nickname);

        char otname[PKSAV_GEN2_POKEMON_OTNAME_LENGTH + 1] = {0};
        PKSAV_CALL(
            pksav_gen2_import_text(
                p_native_pokemon_data->otname,
                otname,
                PKSAV_GEN2_POKEMON_OTNAME_LENGTH
            );
        )
        r_pokemon->set_original_trainer_name(otname);
    }

    static void export_native_pokemon_data(
        pksav_gen2_daycare_pokemon_data* p_native_pokemon_data,
        const pkmn::pokemon::sptr& pokemon
    )
    {
        PKSAV_CALL(
            pksav_gen2_export_text(
                pokemon->get_nickname().c_str(),
                p_native_pokemon_data->nickname,
                sizeof(p_native_pokemon_data->nickname)
            );
        )
        PKSAV_CALL(
            pksav_gen2_export_text(
                pokemon->get_original_trainer_name().c_str(),
                p_native_pokemon_data->otname,
                sizeof(p_native_pokemon_data->otname)
            );
        )
    }

    daycare_gen2impl::daycare_gen2impl(
        int game_id,
        void* p_native
    ): daycare_impl(game_id)
    {
        if(p_native == nullptr)
        {
            _p_native = new struct pksav_gen2_daycare_data;
            _is_our_mem = true;

            // TODO: flags
        }
        else
        {
            _p_native = p_native;
            _is_our_mem = false;
        }

        _can_breed = true;

        this->_from_native_levelup();
        this->_from_native_breeding();
    }

    daycare_gen2impl::~daycare_gen2impl()
    {
        if(_is_our_mem)
        {
            delete NATIVE_RCAST(_p_native);
        }
    }

    void daycare_gen2impl::set_levelup_pokemon(
        int position,
        const pkmn::pokemon::sptr& new_pokemon
    )
    {
        pkmn::enforce_bounds(
            "Position",
            position,
            0,
            (LEVELUP_CAPACITY - 1)
        );

        boost::lock_guard<daycare_gen2impl> lock(*this);

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
        copy_box_pokemon<struct pksav_gen2_pc_pokemon, struct pksav_gen2_pokemon_party_data>(
            r_levelup_pokemon,
            position
        );

        // Copy the new Pokémon's internals into the box's internals and create a new sptr.
        void* new_pokemon_native_pc_ptr = new_pokemon_impl_ptr->_native_pc;

        // Unlock the old Pokémon's mutex is unlocked before it's destructor is called.
        old_box_pokemon_impl_ptr->unlock();

        // Set the entry in the native struct.
        struct pksav_gen2_pc_pokemon* p_native_pokemon =
            (position == 0) ? &NATIVE_RCAST(_p_native)->stored_pokemon1_data.pokemon
                            : &NATIVE_RCAST(_p_native)->stored_pokemon2_data.pokemon;

        *p_native_pokemon = *reinterpret_cast<struct pksav_gen2_pc_pokemon*>(
                                 new_pokemon_native_pc_ptr
                             );
        r_levelup_pokemon[position] = std::make_shared<pokemon_gen2impl>(
                                          p_native_pokemon,
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

        // Set the names in the struct. TODO: do this smarter
        this->_to_native_levelup();
    }

    int daycare_gen2impl::get_levelup_pokemon_capacity()
    {
        return LEVELUP_CAPACITY;
    }

    void daycare_gen2impl::set_breeding_pokemon(
        int position,
        const pkmn::pokemon::sptr& new_pokemon
    )
    {
        // Same thing as levelup for Generation II
        this->set_levelup_pokemon(position, new_pokemon);
    }

    int daycare_gen2impl::get_breeding_pokemon_capacity()
    {
        return BREEDING_CAPACITY;
    }

    void daycare_gen2impl::_from_native_levelup()
    {
        // TODO: if not set, zero out memory first

        pkmn::pokemon_list_t& r_levelup_pokemon = this->_get_levelup_pokemon_ref();
        r_levelup_pokemon.resize(LEVELUP_CAPACITY);

        import_native_pokemon_data(
            &NATIVE_RCAST(_p_native)->stored_pokemon1_data,
            r_levelup_pokemon[0],
            _game_id
        );
        import_native_pokemon_data(
            &NATIVE_RCAST(_p_native)->stored_pokemon2_data,
            r_levelup_pokemon[1],
            _game_id
        );
    }

    void daycare_gen2impl::_to_native_levelup()
    {
        // TODO: if Pokémon aren't empty, set bit to say daycare's in use
        //       be smarter if an entry isn't set

        pkmn::pokemon_list_t& r_levelup_pokemon = this->_get_levelup_pokemon_ref();
        BOOST_ASSERT(!r_levelup_pokemon.empty());

        export_native_pokemon_data(
            &NATIVE_RCAST(_p_native)->stored_pokemon1_data,
            r_levelup_pokemon[0]
        );
        export_native_pokemon_data(
            &NATIVE_RCAST(_p_native)->stored_pokemon2_data,
            r_levelup_pokemon[1]
        );
    }

    void daycare_gen2impl::_from_native_breeding()
    {
        import_native_pokemon_data(
            &NATIVE_RCAST(_p_native)->egg_pokemon_data,
            _egg,
            _game_id
        );
    }

    void daycare_gen2impl::_to_native_breeding()
    {
        export_native_pokemon_data(
            &NATIVE_RCAST(_p_native)->egg_pokemon_data,
            _egg
        );
    }
}

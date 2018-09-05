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

    static void init_empty_pokemon_data(
        pksav_gen2_daycare_pokemon_data* p_native_pokemon_data
    )
    {
        BOOST_ASSERT(p_native_pokemon_data != nullptr);

        std::memset(
            p_native_pokemon_data->nickname,
            PKSAV_GEN2_TEXT_TERMINATOR,
            sizeof(p_native_pokemon_data->nickname)
        );
        std::memset(
            p_native_pokemon_data->otname,
            PKSAV_GEN2_TEXT_TERMINATOR,
            sizeof(p_native_pokemon_data->otname)
        );
        std::memset(
            &p_native_pokemon_data->pokemon,
            0,
            sizeof(p_native_pokemon_data->pokemon)
        );
    }

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
        if(std::strlen(nickname) > 0ULL)
        {
            r_pokemon->set_nickname(nickname);
        }

        char otname[PKSAV_GEN2_POKEMON_OTNAME_LENGTH + 1] = {0};
        PKSAV_CALL(
            pksav_gen2_import_text(
                p_native_pokemon_data->otname,
                otname,
                PKSAV_GEN2_POKEMON_OTNAME_LENGTH
            );
        )
        if(std::strlen(otname) > 0ULL)
        {
            r_pokemon->set_original_trainer_name(otname);
        }
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

            // Only fields set
            NATIVE_RCAST(_p_native)->daycare_man_fields =
                PKSAV_GEN2_DAYCARE_MAN_IS_ACTIVE_MASK;
            NATIVE_RCAST(_p_native)->daycare_lady_fields =
                PKSAV_GEN2_DAYCARE_LADY_IS_ACTIVE_MASK;

            NATIVE_RCAST(_p_native)->steps_to_egg = 0;
            NATIVE_RCAST(_p_native)->is_breed_mother_or_non_ditto =
                PKSAV_GEN2_DAYCARE_BREED_IS_NOT_MOTHER_OR_NON_DITTO;

            init_empty_pokemon_data(&NATIVE_RCAST(_p_native)->stored_pokemon1_data);
            init_empty_pokemon_data(&NATIVE_RCAST(_p_native)->stored_pokemon2_data);
            init_empty_pokemon_data(&NATIVE_RCAST(_p_native)->egg_pokemon_data);
        }
        else
        {
            _p_native = p_native;
            _is_our_mem = false;
        }

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

    int daycare_gen2impl::get_levelup_pokemon_capacity()
    {
        return LEVELUP_CAPACITY;
    }

    bool daycare_gen2impl::can_breed_pokemon()
    {
        return CAN_BREED_POKEMON;
    }

    int daycare_gen2impl::get_breeding_pokemon_capacity()
    {
        return BREEDING_CAPACITY;
    }

    void daycare_gen2impl::_set_levelup_pokemon(
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

        // Make sure no one else is using the new Pokémon variable.
        pokemon_gen2impl* p_new_pokemon = dynamic_cast<pokemon_gen2impl*>(
                                              new_pokemon.get()
                                          );
        BOOST_ASSERT(p_new_pokemon != nullptr);
        boost::lock_guard<pokemon_gen2impl> new_pokemon_lock(*p_new_pokemon);

        // Copy the underlying memory.
        //
        // Note: as we control the implementation, we know the PC data points
        // to the whole Pokémon data structure.
        struct pksav_gen2_pc_pokemon* p_native_pokemon =
            (position == 0) ? &NATIVE_RCAST(_p_native)->stored_pokemon1_data.pokemon
                            : &NATIVE_RCAST(_p_native)->stored_pokemon2_data.pokemon;

        rcast_equal<struct pksav_gen2_pc_pokemon>(
            new_pokemon->get_native(),
            p_native_pokemon
        );
        r_levelup_pokemon[position] = std::make_shared<pokemon_gen2impl>(
                                          p_native_pokemon,
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

        // Set the names in the struct. TODO: do this smarter
        this->_to_native_levelup();
    }

    void daycare_gen2impl::_set_breeding_pokemon(
        int position,
        const pkmn::pokemon::sptr& new_pokemon
    )
    {
        // Same thing as levelup for Generation II
        this->set_levelup_pokemon(position, new_pokemon);
    }

    void daycare_gen2impl::_from_native_levelup()
    {
        // For the sake of speed, when a Pokémon is taken out of daycare,
        // the game doesn't zero out the memory and just uses the "in use"
        // flag to dictate whether the daycare is in use. However, LibPKMN
        // needs the underlying memory to reflect the lack of Pokémon, so
        // we'll zero out the memory ourselves.
        if(!(NATIVE_RCAST(_p_native)->daycare_man_fields & PKSAV_GEN2_DAYCARE_MAN_POKEMON_PRESENT_MASK))
        {
            init_empty_pokemon_data(&NATIVE_RCAST(_p_native)->stored_pokemon1_data);
        }
        if(!(NATIVE_RCAST(_p_native)->daycare_lady_fields & PKSAV_GEN2_DAYCARE_LADY_POKEMON_PRESENT_MASK))
        {
            init_empty_pokemon_data(&NATIVE_RCAST(_p_native)->stored_pokemon2_data);
        }
        if(!(NATIVE_RCAST(_p_native)->daycare_man_fields & PKSAV_GEN2_DAYCARE_MAN_IS_EGG_READY_MASK))
        {
            init_empty_pokemon_data(&NATIVE_RCAST(_p_native)->egg_pokemon_data);
        }

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
        _egg->set_is_egg(true);
    }

    void daycare_gen2impl::_to_native_breeding()
    {
        export_native_pokemon_data(
            &NATIVE_RCAST(_p_native)->egg_pokemon_data,
            _egg
        );
    }
}

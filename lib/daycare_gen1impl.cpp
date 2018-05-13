/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "daycare_gen1impl.hpp"
#include "pokemon_gen1impl.hpp"

#include "pksav/pksav_call.hpp"

#include <pksav/gen1/daycare_data.h>
#include <pksav/gen1/pokemon.h>
#include <pksav/gen1/text.h>

#include <boost/assert.hpp>

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
            _is_our_mem = true;

            // TODO: flags
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

    int daycare_gen1impl::get_levelup_pokemon_capacity()
    {
        return LEVELUP_CAPACITY;
    }

    int daycare_gen1impl::get_breeding_pokemon_capacity()
    {
        return BREEDING_CAPACITY;
    }

    void daycare_gen1impl::_from_native_levelup()
    {
        // TODO: if not set, zero out memory first

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
        r_levelup_pokemon[0]->set_nickname(nickname);

        char otname[PKSAV_GEN1_POKEMON_OTNAME_LENGTH + 1] = {0};
        PKSAV_CALL(
            pksav_gen1_import_text(
                NATIVE_RCAST(_p_native)->stored_pokemon_otname,
                otname,
                PKSAV_GEN1_POKEMON_OTNAME_LENGTH
            );
        )
        r_levelup_pokemon[0]->set_original_trainer_name(otname);
    }

    void daycare_gen1impl::_to_native_levelup()
    {
        // TODO: if Pokémon aren't empty, set bit to say daycare's in use

        pkmn::pokemon_list_t& r_levelup_pokemon = this->_get_levelup_pokemon_ref();
        BOOST_ASSERT(!r_levelup_pokemon.empty());

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

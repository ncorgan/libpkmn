/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "daycare_gen2impl.hpp"
#include "pokemon_gen2impl.hpp"

#include "pksav/pksav_call.hpp"

#include <pksav/gen2/daycare_data.h>
#include <pksav/gen2/pokemon.h>
#include <pksav/gen2/text.h>

#include <boost/assert.hpp>

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
        import_native_pokemon_data(
            &NATIVE_RCAST(_p_native)->egg_pokemon_data,
            _egg,
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
        export_native_pokemon_data(
            &NATIVE_RCAST(_p_native)->egg_pokemon_data,
            _egg
        );
    }

    void daycare_gen2impl::_from_native_breeding() {}
    void daycare_gen2impl::_to_native_breeding() {}
}

/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "daycare_impl.hpp"
#include "daycare_gen1impl.hpp"
#include "daycare_gen2impl.hpp"
#include "daycare_gcnimpl.hpp"

#include "exception_internal.hpp"

#include "database/database_common.hpp"
#include "database/id_to_string.hpp"

#include <boost/assert.hpp>
#include <boost/thread/lock_guard.hpp>

namespace pkmn {

    daycare::sptr daycare::make(const std::string& game)
    {
        int game_id = pkmn::database::game_name_to_id(game);
        int generation = pkmn::database::game_name_to_generation(game);

        daycare::sptr ret;

        switch(generation)
        {
            case 1:
                ret = std::make_shared<daycare_gen1impl>(game_id);
                break;

            case 2:
                ret = std::make_shared<daycare_gen2impl>(game_id);
                break;

            case 3:
                if(game_is_gamecube(game_id))
                {
                    ret = std::make_shared<daycare_gcnimpl>(game_id);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

            case 4:
            case 5:
            case 6:
            case 7:
                throw pkmn::unimplemented_error();

            default:
                throw std::out_of_range("Invalid game");
        }

        return ret;
    }

    daycare_impl::daycare_impl(int game_id):
        daycare(),
        _game_id(game_id),
        _generation(pkmn::database::game_id_to_generation(game_id)),
        _can_breed(false),
        _levelup_pokemon(),
        _breeding_pokemon(),
        _egg(pkmn::pokemon::make("None", pkmn::database::game_id_to_name(game_id), "", 1)),
        _p_native(nullptr),
        _is_our_mem(false)
    {}

    daycare_impl::~daycare_impl() {}

    std::string daycare_impl::get_game()
    {
        return pkmn::database::game_id_to_name(_game_id);
    }

    const pkmn::pokemon_list_t& daycare_impl::get_levelup_pokemon()
    {
        boost::lock_guard<daycare_impl> lock(*this);

        return this->_get_levelup_pokemon_ref();
    }

    const pkmn::pokemon_list_t& daycare_impl::get_breeding_pokemon()
    {
        if(!_can_breed)
        {
            throw pkmn::feature_not_in_game_error(
                      "Breeding",
                      this->get_game()
                  );
        }

        boost::lock_guard<daycare_impl> lock(*this);

        return _breeding_pokemon;
    }

    const pkmn::pokemon::sptr& daycare_impl::get_egg()
    {
        if(!_can_breed)
        {
            throw pkmn::feature_not_in_game_error(
                      "Breeding",
                      this->get_game()
                  );
        }

        boost::lock_guard<daycare_impl> lock(*this);

        return _egg;
    }

    void* daycare_impl::get_native()
    {
        boost::lock_guard<daycare_impl> lock(*this);

        _to_native_levelup();
        _to_native_breeding();

        return _p_native;
    }

    // Before Generation VII, daycares and nurseries were the same, so
    // we use a single vector. The choice of using the breeding vector
    // is arbitrary.
    pkmn::pokemon_list_t& daycare_impl::_get_levelup_pokemon_ref()
    {
        return (_generation < 7) ? _breeding_pokemon : _levelup_pokemon;
    }
}

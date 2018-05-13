/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "daycare_impl.hpp"
#include "exception_internal.hpp"

#include "database/database_common.hpp"
#include "database/id_to_string.hpp"

#include <boost/assert.hpp>
#include <boost/thread/lock_guard.hpp>

namespace pkmn {

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

    void daycare_impl::add_levelup_pokemon(
        const pkmn::pokemon::sptr& pokemon
    )
    {
        boost::lock_guard<daycare_impl> lock(*this);

        int capacity = this->get_levelup_pokemon_capacity();

        pkmn::pokemon_list_t& r_levelup_pokemon = this->_get_levelup_pokemon_ref();

        if(r_levelup_pokemon.size() < static_cast<size_t>(capacity))
        {
            pkmn::pokemon::sptr actual_pokemon;
            if(pokemon->get_database_entry().get_game_id() == _game_id)
            {
                actual_pokemon = pokemon;
            }
            else
            {
                actual_pokemon = pokemon->to_game(this->get_game());
            }

            r_levelup_pokemon.emplace_back(actual_pokemon);

            this->_to_native_levelup();
        }
    }

    void daycare_impl::remove_levelup_pokemon(int position)
    {
        boost::lock_guard<daycare_impl> lock(*this);

        pkmn::pokemon_list_t& r_levelup_pokemon = this->_get_levelup_pokemon_ref();

        if(r_levelup_pokemon.empty())
        {
            throw std::runtime_error("Cannot remove Pokémon from an empty daycare.");
        }

        pkmn::enforce_bounds(
            "Position",
            position,
            0,
            static_cast<int>(r_levelup_pokemon.size() - 1)
        );

        r_levelup_pokemon.erase(r_levelup_pokemon.begin() + position);

        this->_to_native_levelup();
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

    void daycare_impl::add_breeding_pokemon(
        const pkmn::pokemon::sptr& pokemon
    )
    {
        if(!_can_breed)
        {
            throw pkmn::feature_not_in_game_error(
                      "Breeding",
                      this->get_game()
                  );
        }

        boost::lock_guard<daycare_impl> lock(*this);

        int capacity = this->get_breeding_pokemon_capacity();

        if(_breeding_pokemon.size() < static_cast<size_t>(capacity))
        {
            pkmn::pokemon::sptr actual_pokemon;
            if(pokemon->get_database_entry().get_game_id() == _game_id)
            {
                actual_pokemon = pokemon;
            }
            else
            {
                actual_pokemon = pokemon->to_game(this->get_game());
            }

            _breeding_pokemon.emplace_back(actual_pokemon);

            this->_to_native_breeding();
        }
    }

    void daycare_impl::remove_breeding_pokemon(int position)
    {
        if(!_can_breed)
        {
            throw pkmn::feature_not_in_game_error(
                      "Breeding",
                      this->get_game()
                  );
        }

        boost::lock_guard<daycare_impl> lock(*this);

        if(_breeding_pokemon.empty())
        {
            throw std::runtime_error("Cannot remove Pokémon from an empty daycare.");
        }

        pkmn::enforce_bounds(
            "Position",
            position,
            0,
            static_cast<int>(_breeding_pokemon.size() - 1)
        );

        _breeding_pokemon.erase(_breeding_pokemon.begin() + position);

        this->_to_native_breeding();
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

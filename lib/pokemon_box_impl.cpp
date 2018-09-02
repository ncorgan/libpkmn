/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "pokemon_box_impl.hpp"
#include "pokemon_box_gbimpl.hpp"
#include "pokemon_box_gbaimpl.hpp"
#include "pokemon_box_gcnimpl.hpp"

#include "database/database_common.hpp"
#include "database/enum_conversions.hpp"
#include "database/id_to_string.hpp"

#include "polymorphism/is_libpkmn_impl.hpp"
#include "polymorphism/pokemon.hpp"

#include "utils/misc.hpp"

#include <pkmn/exception.hpp>

#include <stdexcept>

namespace pkmn {

    pokemon_box::sptr pokemon_box::make(pkmn::e_game game)
    {
        int game_id = pkmn::database::game_enum_to_id(game);
        int generation = pkmn::database::game_id_to_generation(game_id);

        switch(generation)
        {
            case 1:
                return std::make_shared<pokemon_box_gen1impl>(game_id);

            case 2:
                return std::make_shared<pokemon_box_gen2impl>(game_id);

            case 3:
                if(game_is_gamecube(game_id))
                {
                    return std::make_shared<pokemon_box_gcnimpl>(game_id);
                }
                else
                {
                    return std::make_shared<pokemon_box_gbaimpl>(game_id);
                }

            case 4:
            case 5:
            case 6:
                throw pkmn::unimplemented_error();

            default:
                throw std::runtime_error("Invalid game.");
        }
    }

    pokemon_box_impl::pokemon_box_impl(
        int game_id
    ): _pokemon_list(),
       _box_name(""),
       _p_native(nullptr),
       _game_id(game_id),
       _generation(pkmn::database::game_id_to_generation(game_id))
    {}

    pkmn::e_game pokemon_box_impl::get_game()
    {
        return pkmn::database::game_id_to_enum(_game_id);
    }

    const pkmn::pokemon::sptr& pokemon_box_impl::get_pokemon(
        int index
    )
    {
        int capacity = get_capacity();
        pkmn::enforce_bounds("Box index", index, 0, (capacity-1));

        boost::lock_guard<pokemon_box_impl> lock(*this);

        return _pokemon_list.at(index);
    }

    void pokemon_box_impl::set_pokemon(
        int index,
        const pkmn::pokemon::sptr& new_pokemon
    )
    {
        int capacity = get_capacity();
        pkmn::enforce_bounds("Box index", index, 0, (capacity-1));

        boost::lock_guard<pokemon_box_impl> lock(*this);

        // Make sure we only proceed if the given Pokémon variable is a LibPKMN
        // implementation of the correct game.
        pkmn::pokemon::sptr actual_new_pokemon;
        if(pkmn::polymorphism::is_pokemon_from_libpkmn(new_pokemon.get()))
        {
            actual_new_pokemon = new_pokemon;
        }
        else
        {
            actual_new_pokemon = pkmn::polymorphism::pokemon_to_libpkmn_impl(
                                     new_pokemon
                                 );
        }

        if(actual_new_pokemon->get_game() != get_game())
        {
            actual_new_pokemon = actual_new_pokemon->to_game(get_game());
        }

        // Actual implementation
        BOOST_ASSERT(actual_new_pokemon.get() != nullptr);
        BOOST_ASSERT(pkmn::polymorphism::is_pokemon_from_libpkmn(actual_new_pokemon.get()));
        _set_pokemon(index, actual_new_pokemon);
    }

    const pkmn::pokemon_list_t& pokemon_box_impl::as_vector()
    {
        boost::lock_guard<pokemon_box_impl> lock(*this);

        return _pokemon_list;
    }

    void* pokemon_box_impl::get_native()
    {
        boost::lock_guard<pokemon_box_impl> lock(*this);

        _to_native();

        return _p_native;
    }
}

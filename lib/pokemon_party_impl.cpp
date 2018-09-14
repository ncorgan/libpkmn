/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "pokemon_party_impl.hpp"
#include "pokemon_party_gbimpl.hpp"
#include "pokemon_party_gbaimpl.hpp"
#include "pokemon_party_gcnimpl.hpp"

#include "database/database_common.hpp"
#include "database/enum_conversions.hpp"
#include "database/id_to_string.hpp"

#include "polymorphism/is_libpkmn_impl.hpp"
#include "polymorphism/pokemon.hpp"

#include "types/mutex_helpers.hpp"

#include "utils/misc.hpp"

#include <pkmn/exception.hpp>

#include <boost/format.hpp>

#include <stdexcept>

namespace pkmn {

    pokemon_party::sptr pokemon_party::make(pkmn::e_game game)
    {
        int game_id = pkmn::database::game_enum_to_id(game);
        int generation = pkmn::database::game_id_to_generation(game_id);

        switch(generation)
        {
            case 1:
                return std::make_shared<pokemon_party_gen1impl>(game_id);

            case 2:
                return std::make_shared<pokemon_party_gen2impl>(game_id);

            case 3:
                if(game_is_gamecube(game_id))
                {
                    return std::make_shared<pokemon_party_gcnimpl>(game_id);
                }
                else
                {
                    return std::make_shared<pokemon_party_gbaimpl>(game_id);
                }

            case 4:
            case 5:
            case 6:
                throw pkmn::unimplemented_error();

            default:
                throw std::runtime_error("Invalid game.");
        }
    }

    pokemon_party_impl::pokemon_party_impl(
        int game_id
    ): _p_native(nullptr),
       _game_id(game_id),
       _generation(pkmn::database::game_id_to_generation(game_id))
    {}

    pkmn::e_game pokemon_party_impl::get_game()
    {
        return pkmn::database::game_id_to_enum(_game_id);
    }

    const pkmn::pokemon::sptr& pokemon_party_impl::get_pokemon(
        int index
    )
    {
        pkmn::enforce_bounds("Party index", index, 0, (PARTY_SIZE-1));

        pkmn::lock_guard<pokemon_party_impl> lock(*this);

        return _pokemon_list.at(index);
    }

    void pokemon_party_impl::set_pokemon(
        int index,
        const pkmn::pokemon& new_pokemon
    )
    {
        pkmn::enforce_bounds("Party index", index, 0, (PARTY_SIZE-1));

        pkmn::lock_guard<pokemon_party_impl> lock(*this);

        _set_pokemon(
            index,
            pkmn::polymorphism::pokemon_to_libpkmn_impl_of_game(
                new_pokemon,
                get_game()
            )
        );
    }

    void pokemon_party_impl::set_pokemon(
        int index,
        const pkmn::pokemon::sptr& new_pokemon
    )
    {
        pkmn::enforce_bounds("Party index", index, 0, (PARTY_SIZE-1));

        pkmn::lock_guard<pokemon_party_impl> lock(*this);

        _set_pokemon(
            index,
            pkmn::polymorphism::pokemon_to_libpkmn_impl_of_game(
                new_pokemon,
                get_game()
            )
        );
    }

    const pkmn::pokemon_list_t& pokemon_party_impl::as_vector()
    {
        pkmn::lock_guard<pokemon_party_impl> lock(*this);

        return _pokemon_list;
    }

    void* pokemon_party_impl::get_native()
    {
        pkmn::lock_guard<pokemon_party_impl> lock(*this);

        _to_native();

        return _p_native;
    }
}

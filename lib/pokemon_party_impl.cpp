/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "pokemon_party_impl.hpp"
#include "pokemon_party_gbimpl.hpp"
#include "pokemon_party_gbaimpl.hpp"
#include "pokemon_party_gcnimpl.hpp"
#include "pokemon_party_gen6impl.hpp"

#include "database/database_common.hpp"
#include "database/id_to_string.hpp"

#include "misc_common.hpp"

#include <pkmn/exception.hpp>

#include <boost/format.hpp>

#include <stdexcept>

namespace pkmn {

    pokemon_party::sptr pokemon_party::make(
        const std::string &game
    ) {
        int game_id = pkmn::database::game_name_to_id(game);
        int generation = pkmn::database::game_id_to_generation(game_id);

        switch(generation) {
            case 1:
                return pkmn::make_shared<pokemon_party_gen1impl>(game_id);

            case 2:
                return pkmn::make_shared<pokemon_party_gen2impl>(game_id);

            case 3:
                if(game_is_gamecube(game_id)) {
                    return pkmn::make_shared<pokemon_party_gcnimpl>(game_id);
                } else {
                    return pkmn::make_shared<pokemon_party_gbaimpl>(game_id);
                }

            case 4:
            case 5:
                throw pkmn::unimplemented_error();

            case 6:
                return pkmn::make_shared<pokemon_party_gen6impl>(game_id);

            default:
                throw std::runtime_error("Invalid game.");
        }
    }

    pokemon_party_impl::pokemon_party_impl(
        int game_id
    ): _native(nullptr),
       _our_mem(false),
       _game_id(game_id),
       _generation(pkmn::database::game_id_to_generation(game_id))
    {}

    std::string pokemon_party_impl::get_game() {
        return pkmn::database::game_id_to_name(
                   _game_id
               );
    }

    pkmn::pokemon::sptr pokemon_party_impl::get_pokemon(
        int index
    )
    {
        pkmn::enforce_bounds("Party index", index, 0, (PARTY_SIZE-1));

        return _pokemon_list.at(index);
    }

    const pkmn::pokemon_list_t& pokemon_party_impl::as_vector() {
        return _pokemon_list;
    }

    void* pokemon_party_impl::get_native() {
        return _native;
    }
}

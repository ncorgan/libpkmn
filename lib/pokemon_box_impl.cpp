/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
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
#include "database/id_to_string.hpp"

#include "misc_common.hpp"

#include <pkmn/exception.hpp>

#include <stdexcept>

namespace pkmn {

    pokemon_box::sptr pokemon_box::make(
        const std::string &game
    ) {
        int game_id = pkmn::database::game_name_to_id(game);
        int generation = pkmn::database::game_id_to_generation(game_id);

        switch(generation) {
            case 1:
                return pkmn::make_shared<pokemon_box_gen1impl>(game_id);

            case 2:
                return pkmn::make_shared<pokemon_box_gen2impl>(game_id);

            case 3:
                if(game_is_gamecube(game_id)) {
                    return pkmn::make_shared<pokemon_box_gcnimpl>(game_id);
                } else {
                    return pkmn::make_shared<pokemon_box_gbaimpl>(game_id);
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
        _native(nullptr),
        _our_mem(false),
        _mem_mutex(),
       _game_id(game_id),
       _generation(pkmn::database::game_id_to_generation(game_id))
    {}

    std::string pokemon_box_impl::get_game() {
        return pkmn::database::game_id_to_name(
                   _game_id
               );
    }

    pkmn::pokemon::sptr pokemon_box_impl::get_pokemon(
        int index
    )
    {
        int capacity = get_capacity();
        pkmn::enforce_bounds("Box index", index, 0, (capacity-1));

        return _pokemon_list.at(index);
    }

    const pkmn::pokemon_list_t& pokemon_box_impl::as_vector() {
        return _pokemon_list;
    }

    void* pokemon_box_impl::get_native() {
        return _native;
    }
}

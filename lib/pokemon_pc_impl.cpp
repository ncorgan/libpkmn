/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "pokemon_pc_impl.hpp"
#include "pokemon_pc_gen1impl.hpp"
#include "pokemon_pc_gen2impl.hpp"
#include "pokemon_pc_gbaimpl.hpp"
#include "pokemon_pc_gcnimpl.hpp"

#include "utils/misc.hpp"

#include "database/database_common.hpp"
#include "database/id_to_string.hpp"

#include <pkmn/exception.hpp>

#include <stdexcept>

namespace pkmn {

    pokemon_pc::sptr pokemon_pc::make(
        const std::string &game
    ) {
        int game_id = pkmn::database::game_name_to_id(game);
        int generation = pkmn::database::game_id_to_generation(game_id);

        switch(generation) {
            case 1:
                return std::make_shared<pokemon_pc_gen1impl>(game_id);

            case 2:
                return std::make_shared<pokemon_pc_gen2impl>(game_id);

            case 3:
                if(game_is_gamecube(game_id)) {
                    return std::make_shared<pokemon_pc_gcnimpl>(game_id);
                } else {
                    return std::make_shared<pokemon_pc_gbaimpl>(game_id);
                }

            case 4:
            case 5:
            case 6:
                throw pkmn::unimplemented_error();

            default:
                throw std::invalid_argument("Invalid game.");
        };
    }

    pokemon_pc_impl::pokemon_pc_impl(
        int game_id
    ): _native(nullptr),
       _our_mem(false),
       _game_id(game_id),
       _generation(pkmn::database::game_id_to_generation(game_id))
    {}

    std::string pokemon_pc_impl::get_game() {
        return pkmn::database::game_id_to_name(_game_id);
    }

    pkmn::pokemon_box::sptr pokemon_pc_impl::get_box(
        int index
    )
    {
        int num_boxes = get_num_boxes();
        pkmn::enforce_bounds("Box index", index, 0, (num_boxes-1));

        return _box_list.at(index);
    }

    const pkmn::pokemon_box_list_t& pokemon_pc_impl::as_vector() {
        return _box_list;
    }

    const std::vector<std::string>& pokemon_pc_impl::get_box_names() {
        _update_box_names();
        return _box_names;
    }

    void* pokemon_pc_impl::get_native() {
        if(_generation > 1) {
            _update_box_names();
        }

        return _native;
    }
}

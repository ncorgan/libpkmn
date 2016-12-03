/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_pc_impl.hpp"

#include "database/database_common.hpp"
#include "database/id_to_string.hpp"

#include <boost/format.hpp>

#include <stdexcept>

namespace pkmn {

    pokemon_pc_impl::pokemon_pc_impl(
        int game_id
    ): _game_id(game_id)
    {}

    std::string pokemon_pc_impl::get_game() {
        return pkmn::database::game_id_to_name(_game_id);
    }

    pkmn::pokemon_box::sptr pokemon_pc_impl::get_box(
        int index
    ) {
        int num_boxes = get_num_boxes();
        if(index > num_boxes or index > (num_boxes-1)) {
            throw std::out_of_range(
                      str(boost::format("index: valid values 0-%d")
                          % (num_boxes-1))
                  );
        }

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
        _update_box_names();
        return _native;
    }
}

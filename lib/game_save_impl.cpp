/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "game_save_impl.hpp"
#include "database/id_to_string.hpp"

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

namespace pkmn {

    game_save_impl::game_save_impl(
        const std::string &filepath
    ) {
        if(not fs::exists(filepath)) {
            throw std::invalid_argument("The given filepath does not exist.");
        }

        _filepath = fs::absolute(filepath).string();
    }

    std::string game_save_impl::get_filepath() {
        return _filepath;
    }

    std::string game_save_impl::get_game() {
        return pkmn::database::game_id_to_name(_game_id);
    }

    pkmn::item_bag::sptr game_save_impl::get_item_bag() {
        return _item_bag;
    }

    pkmn::item_list::sptr game_save_impl::get_item_pc() {
        return _item_pc;
    }
}

/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/database/item_entry.hpp>

namespace pkmn { namespace database {

    // TODO: database sptr

    item_entry::item_entry():
        _game_id(0),
        _generation(0),
        _version_group_id(0),
        _item_id(0),
        _item_index(0),
        _category_id(0),
        _item_list_id(0),
        _tmhm(false),
        _none(false),
        _invalid(true) {}

    item_entry::item_entry(
        int item_index,
        int game_id
    ):
        _game_id(0),
        _generation(0),
        _version_group_id(0),
        _item_id(0),
        _item_index(0),
        _category_id(0),
        _item_list_id(0),
        _tmhm(false),
        _none(false),
        _invalid(true)
    {
        (void)item_index;
        (void)game_id;
    }

    item_entry::item_entry(
        const std::string &item_name,
        const std::string &game_name
    ):
        _game_id(0),
        _generation(0),
        _version_group_id(0),
        _item_id(0),
        _item_index(0),
        _category_id(0),
        _item_list_id(0),
        _tmhm(false),
        _none(false),
        _invalid(true)
    {
        (void)item_name;
        (void)game_name;
    }

    std::string item_entry::get_name() const {
        return "";
    }

    std::string item_entry::get_game() const {
        return "";
    }

    std::string item_entry::get_category() const {
        return "";
    }

    std::string item_entry::get_pocket() const {
        return "";
    }

    std::string item_entry::get_description() const {
        return "";
    }

    int item_entry::get_cost() const {
        return 0;
    }

    bool item_entry::holdable() const {
        return false;
    }

    int item_entry::get_fling_power() const {
        return 0;
    }

    std::string item_entry::get_fling_effect() const {
        return "";
    }

    void item_entry::_set_vars(
        bool from_index
    ) {
        (void)from_index;
    }

}}

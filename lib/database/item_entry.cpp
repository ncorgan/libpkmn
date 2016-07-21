/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "database_common.hpp"
#include "id_to_string.hpp"

#include <pkmn/database/item_entry.hpp>

#include <boost/config.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>

#include <stdexcept>

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
        _invalid(true)
    {}

    item_entry::item_entry(
        int item_index,
        int game_id
    ):
        _game_id(game_id),
        _generation(0),
        _version_group_id(0),
        _item_id(0),
        _item_index(item_index),
        _category_id(0),
        _item_list_id(0),
        _tmhm(false),
        _none(false),
        _invalid(true)
    {}

    item_entry::item_entry(
        const std::string &item_name,
        const std::string &game_name
    ):
        _generation(0),
        _version_group_id(0),
        _item_index(0),
        _category_id(0),
        _item_list_id(0),
        _tmhm(false),
        _none(false),
        _invalid(true)
    {
        // Make sure item and game are valid
        _item_id = pkmn::database::item_name_to_id(
                       item_name
                   );
        _game_id = pkmn::database::game_name_to_id(
                       game_name
                   );
    }

    std::string item_entry::get_name() const {
        if(_none) {
            return "None";
        } else if(_invalid) {
            return str(boost::format("Invalid (0x%x)") % _item_id);
        }

        return pkmn::database::item_id_to_name(
                   _item_id
               );
    }

    std::string item_entry::get_game() const {
        return pkmn::database::game_id_to_name(
                   _game_id
               );
    }

    std::string item_entry::get_category() const {
        if(_none) {
            return "None";
        } else if(_invalid) {
            return "Unknown";
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM item_category_prose WHERE item_category_id=? "
            "AND local_language_id=9";

        return pkmn_db_query_bind1<std::string, int>(
                   query, _category_id
               );

        return "";
    }

    std::string item_entry::get_pocket() const {
        return "";
    }

    std::string item_entry::get_description() const {
        return "";
    }

    int item_entry::get_cost() const {
        if(_none or _invalid) {
            return 0;
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT cost FROM items WHERE id=?";

        return pkmn_db_query_bind1<int, int>(
                   query, _item_id
               );
    }

    bool item_entry::holdable() const {
        return false;
    }

    int item_entry::get_fling_power() const {
        // Fling was introduced in Generation IV
        if(_none or _invalid or _generation < 4) {
            return 0;
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT fling_power FROM items WHERE id=?";

        return pkmn_db_query_bind1<int, int>(
                   query, _item_id
               );
    }

    std::string item_entry::get_fling_effect() const {
        if(_none or _generation < 4) {
            return "None";
        } else if(_invalid) {
            return "Unknown";
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT effect FROM item_fling_effect_prose WHERE "
            "local_language_id=9 AND item_fling_effect_id="
            "(SELECT fling_effect_id FROM items WHERE id=?)";

        return pkmn_db_query_bind1<std::string, int>(
                   query, _item_id
               );
    }

    void item_entry::_set_vars(
        bool from_index
    ) {
        (void)from_index;
    }

}}

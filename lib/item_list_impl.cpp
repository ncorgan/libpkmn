/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "item_list_impl.hpp"
#include "database/database_common.hpp"
#include "database/id_to_string.hpp"

#include <boost/config.hpp>
#include <boost/format.hpp>

#include <stdexcept>

namespace pkmn {

    static pkmn::database::sptr _db;

    item_list::sptr item_list::make(
        const std::string &name,
        const std::string &game
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        (void)name;
        (void)game;
        return sptr();
    }

    item_list_impl::item_list_impl(
        int item_list_id,
        int game_id
    ): _item_list_id(item_list_id),
       _game_id(game_id),
       _version_group_id(pkmn::database::game_id_to_version_group(game_id)),
       _num_items(0)
    {
        static BOOST_CONSTEXPR const char* capacity_query = \
            "SELECT capacity FROM libpkmn_item_lists WHERE id=? AND "
            "version_group_id=?";

        _capacity = pkmn::database::query_db_bind2<int, int, int>(
                        _db, capacity_query, _item_list_id,
                        _version_group_id
                    );
    }

    std::string item_list_impl::get_name() {
        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM libpkmn_item_lists WHERE id=? AND "
            "version_group=?";

        return pkmn::database::query_db_bind2<std::string, int, int>(
                   _db, query, _item_list_id, _version_group_id
               );
    }

    std::string item_list_impl::get_game() {
        return pkmn::database::game_id_to_name(
                   _game_id
               );
    }

    int item_list_impl::get_capacity() {
        return _capacity;
    }

    int item_list_impl::get_num_items() {
        return _num_items;
    }

    const pkmn::item_slot_t& item_list_impl::at(
        int position
    ) {
        if(position < 0 or position >= _num_items) {
            throw std::out_of_range(
                      str(boost::format("position: valid range 0-%d") % (_num_items-1))
                  );
        }

        return _item_slots.at(position);
    }

    const pkmn::item_slots_t& item_list_impl::as_vector() {
        return _item_slots;
    }

    void* item_list_impl::get_native() {
        return _native;
    }
}

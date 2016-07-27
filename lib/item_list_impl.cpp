/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "item_list_impl.hpp"
#include "item_list_gbimpl.hpp"
#include "database/database_common.hpp"
#include "database/id_to_string.hpp"

#include <pksav/gen1/items.h>

#include <boost/config.hpp>
#include <boost/format.hpp>

#include <stdexcept>

namespace pkmn {

    static pkmn::database::sptr _db;

    // Game Boy list templated classes
    typedef item_list_gbimpl<pksav_gen1_item_bag_t> item_list_gen1_bagimpl;
    typedef item_list_gbimpl<pksav_gen1_item_pc_t>  item_list_gen1_pcimpl;

    item_list::sptr item_list::make(
        const std::string &name,
        const std::string &game
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* id_query = \
            "SELECT id FROM libpkmn_item_lists WHERE name=? AND "
            "version_group_id=(SELECT version_group_id FROM versions "
            "WHERE id=?)";

        int game_id = pkmn::database::game_name_to_id(game);
        int item_list_id = pkmn::database::query_db_bind2<int, const std::string&, int>(
                               _db, id_query, name, game_id
                           );
        int generation = pkmn::database::game_id_to_generation(game_id);

        switch(generation) {
            case 1:
                if(item_list_id == 1) {
                    return pkmn::make_shared<item_list_gen1_bagimpl>(
                               item_list_id, game_id, nullptr
                           );
                } else {
                    return pkmn::make_shared<item_list_gen1_pcimpl>(
                               item_list_id, game_id, nullptr
                           );
                }

            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
                throw std::runtime_error("Currently unimplemented.");
                break;

            default:
                throw std::invalid_argument("Invalid game.");
        }
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
        _item_slots.resize(_capacity);
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

    void item_list_impl::add(
        const std::string &item_name,
        int amount
    ) {
        // Input validation
        if(amount < 1 or amount > 99) {
            throw std::out_of_range("amount: valid range 1-99");
        }

        /*
         * Check if this item is already in the list. If so, add to
         * that amount. If not, see if there's room to add another
         * item.
         */
        int item_id = pkmn::database::item_name_to_id(item_name);
        for(int i = 0; i < _num_items; ++i) {
            if(_item_slots[i].item.get_item_id() == item_id) {
                if(_item_slots[i].amount == 99) {
                    throw std::runtime_error("Cannot add any more of this item.");
                } else if((_item_slots[i].amount + amount) > 99) {
                    int new_amount = _item_slots[i].amount + amount;
                    throw std::runtime_error(
                              str(boost::format("Can only add %d more items.") %
                                  (new_amount - amount))
                          );
                } else {
                    _item_slots[i].amount += amount;
                    _to_native(i);
                    return;
                }
            }
        }

        // At this point, we know the item isn't already in the pocket
        if(_num_items == _capacity) {
            throw std::runtime_error("Cannot add any new items.");
        } else {
            _item_slots[_num_items].item = pkmn::database::item_entry(
                                               item_name, get_game()
                                           );
            _item_slots[_num_items].amount = amount;
            _to_native(_num_items++);
        }
    }

    void item_list_impl::remove(
        const std::string &item_name,
        int amount
    ) {
        // Input validation
        if(amount < 1 or amount > 99) {
            throw std::out_of_range("amount: valid range 1-99");
        }

        /*
         * Check if this item is in the list. If so, remove that amount,
         * and if there are no more, remove the item from the list and
         * shift everything over.
         */
        int item_id = pkmn::database::item_name_to_id(item_name);
        for(int i = 0; i < _num_items; ++i) {
            if(_item_slots[i].item.get_item_id() == item_id) {
                if(_item_slots[i].amount < amount) {
                    throw std::runtime_error(
                              str(boost::format("Can only remove items.") %
                                  _item_slots[i].amount)
                          );
                } else {
                    _item_slots[i].amount -= amount;
                    if(_item_slots[i].amount == 0) {
                        _item_slots.erase(_item_slots.begin()+i);
                        _item_slots.resize(_capacity);
                        _num_items--;
                        _to_native();
                    }
                }
            }
        }

        // At this point, this item was never in the pocket
        throw std::runtime_error("Item not found.");
    }

    const pkmn::item_slots_t& item_list_impl::as_vector() {
        return _item_slots;
    }

    void* item_list_impl::get_native() {
        return _native;
    }
}

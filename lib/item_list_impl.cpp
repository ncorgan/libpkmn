/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "item_list_impl.hpp"
#include "item_list_gbimpl.hpp"
#include "item_list_gen2_tmhmimpl.hpp"
#include "item_list_gcnimpl.hpp"
#include "item_list_modernimpl.hpp"

#include "misc_common.hpp"

#include "database/database_common.hpp"
#include "database/id_to_string.hpp"

#include <pkmn/exception.hpp>

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

        int game_id = pkmn::database::game_name_to_id(game);
        int generation = pkmn::database::game_id_to_generation(game_id);
        int item_list_id = 0;
        int capacity = 0;

        static BOOST_CONSTEXPR const char* id_capacity_query = \
            "SELECT id,capacity FROM libpkmn_item_lists WHERE name=? AND "
            "version_group_id=(SELECT version_group_id FROM versions "
            "WHERE id=?)";

        SQLite::Statement stmt((*_db), id_capacity_query);
        stmt.bind(1, name);
        stmt.bind(2, game_id);
        if(stmt.executeStep()) {
            item_list_id = stmt.getColumn(0);
            capacity = stmt.getColumn(1);
        } else {
            throw std::invalid_argument("Invalid list.");
        }

        switch(generation) {
            case 1:
                switch(item_list_id) {
                    case 1:
                    case 3:
                        return pkmn::make_shared<item_list_gen1_bagimpl>(
                                   item_list_id, game_id, nullptr
                               );

                    case 2:
                    case 4:
                        return pkmn::make_shared<item_list_gen1_pcimpl>(
                                   item_list_id, game_id, nullptr
                               );

                    default:
                        throw std::runtime_error("Invalid list.");
                }

            case 2:
                switch(item_list_id) {
                    case 5:
                    case 10:
                        return pkmn::make_shared<item_list_gen2_item_pocketimpl>(
                                   item_list_id, game_id, nullptr
                               );

                    case 6:
                    case 11:
                        return pkmn::make_shared<item_list_gen2_ball_pocketimpl>(
                                   item_list_id, game_id, nullptr
                               );

                    case 7:
                    case 12:
                        return pkmn::make_shared<item_list_gen2_key_item_pocketimpl>(
                                   item_list_id, game_id, nullptr
                               );

                    case 8:
                    case 13:
                        return pkmn::make_shared<item_list_gen2_tmhmimpl>(
                                   item_list_id, game_id, nullptr
                               );

                    case 9:
                    case 14:
                        return pkmn::make_shared<item_list_gen2_pcimpl>(
                                   item_list_id, game_id, nullptr
                               );

                    default:
                        throw std::runtime_error("Invalid list.");
                }

            case 3:
                if(game_is_gamecube(game_id)) {
                    return pkmn::make_shared<item_list_gcnimpl>(
                               item_list_id, game_id, nullptr, capacity, false
                           );
                } else {
                    return pkmn::make_shared<item_list_modernimpl>(
                               item_list_id, game_id, nullptr, capacity, false
                           );
                }

            // Technically, this works for everything past this, but
            // we'll error out until their item_bag implementations
            // are done.
            case 4:
            case 5:
            case 6:
                throw pkmn::unimplemented_error();

            default:
                throw std::invalid_argument("Invalid game.");
        }
    }

    BOOST_STATIC_CONSTEXPR int RB_PC_ID = 2;
    BOOST_STATIC_CONSTEXPR int YELLOW_PC_ID = 4;
    BOOST_STATIC_CONSTEXPR int GS_PC_ID = 9;
    BOOST_STATIC_CONSTEXPR int CRYSTAL_PC_ID = 14;
    BOOST_STATIC_CONSTEXPR int RS_PC_ID = 20;
    BOOST_STATIC_CONSTEXPR int EMERALD_PC_ID = 26;
    BOOST_STATIC_CONSTEXPR int FRLG_PC_ID = 32;
    BOOST_STATIC_CONSTEXPR int COLO_PC_ID = 68;
    BOOST_STATIC_CONSTEXPR int XD_PC_ID = 75;

    static PKMN_CONSTEXPR_OR_INLINE bool ITEM_LIST_ID_IS_PC(
        int item_list_id
    ) {
        return (item_list_id == RB_PC_ID) or
               (item_list_id == YELLOW_PC_ID) or
               (item_list_id == GS_PC_ID) or
               (item_list_id == CRYSTAL_PC_ID) or
               (item_list_id == RS_PC_ID) or
               (item_list_id == EMERALD_PC_ID) or
               (item_list_id == FRLG_PC_ID) or
               (item_list_id == COLO_PC_ID) or
               (item_list_id == XD_PC_ID);
    }

    item_list_impl::item_list_impl(
        int item_list_id,
        int game_id
    ): item_list(),
       _item_list_id(item_list_id),
       _game_id(game_id),
       _version_group_id(pkmn::database::game_id_to_version_group(game_id)),
       _num_items(0),
       _pc(ITEM_LIST_ID_IS_PC(item_list_id))
    {
        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* capacity_query = \
            "SELECT capacity FROM libpkmn_item_lists WHERE id=? AND "
            "version_group_id=?";

        _capacity = pkmn::database::query_db_bind2<int, int, int>(
                        _db, capacity_query, _item_list_id,
                        _version_group_id
                    );

        _item_slots.resize(_capacity);
        for(int i = 0; i < _capacity; ++i) {
            _item_slots[i].item = "None";
            _item_slots[i].amount = 0;
        }
    }

    std::string item_list_impl::get_name() {
        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM libpkmn_item_lists WHERE id=? AND "
            "version_group_id=?";

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

    const pkmn::item_slot& item_list_impl::at(
        int position
    ) {
        if(position < 0 or position >= _capacity) {
            pkmn::throw_out_of_range("position", 0, (_capacity-1));
        }

        return _item_slots.at(position);
    }

    void item_list_impl::add(
        const std::string &item_name,
        int amount
    )
    {
        // Input validation
        if(amount < 1 or amount > 99)
        {
            pkmn::throw_out_of_range("amount", 1, 99);
        }

        /*
         * Check if this item is already in the list. If so, add to
         * that amount. If not, see if there's room to add another
         * item.
         */
        for(int i = 0; i < _num_items; ++i)
        {
            if(_item_slots[i].item == item_name)
            {
                if(_item_slots[i].amount == 99)
                {
                    throw std::runtime_error("Cannot add any more of this item.");
                } else if((_item_slots[i].amount + amount) > 99)
                {
                    int new_amount = _item_slots[i].amount + amount;
                    throw std::runtime_error(
                              str(boost::format("Can only add %d more items.") %
                                  (new_amount - amount))
                          );
                }
                else
                {
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
            // Confirm the item can be placed in this pocket
            pkmn::database::item_entry entry(
                item_name, get_game()
            );
            if(not _pc and entry.get_item_list_id() != _item_list_id) {
                throw std::invalid_argument(
                          str(boost::format("This item belongs in the \"%s\" pocket.") %
                              entry.get_pocket())
                      );
            }

            _item_slots[_num_items].item = item_name;
            _item_slots[_num_items].amount = amount;
            _to_native(_num_items++);
        }
    }

    void item_list_impl::remove(
        const std::string &item_name,
        int amount
    )
    {
        // Input validation
        if(amount < 1 or amount > 99)
        {
            pkmn::throw_out_of_range("amount", 1, 99);
        }

        /*
         * Check if this item is in the list. If so, remove that amount,
         * and if there are no more, remove the item from the list and
         * shift everything over.
         */
        for(int i = 0; i < _num_items; ++i)
        {
            if(_item_slots[i].item == item_name)
            {
                if(_item_slots[i].amount < amount)
                {
                    throw std::runtime_error(
                              str(boost::format("Can only remove %d items.") %
                                  _item_slots[i].amount)
                          );
                }
                else
                {
                    _item_slots[i].amount -= amount;
                    if(_item_slots[i].amount == 0)
                    {
                        _item_slots.erase(_item_slots.begin()+i);
                        _item_slots.resize(_capacity);
                        _num_items--;
                        _to_native();
                    }
                    else
                    {
                        _to_native(i);
                    }
                    return;
                }
            }
        }

        // At this point, this item was never in the pocket
        throw std::invalid_argument("Item not found.");
    }

    void item_list_impl::move(
        int old_position,
        int new_position
    ) {
        if(old_position < 0 or old_position >= _num_items or
           new_position < 0 or new_position >= _num_items)
        {
            throw std::range_error("Cannot move an item outside of the list.");
        } else if(old_position == new_position) {
            throw std::invalid_argument("Positions cannot match.");
        }

        pkmn::item_slot temp = _item_slots[old_position];
        _item_slots.erase(_item_slots.begin()+old_position);
        _item_slots.insert(_item_slots.begin()+new_position, temp);
        _to_native();
    }

    void item_list_impl::set_item(
        int position,
        const std::string& item_name,
        int amount
    )
    {
        // Input validation.
        int end_boundary = std::min<int>(_num_items, _capacity-1);
        if(position < 0 or position > end_boundary)
        {
            pkmn::throw_out_of_range("position", 0, end_boundary);
        }
        if(item_name == "None")
        {
            if(amount != 0)
            {
                throw std::invalid_argument("\"None\" entries must have an amount of 0.");
            }
            else if(amount < 0 or amount > 99)
            {
                pkmn::throw_out_of_range("amount", 0, 99);
            }
        }
        else
        {
            pkmn::database::item_entry entry(item_name, get_game());
            if(get_name() != "PC" and item_name != "None" and entry.get_pocket() != get_name())
            {
                throw std::invalid_argument("This item does not belong in this pocket.");
            }
            if(amount < 1 or amount > 99)
            {
                pkmn::throw_out_of_range("amount", 1, 99);
            }
            for(int i = 0; i < _num_items; ++i)
            {
                if((_item_slots[i].item == item_name) && (i != position))
                {
                    std::string err_msg = "This item is already present in slot ";
                    err_msg.append(std::to_string(i));
                    err_msg.append(".");

                    throw std::invalid_argument(err_msg.c_str());
                }
            }
        }

        _item_slots[position].item = item_name;
        _item_slots[position].amount = amount;
        if(item_name == "None" and position < end_boundary)
        {
            _item_slots.erase(_item_slots.begin()+position);
            _item_slots.emplace_back(pkmn::item_slot("None", 0));
            --_num_items;
        }
        else
        {
            ++_num_items;
        }

        _to_native();
    }

    const pkmn::item_slots_t& item_list_impl::as_vector() {
        return _item_slots;
    }

    /*
     * Veekun's database does not distinguish berries from other healing items,
     * but they go in separate pockets in every game past Generation II, so this
     * overrides the database query.
     */
    BOOST_STATIC_CONSTEXPR int BERRY_LIST_IDS[] = {
        -1, // None
        -1, // Red/Blue
        -1, // Yellow
        5,  // Gold/Silver
        10, // Crystal
        18, // Ruby/Sapphire
        24, // Emerald
        30, // FireRed/LeafGreen
        37, // Diamond/Pearl
        45, // Platinum
        53, // HeartGold/SoulSilver
        60, // Black/White
        66, // Colosseum
        73, // XD
        79, // Black 2/White 2
        84, // X/Y
        89  // Omega Ruby/Alpha Sapphire
    };

    /*
     * TODO: if PC, all items valid except Berry Pouch, TM Case
     */
    const std::vector<std::string>& item_list_impl::get_valid_items() {
        if(_valid_items.size() == 0) {
            if(std::find(BERRY_LIST_IDS, BERRY_LIST_IDS+17, _item_list_id) != BERRY_LIST_IDS+17) {
                static BOOST_CONSTEXPR const char* berry_list_query = \
                    "SELECT DISTINCT item_names.name FROM item_names JOIN item_game_indices ON "
                    "(item_names.item_id=item_game_indices.item_id) WHERE item_game_indices.generation_id=? "
                    "AND item_names.name LIKE '%Berry'";

                pkmn::database::query_db_list_bind1<std::string, int>(
                    _db, berry_list_query, _valid_items,
                    pkmn::database::game_id_to_generation(_game_id)
                );
            } else {
                pkmn::database::_get_item_list(
                    _valid_items,
                    ((get_name() == "PC") ? -1 : _item_list_id),
                    _game_id
                );
            }
        }

        return _valid_items;
    }

    void* item_list_impl::get_native() {
        boost::mutex::scoped_lock scoped_lock(_mem_mutex);

        return _native;
    }
}

/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "item_list_gen2_keyitemimpl.hpp"

#include <pksav/gen2/items.h>

#include <pkmn/database/item_entry.hpp>
#include <pkmn/exception.hpp>

#include <boost/thread/lock_guard.hpp>

#include <cstdio>
#include <cstring>
#include <stdexcept>

#define NATIVE_RCAST (reinterpret_cast<struct pksav_gen2_key_item_pocket*>(_p_native))

namespace pkmn {

    item_list_gen2_keyitemimpl::item_list_gen2_keyitemimpl(
        int item_list_id,
        int game_id,
        void* ptr
    ): item_list_impl(item_list_id, game_id)
    {
        if(ptr)
        {
            _p_native = ptr;
            _is_our_mem = false;

            _from_native();
        }
        else
        {
            _p_native = reinterpret_cast<void*>(new struct pksav_gen2_key_item_pocket);
            std::memset(_p_native, 0, sizeof(struct pksav_gen2_key_item_pocket));
            NATIVE_RCAST->terminator = 0xFF;

            _is_our_mem = true;
        }
    }

    item_list_gen2_keyitemimpl::~item_list_gen2_keyitemimpl()
    {
        if(_is_our_mem)
        {
            delete NATIVE_RCAST;
        }
    }

    void item_list_gen2_keyitemimpl::add(
        const std::string& item_name,
        int amount
    )
    {
        if(amount != 1)
        {
            throw std::out_of_range("Amount: valid value 1");
        }

        pkmn::database::item_entry item(item_name, get_game());
        if(item.get_pocket() != get_name())
        {
            throw std::invalid_argument("This item is not valid for this list.");
        }

        boost::lock_guard<item_list_gen2_keyitemimpl> lock(*this);

        /*
         * Check if this item is already in the list. If so, it cannot
         * be added. If not, see if there's room to add another
         * item.
         */
        for(int item_index = 0; item_index < _num_items; ++item_index)
        {
            if(_item_slots[item_index].item == item_name)
            {
                throw std::invalid_argument("This item is already in this pocket.");
            }
        }

        if(_num_items < _capacity)
        {
            _item_slots[_num_items].item = item_name;
            _item_slots[_num_items].amount = amount;
            _to_native(_num_items++);
        }
        else
        {
            throw std::runtime_error("Cannot add any more items.");
        }
    }

    void item_list_gen2_keyitemimpl::remove(
        const std::string& item_name,
        int amount
    )
    {
        if(amount != 1)
        {
            throw std::out_of_range("Amount: valid value 1");
        }

        pkmn::database::item_entry item(item_name, get_game());
        if(item.get_pocket() != get_name())
        {
            throw std::invalid_argument("This item is not valid for this list.");
        }

        boost::lock_guard<item_list_gen2_keyitemimpl> lock(*this);

        /*
         * Check if this item is in the list. If so, remove that amount,
         * and if there are no more, remove the item from the list and
         * shift everything over.
         */
        for(int item_index = 0; item_index < _num_items; ++item_index)
        {
            if(_item_slots[item_index].item == item_name)
            {
                _item_slots.erase(_item_slots.begin()+item_index);
                _item_slots.resize(_capacity);
                --_num_items;
                _to_native(item_index);

                break;
            }
        }
    }

    void item_list_gen2_keyitemimpl::set_item(
        int position,
        const std::string& item_name,
        int amount
    )
    {
        // Input validation.
        int end_boundary = std::min<int>(_num_items, _capacity-1);
        pkmn::enforce_bounds("Position", position, 0, end_boundary);

        boost::lock_guard<item_list_gen2_keyitemimpl> lock(*this);

        if(item_name == "None")
        {
            if(amount != 0)
            {
                throw std::invalid_argument("\"None\" entries must have an amount of 0.");
            }
            else
            {
                pkmn::enforce_bounds("Amount", amount, 0, 99);
            }
        }
        else
        {
            pkmn::database::item_entry entry(item_name, get_game());
            if(entry.get_pocket() != get_name())
            {
                throw std::invalid_argument("This item does not belong in this pocket.");
            }

            if(amount != 1)
            {
                throw std::out_of_range("Amount: valid value 1");
            }

            for(int item_index = 0; item_index < _num_items; ++item_index)
            {
                if((_item_slots[item_index].item == item_name) && (item_index != position))
                {
                    // TODO: all error messages should have what slot item is in
                    std::string err_msg = "This item is already present in slot ";
                    err_msg.append(std::to_string(item_index));
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

    void item_list_gen2_keyitemimpl::_from_native(
        int index
    )
    {
        boost::lock_guard<item_list_gen2_keyitemimpl> lock(*this);

        if(index >= 0)
        {
            _item_slots[index].item = pkmn::database::item_entry(
                                          NATIVE_RCAST->item_indices[index],
                                          _game_id
                                      ).get_name();
            _item_slots[index].amount = (NATIVE_RCAST->item_indices[index] > 0) ? 1 : 0;
        }
        else
        {
            for(int item_index = 0; item_index < _capacity; ++item_index)
            {
                _item_slots[item_index].item = pkmn::database::item_entry(
                                                   NATIVE_RCAST->item_indices[item_index],
                                                   _game_id
                                               ).get_name();
                _item_slots[item_index].amount = (NATIVE_RCAST->item_indices[item_index] > 0) ? 1 : 0;
            }
        }

        _num_items = NATIVE_RCAST->count;
    }

    void item_list_gen2_keyitemimpl::_to_native(
        int index
    )
    {
        boost::lock_guard<item_list_gen2_keyitemimpl> lock(*this);

        if(index >= 0)
        {
            NATIVE_RCAST->item_indices[index] = uint8_t(pkmn::database::item_entry(
                                                            _item_slots[index].item,
                                                            get_game()
                                                        ).get_item_index());
        }
        else
        {
            for(int item_index = 0; item_index < _capacity; ++item_index)
            {
                NATIVE_RCAST->item_indices[item_index] = uint8_t(pkmn::database::item_entry(
                                                                     _item_slots[item_index].item,
                                                                     get_game()
                                                                 ).get_item_index());
            }
        }

        NATIVE_RCAST->count = uint8_t(_num_items);
    }
}

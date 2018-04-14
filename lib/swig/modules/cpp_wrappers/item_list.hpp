/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_WRAPPERS_ITEM_LIST_HPP
#define CPP_WRAPPERS_ITEM_LIST_HPP

#include <pkmn/config.hpp>
#include <pkmn/exception.hpp>
#include <pkmn/item_list.hpp>

#if defined(SWIGPYTHON) && SWIG_VERSION < 0x030008
#    include <boost/locale/encoding_utf.hpp>
#endif

#include "item_slot.hpp"

namespace pkmn { namespace swig {

    /*
     * This class is a thin wrapper around pkmn::item_list::sptr and
     * will be what some SWIG wrappers will use instead of the class
     * itself. It will allow syntax like the following to be used:
     *
     * itemPocket[0].item = "Potion"
     */
    class item_list
    {
        public:
            item_list():
                _item_list(nullptr)
            {}

            item_list(
                const pkmn::item_list::sptr& cpp_item_list
            ): _item_list(cpp_item_list)
            {
            }

/*
 * SWIG 3.0.8 introduced the SWIG_PYTHON_2_UNICODE macro, which allows the
 * Python 2 "unicode" type to be converted to a char* or std::string. There's
 * no way for a SWIG project to bring this in, so we need this ugly workaround
 * when using earlier verisons of SWIG.
 */
#if defined(SWIGPYTHON) && SWIG_VERSION < 0x030008
            item_list(
                const std::wstring& name,
                const std::wstring& game
            ): _item_list(pkmn::item_list::make(
                   boost::locale::conv::utf_to_utf<char>(name),
                   boost::locale::conv::utf_to_utf<char>(game)
                   ))
            {
            }
#else
            item_list(
                const std::string& name,
                const std::string& game
            ): _item_list(pkmn::item_list::make(name, game))
            {
            }
#endif

            inline std::string get_name()
            {
                return _item_list->get_name();
            }

            inline std::string get_game()
            {
                return _item_list->get_game();
            }

            inline int get_capacity()
            {
                return _item_list->get_capacity();
            }

            inline int get_num_items()
            {
                return _item_list->get_num_items();
            }

            inline item_slot at(
                int position
            )
            {
#ifdef SWIGLUA
                pkmn::enforce_bounds(
                    "Position",
                    position,
                    1,
                    get_capacity()
                );

                return item_slot(_item_list, position-1);
#else
                return item_slot(_item_list, position);
#endif
            }

            inline void add(
                const std::string& item,
                int amount
            )
            {
                _item_list->add(item, amount);
            }

            inline void remove(
                const std::string& item,
                int amount
            )
            {
                _item_list->remove(item, amount);
            }

            inline void move(
                int old_position,
                int new_position
            )
            {
#ifdef SWIGLUA
                pkmn::enforce_bounds(
                    "Old position",
                    old_position,
                    1,
                    get_num_items()
                );
                pkmn::enforce_bounds(
                    "New position",
                    new_position,
                    1,
                    get_num_items()
                );

                _item_list->move(old_position-1, new_position-1);
#else
                _item_list->move(old_position, new_position);
#endif
            }

            // Copy the vector, since the const in the reference
            // is casted away.
            inline std::vector<std::string> get_valid_items()
            {
                return _item_list->get_valid_items();
            }

#ifdef SWIGCSHARP
            inline uintmax_t cptr()
            {
                return uintmax_t(_item_list.get());
            }
#else
            inline bool operator==(const item_list& rhs) const
            {
                return (_item_list == rhs._item_list);
            }

            inline bool operator!=(const item_list& rhs) const
            {
                return !operator==(rhs);
            }
#endif

        private:
            pkmn::item_list::sptr _item_list;
    };

}}

#endif /* CPP_WRAPPERS_ITEM_LIST_HPP */

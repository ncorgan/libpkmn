/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_DATABASE_ITEM_ENTRY_HPP
#define PKMN_DATABASE_ITEM_ENTRY_HPP

#include <pkmn/config.hpp>

#include <string>

namespace pkmn { namespace database {

    class PKMN_API item_entry {
        public:
            item_entry();

            #ifndef __DOXYGEN__
            item_entry(
                int item_index,
                int game_id
            );
            #endif

            item_entry(
                const std::string &item_name,
                const std::string &game_name
            );

            std::string get_name() const;

            std::string get_game() const;

            std::string get_category() const;

            std::string get_pocket() const;

            std::string get_description() const;

            int get_cost() const;

            bool holdable() const;

            int get_fling_power() const;

            std::string get_fling_effect() const;

            #ifndef __DOXYGEN__
            PKMN_INLINE int get_item_id() const {
                return _item_id;
            }

            PKMN_INLINE int get_item_index() const {
                return _item_index;
            }

            PKMN_INLINE int get_game_id() const {
                return _game_id;
            }

            PKMN_INLINE int get_item_list_id() const {
                return _item_list_id;
            }
            #endif

            //! Equality check between two item entries
            PKMN_INLINE bool operator==(const item_entry &rhs) const {
                return ((this->_game_id == rhs._game_id) and
                        (this->_item_id == rhs._item_id));
            }

            //! Inequality check between two item entries
            PKMN_INLINE bool operator!=(const item_entry &rhs) const {
                return ((this->_game_id != rhs._game_id) or
                        (this->_item_id != rhs._item_id));
            }

        private:
            int _item_id, _item_index, _category_id, _item_list_id, _tmhm;
            int _game_id, _generation, _version_group_id;
            bool _none, _invalid;

            void _set_vars();
    };
}}
#endif /* PKMN_DATABASE_ITEM_ENTRY_HPP */

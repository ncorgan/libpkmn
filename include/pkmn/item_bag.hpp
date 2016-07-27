/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_ITEM_BAG_HPP
#define PKMN_ITEM_BAG_HPP

#include <pkmn/config.hpp>
#include <pkmn/item_list.hpp>
#include <pkmn/types/shared_ptr.hpp>

#include <string>
#include <vector>

namespace pkmn {

    class PKMN_API item_bag {
        public:
            typedef pkmn::shared_ptr<item_bag> sptr;

            static sptr make(
                const std::string &game
            );

            #ifndef __DOXYGEN__
            item_bag() {};
            virtual ~item_bag() {};
            #endif

            virtual std::string get_game() = 0;

            virtual pkmn::item_list::sptr get_pocket(
                const std::string &name
            ) = 0;

            virtual const pkmn::item_pockets_t& get_pockets() = 0;

            virtual const std::vector<std::string>& get_pocket_names() = 0;

            virtual void add(
                const std::string &item_name,
                int amount
            ) = 0;

            virtual void remove(
                const std::string &item_name,
                int amount
            ) = 0;

            #ifndef __DOXYGEN__
            virtual void* get_native() = 0;
            #endif
    };

}

#endif /* PKMN_ITEM_BAG_HPP */

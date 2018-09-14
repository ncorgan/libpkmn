/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "item_bag_impl.hpp"
#include "item_bag_gen1impl.hpp"
#include "item_bag_gen2impl.hpp"
#include "item_bag_gbaimpl.hpp"
#include "item_bag_gcnimpl.hpp"

#include "database/database_common.hpp"
#include "database/enum_conversions.hpp"
#include "database/id_to_string.hpp"

#include "exception_internal.hpp"

#include "types/mutex_helpers.hpp"

#include "utils/misc.hpp"

#include <pkmn/exception.hpp>

#include <boost/config.hpp>

#include <stdexcept>

namespace pkmn {

    item_bag::sptr item_bag::make(pkmn::e_game game)
    {
        int game_id = pkmn::database::game_enum_to_id(game);
        int generation = pkmn::database::game_id_to_generation(game_id);

        switch(generation)
        {
            case 1:
                return std::make_shared<item_bag_gen1impl>(
                           game_id, nullptr
                       );

            case 2:
                return std::make_shared<item_bag_gen2impl>(
                           game_id, nullptr
                       );

            case 3:
                if(game_is_gamecube(game_id)) {
                    return std::make_shared<item_bag_gcnimpl>(
                               game_id, nullptr
                           );
                } else {
                    return std::make_shared<item_bag_gbaimpl>(
                               game_id, nullptr
                           );
                }

            case 4:
            case 5:
            case 6:
                throw pkmn::unimplemented_error();

            default:
                throw std::runtime_error("Invalid game.");
        }
    }

    item_bag_impl::item_bag_impl(
        int game_id
    ): item_bag(),
       _game_id(game_id),
       _generation(pkmn::database::game_id_to_generation(game_id)),
       _version_group_id(pkmn::database::game_id_to_version_group(game_id)),
       _p_native(nullptr)
    {
        // Populate pocket name vector
        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM libpkmn_item_lists WHERE version_group_id=? "
            "AND name != 'PC'";

        pkmn::database::query_db_list_bind1<std::string, int>(
            query, _item_pocket_names, _version_group_id
        );
    }

    pkmn::e_game item_bag_impl::get_game()
    {
        return pkmn::database::game_id_to_enum(_game_id);
    }

    const pkmn::item_list::sptr& item_bag_impl::get_pocket(
        const std::string& name
    )
    {
        pkmn::enforce_value_in_map_keys(
            "Pocket name",
            name,
            _item_pockets
        );

        return _item_pockets.at(name);
    }

    const pkmn::item_pockets_t& item_bag_impl::get_pockets()
    {
        _to_native();

        return _item_pockets;
    }

    const std::vector<std::string>& item_bag_impl::get_pocket_names()
    {
        return _item_pocket_names;
    }

    void item_bag_impl::add(
        pkmn::e_item item,
        int amount
    )
    {
        pkmn::lock_guard<item_bag_impl> lock(*this);

        std::string pocket_name = _get_pocket_name(item);
        _item_pockets.at(pocket_name)->add(item, amount);

        _to_native();
    }

    void item_bag_impl::remove(
        pkmn::e_item item,
        int amount
    )
    {
        pkmn::lock_guard<item_bag_impl> lock(*this);

        std::string pocket_name = _get_pocket_name(item);
        _item_pockets.at(pocket_name)->remove(item, amount);

        _to_native();
    }

    void* item_bag_impl::get_native()
    {
        pkmn::lock_guard<item_bag_impl> lock(*this);

        _to_native();

        return _p_native;
    }

    // Skips creating item entry
    std::string item_bag_impl::_get_pocket_name(pkmn::e_item item)
    {
        std::string pocket_name;

        if(pkmn::database::is_item_enum_berry(item) && (_generation > 2))
        {
            static const int VERSION_GROUP_FRLG = 7;

            pocket_name = (_version_group_id == VERSION_GROUP_FRLG) ? "Berry Pouch"
                                                                    : "Berries";
        }
        else
        {
            static const std::string list_name_query =
                "SELECT name FROM libpkmn_item_lists WHERE id="
                "(SELECT libpkmn_list_id FROM veekun_pocket_to_libpkmn_list "
                "WHERE version_group_id=? AND veekun_pocket_id=(SELECT "
                "pocket_id FROM item_categories WHERE id=(SELECT category_id "
                "FROM items WHERE id=?)))";

            pocket_name = pkmn::database::query_db_bind2<std::string, int, int>(
                              list_name_query.c_str(),
                              _version_group_id,
                              static_cast<int>(item)
                          );
        }

        return pocket_name;
    }

}

/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "item_bag_impl.hpp"
#include "item_bag_gen1impl.hpp"
#include "database/database_common.hpp"
#include "database/id_to_string.hpp"

#include <boost/config.hpp>

#include <stdexcept>

namespace pkmn {

    static pkmn::database::sptr _db;

    item_bag::sptr item_bag::make(
        const std::string &game
    ) {
        int game_id = pkmn::database::game_name_to_id(
                          game
                      );
        int generation = pkmn::database::game_id_to_generation(
                             game_id
                         );

        switch(generation) {
            case 1:
                return pkmn::make_shared<item_bag_gen1impl>(
                           game_id, nullptr
                       );

            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
                throw std::runtime_error("Currently unimplemented.");

            default:
                throw std::runtime_error("Invalid game.");
        }
    }

    item_bag_impl::item_bag_impl(
        int game_id
    ): item_bag(),
       _game_id(game_id)
    {
        // Connect to database
        pkmn::database::get_connection(_db);

        // Populate pocket name vector
        int version_group_id = pkmn::database::game_id_to_version_group(game_id);
        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM libpkmn_item_lists WHERE version_group_id=? "
            "AND name != 'PC'";

        pkmn::database::query_db_list_bind1<std::string, int>(
            _db, query, _item_pocket_names, version_group_id
        );
    }

    std::string item_bag_impl::get_game() {
        return pkmn::database::game_id_to_name(
                   _game_id
               );
    }

    pkmn::item_list::sptr item_bag_impl::get_pocket(
        const std::string &name
    ) {
        try {
            return _item_pockets.at(name);
        } catch(const std::exception&) {
            throw std::invalid_argument("Invalid pocket name.");
        }
    }

    const pkmn::item_pockets_t& item_bag_impl::get_pockets() {
        return _item_pockets;
    }

    const std::vector<std::string>& item_bag_impl::get_pocket_names() {
        return _item_pocket_names;
    }

    static BOOST_CONSTEXPR const char* list_name_query = \
        "SELECT name FROM libpkmn_pocket_names WHERE id="
        "(SELECT libpkmn_list_id FROM veekun_pocket_to_libpkmn_list "
        "WHERE version_group_id=? AND veekun_pocket_id=(SELECT "
        "pocket_id FROM item_categories WHERE id=(SELECT category_id "
        "FROM items WHERE id=(SELECT item_id FROM item_names WHERE "
        "name=?))))";

    void item_bag_impl::add(
        const std::string &item_name,
        int amount
    ) {
        int version_group_id = pkmn::database::game_id_to_version_group(
                                   _game_id
                               );

        std::string pocket_name = pkmn::database::query_db_bind2<std::string, int, const std::string&>(
                                      _db, list_name_query, version_group_id, item_name
                                  );

        _item_pockets.at(pocket_name)->add(item_name, amount);
    }

    void item_bag_impl::remove(
        const std::string &item_name,
        int amount
    ) {
        int version_group_id = pkmn::database::game_id_to_version_group(
                                   _game_id
                               );

        std::string pocket_name = pkmn::database::query_db_bind2<std::string, int, const std::string&>(
                                      _db, list_name_query, version_group_id, item_name
                                  );

        _item_pockets.at(pocket_name)->remove(item_name, amount);
    }

    void* item_bag_impl::get_native() {
        return _native;
    }

}

/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#define NATIVE_RCAST    reinterpret_cast<pksav_gba_item_storage_t*>(_native)
#define RS_STORAGE      NATIVE_RCAST->rs
#define EMERALD_STORAGE NATIVE_RCAST->emerald
#define FRLG_STORAGE    NATIVE_RCAST->frlg

#include "item_bag_gbaimpl.hpp"
#include "item_list_modernimpl.hpp"

#include "database/database_common.hpp"

#include <pkmn/types/shared_ptr.hpp>

#include <cstring>

namespace pkmn {

    item_bag_gbaimpl::item_bag_gbaimpl(
        int game_id,
        void* ptr
    ): item_bag_impl(game_id)
    {
        if(ptr) {
            _native = ptr;
            _our_mem = false;
        } else {
            _native = reinterpret_cast<void*>(new pksav_gba_item_storage_t);
            std::memset(_native, 0, sizeof(pksav_gba_item_storage_t));
            _our_mem = true;
        }

        _set_ptrs();
    }

    item_bag_gbaimpl::item_bag_gbaimpl(
        int game_id,
        const pksav_gba_item_storage_t &item_storage
    ): item_bag_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new pksav_gba_item_storage_t);
        *NATIVE_RCAST = item_storage;
        _our_mem = true;

        _set_ptrs();
    }

    item_bag_gbaimpl::~item_bag_gbaimpl() {
        item_bag_scoped_lock lock(this);

        if(_our_mem) {
            delete NATIVE_RCAST;
        }
    }

    void item_bag_gbaimpl::_set_ptrs() {

        BOOST_STATIC_CONSTEXPR int VERSION_GROUP_RS = 5;
        BOOST_STATIC_CONSTEXPR int VERSION_GROUP_EMERALD = 6;
        BOOST_STATIC_CONSTEXPR int VERSION_GROUP_FRLG = 7;

        BOOST_STATIC_CONSTEXPR int ITEM_POCKET_IDS[]     = {15,21,27};
        BOOST_STATIC_CONSTEXPR int BALL_POCKET_IDS[]     = {16,22,28};
        BOOST_STATIC_CONSTEXPR int TMHM_POCKET_IDS[]     = {17,23,29};
        BOOST_STATIC_CONSTEXPR int BERRY_POCKET_IDS[]    = {18,24,30};
        BOOST_STATIC_CONSTEXPR int KEY_ITEM_POCKET_IDS[] = {19,25,31};

        int version_group = pkmn::database::game_id_to_version_group(_game_id);
        if(version_group < VERSION_GROUP_RS or version_group > VERSION_GROUP_FRLG) {
            throw std::runtime_error("Invalid version group.");
        }

        int index = version_group - VERSION_GROUP_RS;

        // Set up differences between versions
        std::string berry_pocket = (version_group == VERSION_GROUP_FRLG) ? "Berry Pouch"
                                                                         : "Berries";
        std::string tmhm_pocket = (version_group == VERSION_GROUP_FRLG) ? "TM Case"
                                                                        : "TMs & HMs";

        switch(version_group) {
            case VERSION_GROUP_RS:
                _item_pockets["Items"] = pkmn::make_shared<item_list_modernimpl>(
                                             ITEM_POCKET_IDS[index], _game_id, RS_STORAGE.items,
                                             sizeof(RS_STORAGE.items)/sizeof(pksav_item_t),
                                             false
                                         );
                _item_pockets["Poké Balls"] = pkmn::make_shared<item_list_modernimpl>(
                                                  BALL_POCKET_IDS[index], _game_id, RS_STORAGE.balls,
                                                  sizeof(RS_STORAGE.balls)/sizeof(pksav_item_t),
                                                  false
                                              );
                _item_pockets[tmhm_pocket] = pkmn::make_shared<item_list_modernimpl>(
                                                 TMHM_POCKET_IDS[index], _game_id, RS_STORAGE.tms_hms,
                                                 sizeof(RS_STORAGE.tms_hms)/sizeof(pksav_item_t),
                                                 false
                                             );
                _item_pockets[berry_pocket] = pkmn::make_shared<item_list_modernimpl>(
                                                  BERRY_POCKET_IDS[index], _game_id, RS_STORAGE.berries,
                                                  sizeof(RS_STORAGE.berries)/sizeof(pksav_item_t),
                                                  false
                                              );
                _item_pockets["Key Items"] = pkmn::make_shared<item_list_modernimpl>(
                                                 KEY_ITEM_POCKET_IDS[index], _game_id, RS_STORAGE.key_items,
                                                 sizeof(RS_STORAGE.key_items)/sizeof(pksav_item_t),
                                                 false
                                             );
                break;

            case VERSION_GROUP_EMERALD:
                _item_pockets["Items"] = pkmn::make_shared<item_list_modernimpl>(
                                             ITEM_POCKET_IDS[index], _game_id, EMERALD_STORAGE.items,
                                             sizeof(EMERALD_STORAGE.items)/sizeof(pksav_item_t),
                                             false
                                         );
                _item_pockets["Poké Balls"] = pkmn::make_shared<item_list_modernimpl>(
                                                  BALL_POCKET_IDS[index], _game_id, EMERALD_STORAGE.balls,
                                                  sizeof(EMERALD_STORAGE.balls)/sizeof(pksav_item_t),
                                                  false
                                              );
                _item_pockets[tmhm_pocket] = pkmn::make_shared<item_list_modernimpl>(
                                                 TMHM_POCKET_IDS[index], _game_id, EMERALD_STORAGE.tms_hms,
                                                 sizeof(EMERALD_STORAGE.tms_hms)/sizeof(pksav_item_t),
                                                 false
                                             );
                _item_pockets[berry_pocket] = pkmn::make_shared<item_list_modernimpl>(
                                                  BERRY_POCKET_IDS[index], _game_id, EMERALD_STORAGE.berries,
                                                  sizeof(EMERALD_STORAGE.berries)/sizeof(pksav_item_t),
                                                  false
                                              );
                _item_pockets["Key Items"] = pkmn::make_shared<item_list_modernimpl>(
                                                 KEY_ITEM_POCKET_IDS[index], _game_id, EMERALD_STORAGE.key_items,
                                                 sizeof(EMERALD_STORAGE.key_items)/sizeof(pksav_item_t),
                                                 false
                                             );
                break;

            case VERSION_GROUP_FRLG:
                _item_pockets["Items"] = pkmn::make_shared<item_list_modernimpl>(
                                             ITEM_POCKET_IDS[index], _game_id, FRLG_STORAGE.items,
                                             sizeof(FRLG_STORAGE.items)/sizeof(pksav_item_t),
                                             false
                                         );
                _item_pockets["Poké Balls"] = pkmn::make_shared<item_list_modernimpl>(
                                                  BALL_POCKET_IDS[index], _game_id, FRLG_STORAGE.balls,
                                                  sizeof(FRLG_STORAGE.balls)/sizeof(pksav_item_t),
                                                  false
                                              );
                _item_pockets[tmhm_pocket] = pkmn::make_shared<item_list_modernimpl>(
                                                 TMHM_POCKET_IDS[index], _game_id, FRLG_STORAGE.tms_hms,
                                                 sizeof(FRLG_STORAGE.tms_hms)/sizeof(pksav_item_t),
                                                 false
                                             );
                _item_pockets[berry_pocket] = pkmn::make_shared<item_list_modernimpl>(
                                                  BERRY_POCKET_IDS[index], _game_id, FRLG_STORAGE.berries,
                                                  sizeof(FRLG_STORAGE.berries)/sizeof(pksav_item_t),
                                                  false
                                              );
                _item_pockets["Key Items"] = pkmn::make_shared<item_list_modernimpl>(
                                                 KEY_ITEM_POCKET_IDS[index], _game_id, FRLG_STORAGE.key_items,
                                                 sizeof(FRLG_STORAGE.key_items)/sizeof(pksav_item_t),
                                                 false
                                             );
                break;

            default:
                throw std::runtime_error("Invalid version group.");
        }
    }
}

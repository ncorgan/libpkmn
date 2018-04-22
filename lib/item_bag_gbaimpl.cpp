/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#define NATIVE_RCAST (reinterpret_cast<union pksav_gba_item_bag*>(_native))
#define RS_BAG       (NATIVE_RCAST->rs)
#define EMERALD_BAG  (NATIVE_RCAST->emerald)
#define FRLG_BAG     (NATIVE_RCAST->frlg)

#include "item_bag_gbaimpl.hpp"
#include "item_list_modernimpl.hpp"

#include "database/database_common.hpp"

#include <boost/assert.hpp>
#include <boost/thread/lock_guard.hpp>

#include <cstring>
#include <stdexcept>

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
            _native = reinterpret_cast<void*>(new union pksav_gba_item_bag);
            std::memset(_native, 0, sizeof(union pksav_gba_item_bag));
            _our_mem = true;
        }

        _set_ptrs();
    }

    item_bag_gbaimpl::item_bag_gbaimpl(
        int game_id,
        const union pksav_gba_item_bag &item_storage
    ): item_bag_impl(game_id)
    {
        _native = reinterpret_cast<void*>(new union pksav_gba_item_bag);
        *NATIVE_RCAST = item_storage;
        _our_mem = true;

        _set_ptrs();
    }

    item_bag_gbaimpl::~item_bag_gbaimpl()
    {
        if(_our_mem)
        {
            delete NATIVE_RCAST;
        }
    }

    void item_bag_gbaimpl::_set_ptrs()
    {
        BOOST_STATIC_CONSTEXPR int VERSION_GROUP_RS = 5;
        BOOST_STATIC_CONSTEXPR int VERSION_GROUP_EMERALD = 6;
        BOOST_STATIC_CONSTEXPR int VERSION_GROUP_FRLG = 7;

        BOOST_STATIC_CONSTEXPR int ITEM_POCKET_IDS[]     = {15,21,27};
        BOOST_STATIC_CONSTEXPR int BALL_POCKET_IDS[]     = {16,22,28};
        BOOST_STATIC_CONSTEXPR int TMHM_POCKET_IDS[]     = {17,23,29};
        BOOST_STATIC_CONSTEXPR int BERRY_POCKET_IDS[]    = {18,24,30};
        BOOST_STATIC_CONSTEXPR int KEY_ITEM_POCKET_IDS[] = {19,25,31};

        int version_group = pkmn::database::game_id_to_version_group(_game_id);
        int index = version_group - VERSION_GROUP_RS;

        // Set up differences between versions
        std::string berry_pocket = (version_group == VERSION_GROUP_FRLG) ? "Berry Pouch"
                                                                         : "Berries";
        std::string tmhm_pocket = (version_group == VERSION_GROUP_FRLG) ? "TM Case"
                                                                        : "TMs & HMs";

        switch(version_group)
        {
            case VERSION_GROUP_RS:
                _item_pockets["Items"] = std::make_shared<item_list_modernimpl>(
                                             ITEM_POCKET_IDS[index], _game_id, RS_BAG.items,
                                             sizeof(RS_BAG.items)/sizeof(struct pksav_item),
                                             false
                                         );
                _item_pockets["Poké Balls"] = std::make_shared<item_list_modernimpl>(
                                                  BALL_POCKET_IDS[index], _game_id, RS_BAG.balls,
                                                  sizeof(RS_BAG.balls)/sizeof(struct pksav_item),
                                                  false
                                              );
                _item_pockets[tmhm_pocket] = std::make_shared<item_list_modernimpl>(
                                                 TMHM_POCKET_IDS[index], _game_id, RS_BAG.tms_hms,
                                                 sizeof(RS_BAG.tms_hms)/sizeof(struct pksav_item),
                                                 false
                                             );
                _item_pockets[berry_pocket] = std::make_shared<item_list_modernimpl>(
                                                  BERRY_POCKET_IDS[index], _game_id, RS_BAG.berries,
                                                  sizeof(RS_BAG.berries)/sizeof(struct pksav_item),
                                                  false
                                              );
                _item_pockets["Key Items"] = std::make_shared<item_list_modernimpl>(
                                                 KEY_ITEM_POCKET_IDS[index], _game_id, RS_BAG.key_items,
                                                 sizeof(RS_BAG.key_items)/sizeof(struct pksav_item),
                                                 false
                                             );
                break;

            case VERSION_GROUP_EMERALD:
                _item_pockets["Items"] = std::make_shared<item_list_modernimpl>(
                                             ITEM_POCKET_IDS[index], _game_id, EMERALD_BAG.items,
                                             sizeof(EMERALD_BAG.items)/sizeof(struct pksav_item),
                                             false
                                         );
                _item_pockets["Poké Balls"] = std::make_shared<item_list_modernimpl>(
                                                  BALL_POCKET_IDS[index], _game_id, EMERALD_BAG.balls,
                                                  sizeof(EMERALD_BAG.balls)/sizeof(struct pksav_item),
                                                  false
                                              );
                _item_pockets[tmhm_pocket] = std::make_shared<item_list_modernimpl>(
                                                 TMHM_POCKET_IDS[index], _game_id, EMERALD_BAG.tms_hms,
                                                 sizeof(EMERALD_BAG.tms_hms)/sizeof(struct pksav_item),
                                                 false
                                             );
                _item_pockets[berry_pocket] = std::make_shared<item_list_modernimpl>(
                                                  BERRY_POCKET_IDS[index], _game_id, EMERALD_BAG.berries,
                                                  sizeof(EMERALD_BAG.berries)/sizeof(struct pksav_item),
                                                  false
                                              );
                _item_pockets["Key Items"] = std::make_shared<item_list_modernimpl>(
                                                 KEY_ITEM_POCKET_IDS[index], _game_id, EMERALD_BAG.key_items,
                                                 sizeof(EMERALD_BAG.key_items)/sizeof(struct pksav_item),
                                                 false
                                             );
                break;

            case VERSION_GROUP_FRLG:
                _item_pockets["Items"] = std::make_shared<item_list_modernimpl>(
                                             ITEM_POCKET_IDS[index], _game_id, FRLG_BAG.items,
                                             sizeof(FRLG_BAG.items)/sizeof(struct pksav_item),
                                             false
                                         );
                _item_pockets["Poké Balls"] = std::make_shared<item_list_modernimpl>(
                                                  BALL_POCKET_IDS[index], _game_id, FRLG_BAG.balls,
                                                  sizeof(FRLG_BAG.balls)/sizeof(struct pksav_item),
                                                  false
                                              );
                _item_pockets[tmhm_pocket] = std::make_shared<item_list_modernimpl>(
                                                 TMHM_POCKET_IDS[index], _game_id, FRLG_BAG.tms_hms,
                                                 sizeof(FRLG_BAG.tms_hms)/sizeof(struct pksav_item),
                                                 false
                                             );
                _item_pockets[berry_pocket] = std::make_shared<item_list_modernimpl>(
                                                  BERRY_POCKET_IDS[index], _game_id, FRLG_BAG.berries,
                                                  sizeof(FRLG_BAG.berries)/sizeof(struct pksav_item),
                                                  false
                                              );
                _item_pockets["Key Items"] = std::make_shared<item_list_modernimpl>(
                                                 KEY_ITEM_POCKET_IDS[index], _game_id, FRLG_BAG.key_items,
                                                 sizeof(FRLG_BAG.key_items)/sizeof(struct pksav_item),
                                                 false
                                             );
                break;

            default:
                BOOST_ASSERT_MSG(false, "Invalid version group.");
        }
    }
}

/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#define GBA_CAST(ptr)    (static_cast<union pksav_gba_item_bag*>(ptr))
#define RS_BAG(ptr)      (GBA_CAST(ptr)->rs)
#define EMERALD_BAG(ptr) (GBA_CAST(ptr)->emerald)
#define FRLG_BAG(ptr)    (GBA_CAST(ptr)->frlg)

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
        void* p_native
    ): item_bag_impl(game_id)
    {
        if(p_native)
        {
            _p_native = p_native;
            _is_our_mem = false;
        }
        else
        {
            _p_native = new union pksav_gba_item_bag;
            std::memset(_p_native, 0, sizeof(union pksav_gba_item_bag));
            _is_our_mem = true;
        }

        _set_ptrs();
    }

    item_bag_gbaimpl::~item_bag_gbaimpl()
    {
        if(_is_our_mem)
        {
            delete GBA_CAST(_p_native);
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
                                             ITEM_POCKET_IDS[index],
                                             _game_id,
                                             RS_BAG(_p_native).items,
                                             RAW_ARRAY_LENGTH(RS_BAG(_p_native).items),
                                             false
                                         );
                _item_pockets["Poké Balls"] = std::make_shared<item_list_modernimpl>(
                                                  BALL_POCKET_IDS[index],
                                                  _game_id,
                                                  RS_BAG(_p_native).balls,
                                                  RAW_ARRAY_LENGTH(RS_BAG(_p_native).balls),
                                                  false
                                              );
                _item_pockets[tmhm_pocket] = std::make_shared<item_list_modernimpl>(
                                                 TMHM_POCKET_IDS[index],
                                                 _game_id,
                                                 RS_BAG(_p_native).tms_hms,
                                                 RAW_ARRAY_LENGTH(RS_BAG(_p_native).tms_hms),
                                                 false
                                             );
                _item_pockets[berry_pocket] = std::make_shared<item_list_modernimpl>(
                                                  BERRY_POCKET_IDS[index],
                                                  _game_id,
                                                  RS_BAG(_p_native).berries,
                                                  RAW_ARRAY_LENGTH(RS_BAG(_p_native).berries),
                                                  false
                                              );
                _item_pockets["Key Items"] = std::make_shared<item_list_modernimpl>(
                                                 KEY_ITEM_POCKET_IDS[index],
                                                 _game_id,
                                                 RS_BAG(_p_native).key_items,
                                                 RAW_ARRAY_LENGTH(RS_BAG(_p_native).key_items),
                                                 false
                                             );
                break;

            case VERSION_GROUP_EMERALD:
                _item_pockets["Items"] = std::make_shared<item_list_modernimpl>(
                                             ITEM_POCKET_IDS[index],
                                             _game_id,
                                             EMERALD_BAG(_p_native).items,
                                             RAW_ARRAY_LENGTH(EMERALD_BAG(_p_native).items),
                                             false
                                         );
                _item_pockets["Poké Balls"] = std::make_shared<item_list_modernimpl>(
                                                  BALL_POCKET_IDS[index],
                                                  _game_id,
                                                  EMERALD_BAG(_p_native).balls,
                                                  RAW_ARRAY_LENGTH(EMERALD_BAG(_p_native).balls),
                                                  false
                                              );
                _item_pockets[tmhm_pocket] = std::make_shared<item_list_modernimpl>(
                                                 TMHM_POCKET_IDS[index],
                                                 _game_id,
                                                 EMERALD_BAG(_p_native).tms_hms,
                                                 RAW_ARRAY_LENGTH(EMERALD_BAG(_p_native).tms_hms),
                                                 false
                                             );
                _item_pockets[berry_pocket] = std::make_shared<item_list_modernimpl>(
                                                  BERRY_POCKET_IDS[index],
                                                  _game_id,
                                                  EMERALD_BAG(_p_native).berries,
                                                  RAW_ARRAY_LENGTH(EMERALD_BAG(_p_native).berries),
                                                  false
                                              );
                _item_pockets["Key Items"] = std::make_shared<item_list_modernimpl>(
                                                 KEY_ITEM_POCKET_IDS[index],
                                                 _game_id,
                                                 EMERALD_BAG(_p_native).key_items,
                                                 RAW_ARRAY_LENGTH(EMERALD_BAG(_p_native).key_items),
                                                 false
                                             );
                break;

            case VERSION_GROUP_FRLG:
                _item_pockets["Items"] = std::make_shared<item_list_modernimpl>(
                                             ITEM_POCKET_IDS[index],
                                             _game_id,
                                             FRLG_BAG(_p_native).items,
                                             RAW_ARRAY_LENGTH(FRLG_BAG(_p_native).items),
                                             false
                                         );
                _item_pockets["Poké Balls"] = std::make_shared<item_list_modernimpl>(
                                                  BALL_POCKET_IDS[index],
                                                  _game_id,
                                                  FRLG_BAG(_p_native).balls,
                                                  RAW_ARRAY_LENGTH(FRLG_BAG(_p_native).balls),
                                                  false
                                              );
                _item_pockets[tmhm_pocket] = std::make_shared<item_list_modernimpl>(
                                                 TMHM_POCKET_IDS[index],
                                                 _game_id,
                                                 FRLG_BAG(_p_native).tms_hms,
                                                 RAW_ARRAY_LENGTH(FRLG_BAG(_p_native).tms_hms),
                                                 false
                                             );
                _item_pockets[berry_pocket] = std::make_shared<item_list_modernimpl>(
                                                  BERRY_POCKET_IDS[index],
                                                  _game_id,
                                                  FRLG_BAG(_p_native).berries,
                                                  RAW_ARRAY_LENGTH(FRLG_BAG(_p_native).berries),
                                                  false
                                              );
                _item_pockets["Key Items"] = std::make_shared<item_list_modernimpl>(
                                                 KEY_ITEM_POCKET_IDS[index],
                                                 _game_id,
                                                 FRLG_BAG(_p_native).key_items,
                                                 RAW_ARRAY_LENGTH(FRLG_BAG(_p_native).key_items),
                                                 false
                                             );
                break;

            default:
                BOOST_ASSERT_MSG(false, "Invalid version group.");
        }
    }
}

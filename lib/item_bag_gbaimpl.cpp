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

static const int VERSION_GROUP_RS      = 5;
static const int VERSION_GROUP_EMERALD = 6;
static const int VERSION_GROUP_FRLG    = 7;

static const std::vector<int> ITEM_POCKET_IDS     = {15,21,27};
static const std::vector<int> BALL_POCKET_IDS     = {16,22,28};
static const std::vector<int> TMHM_POCKET_IDS     = {17,23,29};
static const std::vector<int> BERRY_POCKET_IDS    = {18,24,30};
static const std::vector<int> KEY_ITEM_POCKET_IDS = {19,25,31};

namespace pkmn {

    item_bag_gbaimpl::item_bag_gbaimpl(
        int game_id,
        const union pksav_gba_item_bag* p_pksav_bag
    ): item_bag_impl(game_id)
    {
        if(p_pksav_bag != nullptr)
        {
            _pksav_bag = *p_pksav_bag;
        }
        else
        {
            std::memset(&_pksav_bag, 0, sizeof(_pksav_bag));
        }

        _p_native = &_pksav_bag;

        const int version_group = pkmn::database::game_id_to_version_group(_game_id);

        const int index = version_group - VERSION_GROUP_RS;
        const bool is_frlg = (version_group == VERSION_GROUP_FRLG);

        const struct pksav_item* p_item_pocket     = nullptr;
        const struct pksav_item* p_ball_pocket     = nullptr;
        const struct pksav_item* p_tmhm_pocket     = nullptr;
        const struct pksav_item* p_berry_pocket    = nullptr;
        const struct pksav_item* p_key_item_pocket = nullptr;

        // Set up differences between versions
        const std::string berry_pocket_name = is_frlg ? "Berry Pouch" : "Berries";
        const std::string tmhm_pocket_name = is_frlg ? "TM Case" : "TMs & HMs";

        switch(version_group)
        {
            case VERSION_GROUP_RS:
                p_item_pocket     = _pksav_bag.rs.items;
                p_ball_pocket     = _pksav_bag.rs.balls;
                p_tmhm_pocket     = _pksav_bag.rs.tms_hms;
                p_berry_pocket    = _pksav_bag.rs.berries;
                p_key_item_pocket = _pksav_bag.rs.key_items;
                break;

            case VERSION_GROUP_EMERALD:
                p_item_pocket     = _pksav_bag.emerald.items;
                p_ball_pocket     = _pksav_bag.emerald.balls;
                p_tmhm_pocket     = _pksav_bag.emerald.tms_hms;
                p_berry_pocket    = _pksav_bag.emerald.berries;
                p_key_item_pocket = _pksav_bag.emerald.key_items;
                break;

            case VERSION_GROUP_FRLG:
                p_item_pocket     = _pksav_bag.frlg.items;
                p_ball_pocket     = _pksav_bag.frlg.balls;
                p_tmhm_pocket     = _pksav_bag.frlg.tms_hms;
                p_berry_pocket    = _pksav_bag.frlg.berries;
                p_key_item_pocket = _pksav_bag.frlg.key_items;
                break;

            default:
                BOOST_ASSERT_MSG(false, "Invalid version group");
        }

        _item_pockets["Items"] = std::make_shared<item_list_modernimpl>(
                                     ITEM_POCKET_IDS[index],
                                     _game_id,
                                     p_item_pocket
                                 );
        _item_pockets["Poké Balls"] = std::make_shared<item_list_modernimpl>(
                                          BALL_POCKET_IDS[index],
                                          _game_id,
                                          p_ball_pocket
                                      );
        _item_pockets[tmhm_pocket_name] = std::make_shared<item_list_modernimpl>(
                                              TMHM_POCKET_IDS[index],
                                              _game_id,
                                              p_tmhm_pocket
                                          );
        _item_pockets[berry_pocket_name] = std::make_shared<item_list_modernimpl>(
                                               BERRY_POCKET_IDS[index],
                                               _game_id,
                                               p_berry_pocket
                                           );
        _item_pockets["Key Items"] = std::make_shared<item_list_modernimpl>(
                                         BERRY_POCKET_IDS[index],
                                         _game_id,
                                         p_berry_pocket
                                     );
    }
}

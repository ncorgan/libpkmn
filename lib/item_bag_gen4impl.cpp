/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#define NATIVE_RCAST (reinterpret_cast<pksav_gen4_item_bag_t*>(_native))
#define DPPT_BAG (NATIVE_RCAST->dppt)
#define HGSS_BAG (NATIVE_RCAST->hgss)

#include "item_bag_gen4impl.hpp"
#include "item_list_modernimpl.hpp"

#include "database/database_common.hpp"

#include <pksav/gen4/items.h>

#include <pkmn/types/shared_ptr.hpp>

#include <cstring>

namespace pkmn {

    item_bag_gen4impl::item_bag_gen4impl(
        int game_id,
        void* ptr
    ): item_bag_impl(game_id)
    {
        if(ptr) {
            _native = ptr;
            _our_mem = false;
        } else {
            _native = reinterpret_cast<void*>(new pksav_gen4_item_bag_t);
            std::memset(_native, 0, sizeof(pksav_gen4_item_bag_t));
            _our_mem = true;
        }

        _set_ptrs();
    }

    item_bag_gen4impl::~item_bag_gen4impl()
    {
        if(_our_mem)
        {
            delete NATIVE_RCAST;
        }
    }

    void item_bag_gen4impl::_set_ptrs() {

        BOOST_STATIC_CONSTEXPR int VERSION_GROUP_DP   = 8;
        BOOST_STATIC_CONSTEXPR int VERSION_GROUP_HGSS = 10;

        BOOST_STATIC_CONSTEXPR int ITEM_POCKET_IDS[]        = {33,41,49};
        BOOST_STATIC_CONSTEXPR int MEDICINE_POCKET_IDS[]    = {34,42,50};
        BOOST_STATIC_CONSTEXPR int BALL_POCKET_IDS[]        = {35,43,51};
        BOOST_STATIC_CONSTEXPR int TMHM_POCKET_IDS[]        = {36,44,52};
        BOOST_STATIC_CONSTEXPR int BERRY_POCKET_IDS[]       = {37,45,53};
        BOOST_STATIC_CONSTEXPR int MAIL_POCKET_IDS[]        = {38,46,54};
        BOOST_STATIC_CONSTEXPR int BATTLE_ITEM_POCKET_IDS[] = {39,47,55};
        BOOST_STATIC_CONSTEXPR int KEY_ITEM_POCKET_IDS[]    = {40,48,56};

        int version_group = pkmn::database::game_id_to_version_group(_game_id);
        if(version_group < VERSION_GROUP_DP or version_group > VERSION_GROUP_HGSS) {
            throw std::runtime_error("Invalid version group.");
        }

        int index = version_group - VERSION_GROUP_DP;
        if(version_group == VERSION_GROUP_HGSS) {
            _item_pockets["Items"] = pkmn::make_shared<item_list_modernimpl>(
                                         ITEM_POCKET_IDS[index], _game_id, HGSS_BAG.items,
                                         sizeof(HGSS_BAG.items)/sizeof(pksav_item_t),
                                         false
                                     );
            _item_pockets["Medicine"] = pkmn::make_shared<item_list_modernimpl>(
                                            MEDICINE_POCKET_IDS[index], _game_id, HGSS_BAG.medicine,
                                            sizeof(HGSS_BAG.medicine)/sizeof(pksav_item_t),
                                            false
                                        );
            _item_pockets["Poké Balls"] = pkmn::make_shared<item_list_modernimpl>(
                                              BALL_POCKET_IDS[index], _game_id, HGSS_BAG.balls,
                                              sizeof(HGSS_BAG.balls)/sizeof(pksav_item_t),
                                              false
                                          );
            _item_pockets["TMs & HMs"] = pkmn::make_shared<item_list_modernimpl>(
                                             TMHM_POCKET_IDS[index], _game_id, HGSS_BAG.tms_hms,
                                             sizeof(HGSS_BAG.tms_hms)/sizeof(pksav_item_t),
                                             false
                                         );
            _item_pockets["Berries"] = pkmn::make_shared<item_list_modernimpl>(
                                           BERRY_POCKET_IDS[index], _game_id, HGSS_BAG.berries,
                                           sizeof(HGSS_BAG.berries)/sizeof(pksav_item_t),
                                           false
                                       );
            _item_pockets["Mail"] = pkmn::make_shared<item_list_modernimpl>(
                                        MAIL_POCKET_IDS[index], _game_id, HGSS_BAG.mail,
                                        sizeof(HGSS_BAG.mail)/sizeof(pksav_item_t),
                                        false
                                    );
            _item_pockets["Battle Items"] = pkmn::make_shared<item_list_modernimpl>(
                                                BATTLE_ITEM_POCKET_IDS[index], _game_id, HGSS_BAG.battle_items,
                                                sizeof(HGSS_BAG.battle_items)/sizeof(pksav_item_t),
                                                false
                                            );
            _item_pockets["Key Items"] = pkmn::make_shared<item_list_modernimpl>(
                                             KEY_ITEM_POCKET_IDS[index], _game_id, HGSS_BAG.key_items,
                                             sizeof(HGSS_BAG.key_items)/sizeof(pksav_item_t),
                                             false
                                         );
        } else {
        }
    }
}

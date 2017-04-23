/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#define GC_RCAST   reinterpret_cast<LibPkmGC::GC::BagData*>(_native)
#define COLO_RCAST reinterpret_cast<LibPkmGC::Colosseum::BagData*>(_native)
#define XD_RCAST   reinterpret_cast<LibPkmGC::XD::BagData*>(_native)

#include "item_bag_gcnimpl.hpp"
#include "item_list_gcnimpl.hpp"

#include "database/database_common.hpp"

#include <LibPkmGC/GC/Common/BagData.h>
#include <LibPkmGC/Colosseum/Common/BagData.h>
#include <LibPkmGC/XD/Common/BagData.h>

namespace pkmn {

    BOOST_STATIC_CONSTEXPR int COLOSSEUM = 19;
    BOOST_STATIC_CONSTEXPR int XD = 20;

    item_bag_gcnimpl::item_bag_gcnimpl(
        int game_id,
        void* ptr
    ): item_bag_impl(game_id)
    {
        if(ptr) {
            _native = ptr;
            _our_mem = false;
        } else {
            if(_game_id == COLOSSEUM) {
                _native = reinterpret_cast<void*>(new LibPkmGC::Colosseum::BagData);
            } else {
                _native = reinterpret_cast<void*>(new LibPkmGC::XD::BagData);
            }

            _our_mem = true;
        }

        _set_ptrs();
    }

    item_bag_gcnimpl::~item_bag_gcnimpl() {
        item_bag_scoped_lock lock(this);

        if(_our_mem) {
            if(_game_id == COLOSSEUM) {
                delete COLO_RCAST;
            } else {
                delete XD_RCAST;
            }
        }
    }

    #define ITEM_POCKET_ID ((_game_id == COLOSSEUM) ? 62 : 69)
    #define KEY_ITEM_POCKET_ID ((_game_id == COLOSSEUM) ? 63 : 70)
    #define BALL_POCKET_ID ((_game_id == COLOSSEUM) ? 64 : 71)
    #define TMHM_POCKET_ID ((_game_id == COLOSSEUM) ? 65 : 72)
    #define BERRY_POCKET_ID ((_game_id == COLOSSEUM) ? 66 : 73)
    #define COLOGNE_POCKET_ID ((_game_id == COLOSSEUM) ? 67 : 74)
    BOOST_STATIC_CONSTEXPR int BATTLE_CD_POCKET_ID = 91;

    // This is stored as a pointer, so we can't use the nicer sizeof approach.
    #define ITEM_POCKET_CAPACITY ((_game_id == COLOSSEUM) ? 20 : 30)

    void item_bag_gcnimpl::_set_ptrs() {
        _item_pockets["Items"] = pkmn::make_shared<item_list_gcnimpl>(
                                     ITEM_POCKET_ID, _game_id, GC_RCAST->regularItems,
                                     ITEM_POCKET_CAPACITY, false
                                 );
        _item_pockets["Key Items"] = pkmn::make_shared<item_list_gcnimpl>(
                                         KEY_ITEM_POCKET_ID, _game_id, GC_RCAST->keyItems,
                                         sizeof(GC_RCAST->keyItems)/sizeof(LibPkmGC::Item),
                                         false
                                     );
        _item_pockets["Poké Balls"] = pkmn::make_shared<item_list_gcnimpl>(
                                          BALL_POCKET_ID, _game_id, GC_RCAST->pokeballs,
                                          sizeof(GC_RCAST->pokeballs)/sizeof(LibPkmGC::Item),
                                          false
                                      );
        _item_pockets["TMs & HMs"] = pkmn::make_shared<item_list_gcnimpl>(
                                         TMHM_POCKET_ID, _game_id, GC_RCAST->TMs,
                                         sizeof(GC_RCAST->TMs)/sizeof(LibPkmGC::Item),
                                         false
                                     );
        _item_pockets["Berries"] = pkmn::make_shared<item_list_gcnimpl>(
                                       BERRY_POCKET_ID, _game_id, GC_RCAST->berries,
                                       sizeof(GC_RCAST->berries)/sizeof(LibPkmGC::Item),
                                       false
                                   );
        _item_pockets["Colognes"] = pkmn::make_shared<item_list_gcnimpl>(
                                        COLOGNE_POCKET_ID, _game_id, GC_RCAST->colognes,
                                        sizeof(GC_RCAST->colognes)/sizeof(LibPkmGC::Item),
                                        false
                                    );
        if(_game_id == XD) {
            _item_pockets["Battle CDs"] = pkmn::make_shared<item_list_gcnimpl>(
                                              BATTLE_CD_POCKET_ID, _game_id, XD_RCAST->battleCDs,
                                              sizeof(XD_RCAST->battleCDs)/sizeof(LibPkmGC::Item),
                                              false
                                          );
        }
    }
}

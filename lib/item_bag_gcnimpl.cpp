/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#define GC_CAST(ptr)   (static_cast<LibPkmGC::GC::BagData*>(ptr))
#define COLO_CAST(ptr) (static_cast<LibPkmGC::Colosseum::BagData*>(ptr))
#define XD_CAST(ptr)   (static_cast<LibPkmGC::XD::BagData*>(ptr))

#include "item_bag_gcnimpl.hpp"
#include "item_list_gcnimpl.hpp"

#include "libpkmgc_includes.hpp"

#include "utils/misc.hpp"

#include <boost/thread/lock_guard.hpp>

namespace pkmn {

    BOOST_STATIC_CONSTEXPR int COLOSSEUM = 19;
    BOOST_STATIC_CONSTEXPR int XD = 20;

    item_bag_gcnimpl::item_bag_gcnimpl(
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
            if(_game_id == COLOSSEUM)
            {
                _p_native = new LibPkmGC::Colosseum::BagData;
            }
            else
            {
                _p_native = new LibPkmGC::XD::BagData;
            }

            _is_our_mem = true;
        }

        _set_ptrs();
    }

    item_bag_gcnimpl::~item_bag_gcnimpl()
    {
        if(_is_our_mem)
        {
            if(_game_id == COLOSSEUM)
            {
                delete COLO_CAST(_p_native);
            }
            else
            {
                delete XD_CAST(_p_native);
            }
        }
    }

    #define ITEM_POCKET_ID ((_game_id == COLOSSEUM) ? 62 : 69)
    #define KEY_ITEM_POCKET_ID ((_game_id == COLOSSEUM) ? 63 : 70)
    #define BALL_POCKET_ID ((_game_id == COLOSSEUM) ? 64 : 71)
    #define TM_POCKET_ID ((_game_id == COLOSSEUM) ? 65 : 72)
    #define BERRY_POCKET_ID ((_game_id == COLOSSEUM) ? 66 : 73)
    #define COLOGNE_POCKET_ID ((_game_id == COLOSSEUM) ? 67 : 74)
    BOOST_STATIC_CONSTEXPR int BATTLE_CD_POCKET_ID = 91;

    // This is stored as a pointer, so we can't use the nicer sizeof approach.
    #define ITEM_POCKET_CAPACITY ((_game_id == COLOSSEUM) ? 20 : 30)

    void item_bag_gcnimpl::_set_ptrs()
    {
        _item_pockets["Items"] = std::make_shared<item_list_gcnimpl>(
                                     ITEM_POCKET_ID,
                                     _game_id,
                                     GC_CAST(_p_native)->regularItems,
                                     ITEM_POCKET_CAPACITY,
                                     false
                                 );
        _item_pockets["Key Items"] = std::make_shared<item_list_gcnimpl>(
                                         KEY_ITEM_POCKET_ID,
                                         _game_id,
                                         GC_CAST(_p_native)->keyItems,
                                         RAW_ARRAY_LENGTH(GC_CAST(_p_native)->keyItems),
                                         false
                                     );
        _item_pockets["Poké Balls"] = std::make_shared<item_list_gcnimpl>(
                                          BALL_POCKET_ID,
                                          _game_id,
                                          GC_CAST(_p_native)->pokeballs,
                                          RAW_ARRAY_LENGTH(GC_CAST(_p_native)->pokeballs),
                                          false
                                      );
        _item_pockets["TMs"] = std::make_shared<item_list_gcnimpl>(
                                   TM_POCKET_ID,
                                   _game_id,
                                   GC_CAST(_p_native)->TMs,
                                   RAW_ARRAY_LENGTH(GC_CAST(_p_native)->TMs),
                                   false
                               );
        _item_pockets["Berries"] = std::make_shared<item_list_gcnimpl>(
                                       BERRY_POCKET_ID,
                                       _game_id,
                                       GC_CAST(_p_native)->berries,
                                       RAW_ARRAY_LENGTH(GC_CAST(_p_native)->berries),
                                       false
                                   );
        _item_pockets["Colognes"] = std::make_shared<item_list_gcnimpl>(
                                        COLOGNE_POCKET_ID,
                                        _game_id,
                                        GC_CAST(_p_native)->colognes,
                                        RAW_ARRAY_LENGTH(GC_CAST(_p_native)->colognes),
                                        false
                                    );
        if(_game_id == XD)
        {
            _item_pockets["Battle CDs"] = std::make_shared<item_list_gcnimpl>(
                                              BATTLE_CD_POCKET_ID,
                                              _game_id,
                                              XD_CAST(_p_native)->battleCDs,
                                              RAW_ARRAY_LENGTH(XD_CAST(_p_native)->battleCDs),
                                              false
                                          );
        }
    }
}

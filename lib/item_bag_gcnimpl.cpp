/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "item_bag_gcnimpl.hpp"
#include "item_list_gcnimpl.hpp"

#include "utils/misc.hpp"

#include <boost/assert.hpp>
#include <boost/thread/lock_guard.hpp>

// To avoid copy+pasting the é
static const std::string BALL_POCKET_NAME = "Poké Balls";

namespace pkmn {

    item_bag_gcnimpl::item_bag_gcnimpl(
        int game_id,
        const LibPkmGC::GC::BagData* p_libpkmgc_bag
    ): item_bag_impl(game_id)
    {
        const bool is_colosseum = (_game_id == COLOSSEUM_ID);

        if(p_libpkmgc_bag != nullptr)
        {
            _libpkmgc_bag_uptr.reset(p_libpkmgc_bag->clone());
        }
        else
        {
            if(_game_id == COLOSSEUM_ID)
            {
                _libpkmgc_bag_uptr.reset(new LibPkmGC::Colosseum::BagData);
            }
            else
            {
                _libpkmgc_bag_uptr.reset(new LibPkmGC::XD::BagData);
            }
        }

        BOOST_ASSERT(_libpkmgc_bag_uptr.get() != nullptr);

        _p_native = _libpkmgc_bag_uptr.get();

        const int item_pocket_id     = is_colosseum ? 62 : 69;
        const int key_item_pocket_id = is_colosseum ? 63 : 70;
        const int ball_pocket_id     = is_colosseum ? 64 : 71;
        const int tm_pocket_id       = is_colosseum ? 65 : 72;
        const int berry_pocket_id    = is_colosseum ? 66 : 73;
        const int cologne_pocket_id  = is_colosseum ? 67 : 74;

        _item_pockets["Items"] = std::make_shared<item_list_gcnimpl>(
                                     item_pocket_id,
                                     _game_id,
                                     _libpkmgc_bag_uptr->regularItems
                                 );
        _item_pockets["Key Items"] = std::make_shared<item_list_gcnimpl>(
                                         key_item_pocket_id,
                                         _game_id,
                                         _libpkmgc_bag_uptr->keyItems
                                     );
        _item_pockets[BALL_POCKET_NAME] = std::make_shared<item_list_gcnimpl>(
                                              ball_pocket_id,
                                              _game_id,
                                              _libpkmgc_bag_uptr->pokeballs
                                          );
        _item_pockets["TMs"] = std::make_shared<item_list_gcnimpl>(
                                   tm_pocket_id,
                                   _game_id,
                                   _libpkmgc_bag_uptr->TMs
                               );
        _item_pockets["Berries"] = std::make_shared<item_list_gcnimpl>(
                                       berry_pocket_id,
                                       _game_id,
                                       _libpkmgc_bag_uptr->berries
                                   );
        _item_pockets["Colognes"] = std::make_shared<item_list_gcnimpl>(
                                        cologne_pocket_id,
                                        _game_id,
                                        _libpkmgc_bag_uptr->colognes
                                    );
        if(!is_colosseum)
        {
            static const int BATTLE_CD_POCKET_ID = 91;

            LibPkmGC::XD::BagData* p_xd_bag = dynamic_cast<LibPkmGC::XD::BagData*>(
                                                  _libpkmgc_bag_uptr.get()
                                              );
            BOOST_ASSERT(p_xd_bag != nullptr);

            _item_pockets["Battle CDs"] = std::make_shared<item_list_gcnimpl>(
                                              BATTLE_CD_POCKET_ID,
                                              _game_id,
                                              p_xd_bag->battleCDs
                                          );
        }
    }

    void item_bag_gcnimpl::_to_native()
    {
        std::memcpy(
            _libpkmgc_bag_uptr->regularItems,
            static_cast<LibPkmGC::Item*>(
                _item_pockets["Items"]->get_native()
            ),
            (sizeof(LibPkmGC::Item) * _libpkmgc_bag_uptr->nbRegularItems)
        );
        std::memcpy(
            _libpkmgc_bag_uptr->keyItems,
            static_cast<LibPkmGC::Item*>(
                _item_pockets["Key Items"]->get_native()
            ),
            sizeof(_libpkmgc_bag_uptr->keyItems)
        );
        std::memcpy(
            _libpkmgc_bag_uptr->pokeballs,
            static_cast<LibPkmGC::Item*>(
                _item_pockets[BALL_POCKET_NAME]->get_native()
            ),
            sizeof(_libpkmgc_bag_uptr->pokeballs)
        );
        std::memcpy(
            _libpkmgc_bag_uptr->TMs,
            static_cast<LibPkmGC::Item*>(
                _item_pockets["TMs"]->get_native()
            ),
            sizeof(_libpkmgc_bag_uptr->TMs)
        );
        std::memcpy(
            _libpkmgc_bag_uptr->berries,
            static_cast<LibPkmGC::Item*>(
                _item_pockets["Berries"]->get_native()
            ),
            sizeof(_libpkmgc_bag_uptr->berries)
        );
        std::memcpy(
            _libpkmgc_bag_uptr->colognes,
            static_cast<LibPkmGC::Item*>(
                _item_pockets["Colognes"]->get_native()
            ),
            sizeof(_libpkmgc_bag_uptr->colognes)
        );

        if(_game_id == pkmn::XD_ID)
        {
            LibPkmGC::XD::BagData* p_xd_bagdata = dynamic_cast<LibPkmGC::XD::BagData*>(
                                                      _libpkmgc_bag_uptr.get()
                                                  );
            BOOST_ASSERT(p_xd_bagdata != nullptr);
            BOOST_ASSERT(_item_pockets.count("Battle CDs") > 0);

            std::memcpy(
                p_xd_bagdata->battleCDs,
                static_cast<LibPkmGC::Item*>(
                    _item_pockets["Battle CDs"]->get_native()
                ),
                sizeof(p_xd_bagdata->battleCDs)
            );
        }
    }
}

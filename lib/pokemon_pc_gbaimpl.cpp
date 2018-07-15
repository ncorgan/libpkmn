/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_pc_gbaimpl.hpp"
#include "pokemon_box_gbaimpl.hpp"

#include "pksav/enum_maps.hpp"
#include "pksav/pksav_call.hpp"

#include <pksav/gba/pokemon.h>
#include <pksav/gba/text.h>

#include <boost/thread/lock_guard.hpp>

#include <cstring>

BOOST_STATIC_CONSTEXPR uint8_t GBA_TEXT_TERMINATOR = 0xFF;

BOOST_STATIC_CONSTEXPR int FIRERED_GAME_ID = 10;
BOOST_STATIC_CONSTEXPR int LEAFGREEN_GAME_ID = 11;

namespace pkmn {

    pokemon_pc_gbaimpl::pokemon_pc_gbaimpl(
        int game_id,
        const struct pksav_gba_pokemon_pc* p_native
    ): pokemon_pc_impl(game_id)
    {
        if(p_native != nullptr)
        {
            _pksav_pc = *p_native;
        }
        else
        {
            std::memset(
                &_pksav_pc,
                0,
                sizeof(_pksav_pc)
            );
            std::memset(
                _pksav_pc.box_names,
                GBA_TEXT_TERMINATOR,
                sizeof(_pksav_pc.box_names)
            );
        }

        _p_native = &_pksav_pc;

        _from_native();
    }

    int pokemon_pc_gbaimpl::get_num_boxes()
    {
        return PKSAV_GBA_NUM_POKEMON_BOXES;
    }

    void pokemon_pc_gbaimpl::_from_native()
    {
        _box_list.resize(PKSAV_GBA_NUM_POKEMON_BOXES);

        static const pksav::gba_box_wallpaper_bimap_t& gba_box_wallpaper_bimap =
            pksav::get_gba_box_wallpaper_bimap();
        static const pksav::gba_rse_box_wallpaper_bimap_t& gba_rse_box_wallpaper_bimap =
            pksav::get_gba_rse_box_wallpaper_bimap();
        static const pksav::gba_frlg_box_wallpaper_bimap_t& gba_frlg_box_wallpaper_bimap =
            pksav::get_gba_frlg_box_wallpaper_bimap();

        for(size_t box_index = 0; box_index < PKSAV_GBA_NUM_POKEMON_BOXES; ++box_index)
        {
            _box_list[box_index] = std::make_shared<pokemon_box_gbaimpl>(
                                       _game_id,
                                       &_pksav_pc.boxes[box_index]
                                   );

            char box_name[PKSAV_GBA_POKEMON_BOX_NAME_LENGTH + 1] = {0};
            PKSAV_CALL(
                pksav_gba_import_text(
                    _pksav_pc.box_names[box_index],
                    box_name,
                    PKSAV_GBA_POKEMON_BOX_NAME_LENGTH
                );
            )
            _box_list[box_index]->set_name(box_name);

            auto gba_box_wallpaper_iter =
                gba_box_wallpaper_bimap.right.find(
                    static_cast<enum pksav_gba_box_wallpaper>(
                        _pksav_pc.wallpapers[box_index]
                    )
                );
            if(gba_box_wallpaper_iter != gba_box_wallpaper_bimap.right.end())
            {
                _box_list[box_index]->set_wallpaper(
                    gba_box_wallpaper_iter->second
                );
            }
            else if((_game_id == FIRERED_GAME_ID) or (_game_id == LEAFGREEN_GAME_ID))
            {
                auto gba_frlg_box_wallpaper_iter =
                    gba_frlg_box_wallpaper_bimap.right.find(
                        static_cast<enum pksav_gba_frlg_box_wallpaper>(
                            _pksav_pc.wallpapers[box_index]
                        )
                    );
                if(gba_frlg_box_wallpaper_iter != gba_frlg_box_wallpaper_bimap.right.end())
                {
                    _box_list[box_index]->set_wallpaper(
                        gba_frlg_box_wallpaper_iter->second
                    );
                }
                else
                {
                    // If the save was screwed up, go with a sensible default.
                    _box_list[box_index]->set_wallpaper("Forest");
                }
            }
            else
            {
                auto gba_rse_box_wallpaper_iter =
                    gba_rse_box_wallpaper_bimap.right.find(
                        static_cast<enum pksav_gba_rse_box_wallpaper>(
                            _pksav_pc.wallpapers[box_index]
                        )
                    );
                if(gba_rse_box_wallpaper_iter != gba_rse_box_wallpaper_bimap.right.end())
                {
                    _box_list[box_index]->set_wallpaper(
                        gba_rse_box_wallpaper_iter->second
                    );
                }
                else
                {
                    // If the save was screwed up, go with a sensible default.
                    _box_list[box_index]->set_wallpaper("Forest");
                }
            }
        }
    }

    void pokemon_pc_gbaimpl::_to_native()
    {
        for(size_t box_index = 0;
            box_index < PKSAV_GBA_NUM_POKEMON_BOXES;
            ++box_index)
        {
            pkmn::rcast_equal<struct pksav_gba_pokemon_box>(
                _box_list[box_index]->get_native(),
                &_pksav_pc.boxes[box_index]
            );
        }

        _update_box_names();
        _update_native_box_wallpapers();
    }

    void pokemon_pc_gbaimpl::_update_box_names()
    {
        _box_names.resize(PKSAV_GBA_NUM_POKEMON_BOXES);

        for(size_t box_index = 0;
            box_index < PKSAV_GBA_NUM_POKEMON_BOXES;
            ++box_index)
        {
            _box_names[box_index] = _box_list[box_index]->get_name();

            PKSAV_CALL(
                pksav_gba_export_text(
                    _box_names[box_index].c_str(),
                    _pksav_pc.box_names[box_index],
                    PKSAV_GBA_POKEMON_BOX_NAME_LENGTH
                );
            )
        }
    }

    void pokemon_pc_gbaimpl::_update_native_box_wallpapers()
    {
        for(size_t box_index = 0;
            box_index < PKSAV_GBA_NUM_POKEMON_BOXES;
            ++box_index)
        {
            // TODO: assert in valid keys
            std::string wallpaper = _box_list[box_index]->get_wallpaper();

            const pksav::gba_box_wallpaper_bimap_t& gba_box_wallpaper_bimap =
                pksav::get_gba_box_wallpaper_bimap();

            auto gba_box_wallpaper_iter = gba_box_wallpaper_bimap.left.find(wallpaper);
            if(gba_box_wallpaper_iter != gba_box_wallpaper_bimap.left.end())
            {
                _pksav_pc.wallpapers[box_index] = static_cast<uint8_t>(
                                                      gba_box_wallpaper_iter->second
                                                  );
            }
            else if((_game_id == FIRERED_GAME_ID) or (_game_id == LEAFGREEN_GAME_ID))
            {
                const pksav::gba_frlg_box_wallpaper_bimap_t& gba_frlg_box_wallpaper_bimap =
                    pksav::get_gba_frlg_box_wallpaper_bimap();

                auto gba_frlg_box_wallpaper_iter = gba_frlg_box_wallpaper_bimap.left.find(wallpaper);
                BOOST_ASSERT(gba_frlg_box_wallpaper_iter != gba_frlg_box_wallpaper_bimap.left.end());

                _pksav_pc.wallpapers[box_index] = static_cast<uint8_t>(
                                                      gba_frlg_box_wallpaper_iter->second
                                                  );
            }
            else
            {
                const pksav::gba_rse_box_wallpaper_bimap_t& gba_rse_box_wallpaper_bimap =
                    pksav::get_gba_rse_box_wallpaper_bimap();

                auto gba_rse_box_wallpaper_iter = gba_rse_box_wallpaper_bimap.left.find(wallpaper);
                BOOST_ASSERT(gba_rse_box_wallpaper_iter != gba_rse_box_wallpaper_bimap.left.end());

                _pksav_pc.wallpapers[box_index] = static_cast<uint8_t>(
                                                      gba_rse_box_wallpaper_iter->second
                                                  );
            }
        }
    }
}

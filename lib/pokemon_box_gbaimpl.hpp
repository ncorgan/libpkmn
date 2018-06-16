/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_BOX_GBAIMPL_HPP
#define PKMN_POKEMON_BOX_GBAIMPL_HPP

#include "pokemon_box_impl.hpp"

#include <pksav/gba/pokemon.h>

#include <vector>

namespace pkmn {

    std::vector<std::string> get_valid_gba_wallpaper_names(
        const std::string& game
    );

    class pokemon_box_gbaimpl: public pokemon_box_impl {
        public:
            pokemon_box_gbaimpl() {}
            explicit pokemon_box_gbaimpl(
                int game_id
            );
            pokemon_box_gbaimpl(
                int game_id,
                struct pksav_gba_pokemon_box* native
            );
            pokemon_box_gbaimpl(
                int game_id,
                const struct pksav_gba_pokemon_box &native
            );

            ~pokemon_box_gbaimpl();

            std::string get_name() final;

            void set_name(
                const std::string& name
            ) final;

            int get_num_pokemon() final;

            int get_capacity() final;

            void set_pokemon(
                int index,
                const pkmn::pokemon::sptr& new_pokemon
            ) final;

            std::string get_wallpaper() final;

            void set_wallpaper(
                const std::string& wallpaper
            ) final;

        private:
            void _from_native() final;

            std::string _wallpaper;
    };
}

#endif /* PKMN_POKEMON_BOX_GBAIMPL_HPP */

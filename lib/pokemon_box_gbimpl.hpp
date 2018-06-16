/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_BOX_GBIMPL_HPP
#define PKMN_POKEMON_BOX_GBIMPL_HPP

#include "pokemon_box_impl.hpp"

#include "pokemon_gen1impl.hpp"
#include "pokemon_gen2impl.hpp"

#define POKEMON_BOX_GBIMPL_TEMPLATE \
    template \
    <typename list_type, \
     typename pksav_pc_pokemon_type, \
     typename pksav_pokemon_party_data_type, \
     typename libpkmn_pokemon_type>

#define POKEMON_BOX_GBIMPL_CLASS \
    pokemon_box_gbimpl< \
        list_type, \
        pksav_pc_pokemon_type, \
        pksav_pokemon_party_data_type, \
        libpkmn_pokemon_type>

namespace pkmn {

    POKEMON_BOX_GBIMPL_TEMPLATE
    class pokemon_box_gbimpl: public pokemon_box_impl {
        public:
            pokemon_box_gbimpl() {}
            explicit pokemon_box_gbimpl(
                int game_id
            );
            pokemon_box_gbimpl(
                int game_id,
                list_type* native
            );

            ~pokemon_box_gbimpl();

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
    };

    // Generation I
    typedef pokemon_box_gbimpl<
                struct pksav_gen1_pokemon_box,
                struct pksav_gen1_pc_pokemon,
                struct pksav_gen1_pokemon_party_data,
                pokemon_gen1impl> pokemon_box_gen1impl;

    // Generation II
    typedef pokemon_box_gbimpl<
                struct pksav_gen2_pokemon_box,
                struct pksav_gen2_pc_pokemon,
                struct pksav_gen2_pokemon_party_data,
                pokemon_gen2impl> pokemon_box_gen2impl;
}

#include "pokemon_box_gbimpl.ipp"

#endif /* PKMN_POKEMON_BOX_GBIMPL_HPP */

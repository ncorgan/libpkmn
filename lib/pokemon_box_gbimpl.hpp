/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
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

            std::string get_name() override final;

            void set_name(
                const std::string& name
            ) override final;

            int get_num_pokemon() override final;

            int get_capacity() override final;

            void set_pokemon(
                int index,
                pkmn::pokemon::sptr new_pokemon
            ) override final;

        private:

            void _from_native() override final;
    };

    // Generation I
    typedef pokemon_box_gbimpl<
                pksav_gen1_pokemon_box_t,
                pksav_gen1_pc_pokemon_t,
                pksav_gen1_pokemon_party_data_t,
                pokemon_gen1impl> pokemon_box_gen1impl;

    // Generation II
    typedef pokemon_box_gbimpl<
                pksav_gen2_pokemon_box_t,
                pksav_gen2_pc_pokemon_t,
                pksav_gen2_pokemon_party_data_t,
                pokemon_gen2impl> pokemon_box_gen2impl;
}

#include "pokemon_box_gbimpl.ipp"

#endif /* PKMN_POKEMON_BOX_GBIMPL_HPP */

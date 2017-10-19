/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_PARTY_GBIMPL_HPP
#define PKMN_POKEMON_PARTY_GBIMPL_HPP

#include "pokemon_party_impl.hpp"

#include "pokemon_gen1impl.hpp"
#include "pokemon_gen2impl.hpp"

#define POKEMON_PARTY_GBIMPL_TEMPLATE \
    template \
    <typename list_type, \
     typename pksav_pc_pokemon_type, \
     typename pksav_pokemon_party_data_type, \
     typename pksav_party_pokemon_type, \
     typename libpkmn_pokemon_type>

#define POKEMON_PARTY_GBIMPL_CLASS \
    pokemon_party_gbimpl< \
        list_type, \
        pksav_pc_pokemon_type, \
        pksav_pokemon_party_data_type, \
        pksav_party_pokemon_type, \
        libpkmn_pokemon_type>


namespace pkmn {

    POKEMON_PARTY_GBIMPL_TEMPLATE
    class pokemon_party_gbimpl: public pokemon_party_impl {
        public:
            pokemon_party_gbimpl() {}
            explicit pokemon_party_gbimpl(
                int game_id
            );
            pokemon_party_gbimpl(
                int game_id,
                list_type* native
            );

            ~pokemon_party_gbimpl();

            int get_num_pokemon() override final;

            void set_pokemon(
                int index,
                pkmn::pokemon::sptr new_pokemon
            ) override final;

        private:

            void _from_native() override final;
    };

    // Generation I
    typedef pokemon_party_gbimpl<
                pksav_gen1_pokemon_party_t,
                pksav_gen1_pc_pokemon_t,
                pksav_gen1_pokemon_party_data_t,
                pksav_gen1_party_pokemon_t,
                pokemon_gen1impl> pokemon_party_gen1impl;

    // Generation II
    typedef pokemon_party_gbimpl<
                pksav_gen2_pokemon_party_t,
                pksav_gen2_pc_pokemon_t,
                pksav_gen2_pokemon_party_data_t,
                pksav_gen2_party_pokemon_t,
                pokemon_gen2impl> pokemon_party_gen2impl;
}

#include "pokemon_party_gbimpl.ipp"

#endif /* PKMN_POKEMON_PARTY_GBIMPL_HPP */

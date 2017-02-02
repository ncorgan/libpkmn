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

namespace pkmn {

    template <typename list_type, typename pksav_pokemon_type, typename libpkmn_pokemon_type>
    class pokemon_party_gbimpl: public pokemon_party_impl {
        public:
            pokemon_party_gbimpl() {}
            pokemon_party_gbimpl(
                int game_id
            );
            pokemon_party_gbimpl(
                int game_id,
                list_type* native
            );

            ~pokemon_party_gbimpl();

            int get_num_pokemon();

            void set_pokemon(
                int index,
                pkmn::pokemon::sptr new_pokemon
            );

        private:

            std::string _party_name;

            void _from_native();
    };

    // Generation I
    typedef pokemon_party_gbimpl<pksav_gen1_pokemon_party_t, pksav_gen1_pc_pokemon_t, pokemon_gen1impl> pokemon_party_gen1impl;

    // Generation II
    typedef pokemon_party_gbimpl<pksav_gen2_pokemon_party_t, pksav_gen2_pc_pokemon_t, pokemon_gen2impl> pokemon_party_gen2impl;
}

#include "pokemon_party_gbimpl.ipp"

#endif /* PKMN_POKEMON_PARTY_GBIMPL_HPP */

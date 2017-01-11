/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_GB_LIST_HPP
#define PKMN_POKEMON_GB_LIST_HPP

#ifndef POKEMON_GB_LIST_BASE_IMPL
#error You must set POKEMON_GB_LIST_BASE_IMPL before including this file.
#endif

#include "pokemon_box_impl.hpp"

#include "pokemon_gen1impl.hpp"
#include "pokemon_gen2impl.hpp"

namespace pkmn {

    template <typename list_type, typename pksav_pokemon_type, typename libpkmn_pokemon_type>
    class pokemon_gb_list: public POKEMON_GB_LIST_BASE_IMPL {
        public:
            pokemon_gb_list() {}
            pokemon_gb_list(
                int game_id
            );
            pokemon_gb_list(
                int game_id,
                list_type* native
            );
            pokemon_gb_list(
                int game_id,
                const list_type &native
            );

            ~pokemon_gb_list();

            std::string get_name();

            void set_name(
                const std::string &name
            );

            int get_capacity();

            void set_pokemon(
                int index,
                pkmn::pokemon::sptr new_pokemon
            );

        private:

            std::string _box_name;

            void _from_native();
    };

    // Generation I
    typedef pokemon_gb_list<pksav_gen1_pokemon_box_t, pksav_gen1_pc_pokemon_t, pokemon_gen1impl> pokemon_box_gen1impl;

    // Generation II
    typedef pokemon_gb_list<pksav_gen2_pokemon_box_t, pksav_gen2_pc_pokemon_t, pokemon_gen2impl> pokemon_box_gen2impl;
}

#include "pokemon_gb_list.ipp"

#endif /* PKMN_POKEMON_GB_LIST_HPP */

/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_PARTY_GCNIMPL_HPP
#define PKMN_POKEMON_PARTY_GCNIMPL_HPP

#include "pokemon_party_impl.hpp"

#include "libpkmgc_includes.hpp"

#include <memory>
#include <vector>

namespace pkmn {

    class pokemon_party_gcnimpl: public pokemon_party_impl
    {
        public:
            pokemon_party_gcnimpl(
                int game_id,
                LibPkmGC::GC::Pokemon* const* pp_libpkmgc_native = nullptr
            );
            ~pokemon_party_gcnimpl() = default;

            int get_num_pokemon() final;

            void set_pokemon(
                int index,
                const pkmn::pokemon::sptr& new_pokemon
            ) final;

        private:

            int _num_pokemon;
            std::vector<std::unique_ptr<LibPkmGC::GC::Pokemon>> _libpkmgc_pokemon_uptrs;

            void _from_native() final;
            void _to_native() final;
    };
}

#endif /* PKMN_POKEMON_PARTY_GCNIMPL_HPP */

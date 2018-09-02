/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMN_POLYMORPHISM_IS_LIBPKMN_IMPL_HPP
#define INCLUDED_PKMN_POLYMORPHISM_IS_LIBPKMN_IMPL_HPP

#include <pkmn/config.hpp>

#include <pkmn/pokemon.hpp>

#include "pokemon_impl.hpp"

namespace pkmn { namespace polymorphism {

    static inline bool is_pokemon_from_libpkmn(
        const pokemon* p_pokemon
    )
    {
        const pokemon_impl* pokemon_as_libpkmn_impl =
            dynamic_cast<const pokemon_impl*>(
                p_pokemon
            );

        return (pokemon_as_libpkmn_impl != nullptr);
    }
}}

#endif /* INCLUDED_PKMN_POLYMORPHISM_IS_LIBPKMN_IMPL_HPP */

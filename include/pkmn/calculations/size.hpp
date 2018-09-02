/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_SIZE_HPP
#define PKMN_CALCULATIONS_SIZE_HPP

#include <pkmn/config.hpp>

#include <pkmn/enums/species.hpp>

#include <cstdint>
#include <string>

namespace pkmn { namespace calculations {

    /*!
     * @brief Returns a Pokémon's size, as seen by Generation III-IV NPCs
     *        looking for "large" Pokémon.
     *
     * While this height is not reflected when looking at the Pokédex, this
     * calculation is performed when bringing a Pokémon to said NPC. This
     * function returns height in meters.
     *
     * \throws std::invalid_argument if species is invalid
     * \throws std::out_of_range if any IV parameter is not in the range [0-31]
     */
    PKMN_API float pokemon_size(
        pkmn::e_species species,
        uint32_t personality,
        int IV_HP,
        int IV_attack,
        int IV_defense,
        int IV_speed,
        int IV_spatk,
        int IV_spdef
    );

}}
#endif /* PKMN_CALCULATIONS_SIZE_HPP */

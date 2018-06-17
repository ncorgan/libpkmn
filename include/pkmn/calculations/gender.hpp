/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_GENDER_HPP
#define PKMN_CALCULATIONS_GENDER_HPP

#include <pkmn/config.hpp>

#include <pkmn/enums/gender.hpp>

#include <cstdint>
#include <string>

namespace pkmn { namespace calculations {

    /*!
     * @brief Returns the gender of a Generation II Pokémon of the given species
     *        with the given Attack IV.
     *
     * In Generation II, unless the Pokémon is genderless, the Pokémon's gender depends
     * on its Attack IV. Below a species-dependent threshold, the Pokémon is female. Above
     * it, it is male.
     *
     * \param species Pokémon species
     * \param IV_attack Attack IV (1-15)
     * \throws std::runtime_error If Pokémon species is invalid
     * \throws std::out_of_range If IV_attack is out of range [0,15]
     * \returns Pokémon gender (possible values: Male, Female, Genderless)
     */
    PKMN_API pkmn::e_gender gen2_pokemon_gender(
        const std::string& species,
        int IV_attack
    );

    /*!
     * @brief Returns the gender of a Generation III+ Pokémon of the given species
     *        with the given personality.
     *
     * In Generation III+, unless the Pokémon is genderless, the Pokémon's gender
     * depends on the lowest byte of its personality. Below a species-dependent
     * threshold, the Pokémon is female. Above it, it is male.
     *
     * \param species Pokémon species
     * \param personality Pokémon personality
     * \throws std::runtime_error If Pokémon species is invalid
     * \returns Pokémon gender (possible values: Male, Female, Genderless)
     */
    PKMN_API pkmn::e_gender modern_pokemon_gender(
        const std::string& species,
        uint32_t personality
    );

}}
#endif /* PKMN_CALCULATIONS_GENDER_HPP */

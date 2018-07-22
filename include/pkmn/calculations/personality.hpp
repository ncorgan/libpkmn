/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_PERSONALITY_HPP
#define PKMN_CALCULATIONS_PERSONALITY_HPP

#include <pkmn/config.hpp>

#include <pkmn/enums/ability.hpp>
#include <pkmn/enums/gender.hpp>
#include <pkmn/enums/nature.hpp>
#include <pkmn/enums/species.hpp>

#include <cstdint>
#include <string>

namespace pkmn { namespace calculations {

    //! Generate a personality based on the given species and values.
    /*!
     * This function is RNG-based and takes an average of 30 milliseconds
     * to generate a valid value.
     *
     * \param species The Pokémon species
     * \param trainer_id The trainer ID to use in the shininess calculation
     * \param shiny Whether or not the Pokémon should be shiny
     * \param ability Which ability the Pokémon should have
     * \param gender What gender the Pokémon should be
     * \param nature What nature the Pokémon should have
     * \throws std::invalid_argument If the given ability or gender is invalid for the species
     * \throws std::invalid_argument If the given nature is invalid
     */
    PKMN_API uint32_t generate_personality(
        pkmn::e_species species,
        uint32_t trainer_id,
        bool shiny,
        pkmn::e_ability ability,
        pkmn::e_gender gender,
        pkmn::e_nature nature
    );

}}
#endif /* PKMN_CALCULATIONS_PERSONALITY_HPP */

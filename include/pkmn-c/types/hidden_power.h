/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_HIDDEN_POWER_H
#define PKMN_C_TYPES_HIDDEN_POWER_H

#include <pkmn-c/enums/type.h>

/*!
 * @brief A struct containing the values relevant to Hidden Power.
 *
 * These values are populated by ::pkmn_calculations_gen2_hidden_power
 * or ::pkmn_calculations_modern_hidden_power.
 */
struct pkmn_hidden_power
{
    //! The type of the move when used by a Pokémon with specific IVs.
    enum pkmn_type type;
    //! The base power of the move when used by a Pokémon with specific IVs.
    int base_power;
};

#endif /* PKMN_C_TYPES_HIDDEN_POWER_H */

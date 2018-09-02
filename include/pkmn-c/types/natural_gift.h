/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_NATURAL_GIFT_H
#define PKMN_C_TYPES_NATURAL_GIFT_H

/*!
 * @brief A struct containing the values relevant to Natural Gift.
 *
 * These values are populated by ::pkmn_calculations_natural_gift_stats.
 */
struct pkmn_natural_gift
{
    //! The type of the move when used by a Pokémon holding a specific item.
    enum pkmn_type type;
    //! The base power of hte move when used by a Pokémon holding a specific item.
    int base_power;
};

#endif /* PKMN_C_TYPES_NATURAL_GIFT_H */

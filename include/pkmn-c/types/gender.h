/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_GENDER_H
#define PKMN_C_TYPES_GENDER_H

//! Valid genders in the Pokémon games.
typedef enum
{
    //! Genderless.
    PKMN_GENDER_GENDERLESS,
    //! Male.
    PKMN_GENDER_MALE,
    //! Female.
    PKMN_GENDER_FEMALE
} pkmn_gender_t;

#endif /* PKMN_C_TYPES_GENDER_H */

/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_GENDER_H
#define PKMN_C_TYPES_GENDER_H

//! Valid genders in the Pokémon games.
enum pkmn_gender
{
    //! Male.
    PKMN_GENDER_MALE = 0,
    //! Female.
    PKMN_GENDER_FEMALE,
    //! Genderless.
    PKMN_GENDER_GENDERLESS
};

#endif /* PKMN_C_TYPES_GENDER_H */

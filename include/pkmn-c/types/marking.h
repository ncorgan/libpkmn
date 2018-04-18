/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_MARKING_H
#define PKMN_C_TYPES_MARKING_H

#define PKMN_NUM_MARKINGS 6

enum pkmn_marking
{
    PKMN_MARKING_CIRCLE = 0,
    PKMN_MARKING_TRIANGLE,
    PKMN_MARKING_SQUARE,
    PKMN_MARKING_HEART,
    PKMN_MARKING_STAR,
    PKMN_MARKING_DIAMOND,
};

#endif /* PKMN_C_TYPES_MARKING_H */

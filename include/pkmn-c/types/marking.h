/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_MARKING_H
#define PKMN_C_TYPES_MARKING_H

/*!
 * @brief The number of valid markings.
 *
 * Use this as the size of the buffer to pass into ::pkmn_pokemon_get_markings
 * in order to receive all marking values.
 */
#define PKMN_NUM_MARKINGS 6

/*!
 * @brief An enum corresponding to all valid markings.
 *
 * All values in this enum are valid inputs to ::pkmn_pokemon_set_has_marking
 * and can be used as indices to access a full buffer returned from
 * ::pkmn_pokemon_get_markings.
 */
typedef enum
{
    //! Circle.
    PKMN_MARKING_CIRCLE = 0,
    //! Triangle.
    PKMN_MARKING_TRIANGLE,
    //! Square.
    PKMN_MARKING_SQUARE,
    //! Heart.
    PKMN_MARKING_HEART,
    //! Star (Generation IV+).
    PKMN_MARKING_STAR,
    //! Diamond (Generation IV+).
    PKMN_MARKING_DIAMOND,
} pkmn_marking_t;

#endif /* PKMN_C_TYPES_MARKING_H */

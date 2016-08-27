/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_SPINDA_SPOTS_H
#define PKMN_C_TYPES_SPINDA_SPOTS_H

typedef struct {
    int x;
    int y;
} pkmn_spinda_coords_t;

typedef struct {
    pkmn_spinda_coords_t left_ear;
    pkmn_spinda_coords_t right_ear;
    pkmn_spinda_coords_t left_face;
    pkmn_spinda_coords_t right_face;
} pkmn_spinda_spots_t;

#endif /* PKMN_C_TYPES_SPINDA_SPOTS_H */

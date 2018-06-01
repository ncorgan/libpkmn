/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_TIME_DURATION_H
#define PKMN_C_TYPES_TIME_DURATION_H

struct pkmn_time_duration
{
    int hours;
    int minutes;
    int seconds;
    int frames;
};

#endif /* PKMN_C_TYPES_TIME_DURATION_H */

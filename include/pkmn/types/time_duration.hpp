/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_TYPES_TIME_DURATION_HPP
#define PKMN_TYPES_TIME_DURATION_HPP

#include <pkmn/config.hpp>

namespace pkmn
{
    struct time_duration
    {
        int hours;
        int minutes;
        int seconds;
        int frames;

        PKMN_CONSTEXPR_OR_INLINE time_duration():
            hours(0),
            minutes(0),
            seconds(0),
            frames(0)
        {}

        PKMN_CONSTEXPR_OR_INLINE time_duration(
            int hrs,
            int mins,
            int secs,
            int frms
        ): hours(hrs), minutes(mins), seconds(secs), frames(frms)
        {}

        PKMN_CONSTEXPR_OR_INLINE time_duration(const time_duration&) = default;
        time_duration& operator=(const time_duration&) = default;

#ifndef SWIG
        PKMN_CONSTEXPR_OR_INLINE time_duration(time_duration&&) = default;
        time_duration& operator=(time_duration&&) = default;
#endif

        PKMN_CONSTEXPR_OR_INLINE bool operator==(
            const time_duration& rhs
        ) const
        {
            return (this->hours == rhs.hours) &&
                   (this->minutes == rhs.minutes) &&
                   (this->seconds == rhs.seconds) &&
                   (this->frames == rhs.frames);
        };

        PKMN_CONSTEXPR_OR_INLINE bool operator!=(
            const time_duration& rhs
        ) const
        {
            return !operator==(rhs);
        }
    };

}

#endif /* PKMN_TYPES_TIME_DURATION_HPP */

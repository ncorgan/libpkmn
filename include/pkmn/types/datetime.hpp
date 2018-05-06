/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_TYPES_DATETIME_HPP
#define PKMN_TYPES_DATETIME_HPP

#include <pkmn/config.hpp>

namespace pkmn {

    struct datetime {
        int year;
        int month;
        int day;

        int hour;
        int minute;
        int second;

        int frames;

        PKMN_CONSTEXPR_OR_INLINE datetime():
            year(0), month(0), day(0),
            hour(0), minute(0), second(0),
            frames(0) {}

        PKMN_CONSTEXPR_OR_INLINE datetime(
            int _year, int _month, int _day,
            int _hour, int _minute, int _second,
            int _frames
        ): year(_year), month(_month), day(_day),
           hour(_hour), minute(_minute), second(_second),
           frames(_frames) {}

        PKMN_CONSTEXPR_OR_INLINE datetime(
            const datetime &other
        ): year(other.year), month(other.month), day(other.day),
           hour(other.hour), minute(other.minute), second(other.second),
           frames(other.frames) {}

        PKMN_CONSTEXPR_OR_INLINE bool operator==(
            const datetime &rhs
        ) const {
            return (this->year == rhs.year) and
                   (this->month == rhs.month) and
                   (this->day == rhs.day) and
                   (this->hour == rhs.hour) and
                   (this->minute == rhs.minute) and
                   (this->second == rhs.second) and
                   (this->frames == rhs.frames);
        }

        PKMN_CONSTEXPR_OR_INLINE bool operator!=(
            const datetime &rhs
        ) const {
            return (this->year != rhs.year) or
                   (this->month != rhs.month) or
                   (this->day != rhs.day) or
                   (this->hour != rhs.hour) or
                   (this->minute != rhs.minute) or
                   (this->second != rhs.second) or
                   (this->frames != rhs.frames);
        }
    };

    PKMN_API pkmn::datetime current_datetime();

}

#endif /* PKMN_TYPES_DATETIME_HPP */

/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
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
                   (this->month == rhs.day) and
                   (this->month == rhs.hour) and
                   (this->month == rhs.minute) and
                   (this->month == rhs.second) and
                   (this->month == rhs.frames);
        }

        PKMN_CONSTEXPR_OR_INLINE bool operator!=(
            const datetime &rhs
        ) const {
            return (this->year != rhs.year) or
                   (this->month != rhs.month) or
                   (this->month != rhs.day) or
                   (this->month != rhs.hour) or
                   (this->month != rhs.minute) or
                   (this->month != rhs.second) or
                   (this->month != rhs.frames);
        }
    };

    PKMN_API pkmn::datetime current_datetime();

}

#endif /* PKMN_TYPES_DATETIME_HPP */

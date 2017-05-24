/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/exception.hpp>

#include <pkmn/types/datetime.hpp>

#include "datetime_internal.hpp"

#include <boost/config.hpp>

#include <ctime>

namespace pkmn {

    pkmn::datetime current_datetime() {
        time_t now = std::time(NULL);
        struct tm* now_tm = std::localtime(&now);

        pkmn::datetime ret;
        ret.year = now_tm->tm_year + 1900; // struct tm stores years since 1900
        ret.month = now_tm->tm_mon + 1;
        ret.day = now_tm->tm_mday;
        ret.hour = now_tm->tm_hour;
        ret.minute = now_tm->tm_min;
        ret.second = now_tm->tm_sec;
        ret.frames = 0;

        return ret;
    }

    BOOST_STATIC_CONSTEXPR int MONTH_DAYS[] = {
        31,28,31,30,31,30,31,31,30,31,30,31
    };

    void libpkmn_datetime_to_pksav_date(
        const pkmn::datetime &libpkmn_date,
        pksav_date_t* pksav_date_out
    ) {
        // Validate inputs, since this can come from a user.
        if(libpkmn_date.year < 2000) {
            throw std::out_of_range("year: minimum value 2000");
        }
        if(libpkmn_date.month < 1 or libpkmn_date.month > 12) {
            pkmn::throw_out_of_range("month", 1, 12);
        }
        if(libpkmn_date.day < 1 or libpkmn_date.day > MONTH_DAYS[libpkmn_date.month-1]) {
            pkmn::throw_out_of_range("day", 1, MONTH_DAYS[libpkmn_date.month-1]);
        }

        // Games store number of years since 2000.
        pksav_date_out->year = uint8_t(libpkmn_date.year - 2000);
        pksav_date_out->month = uint8_t(libpkmn_date.month);
        pksav_date_out->day = uint8_t(libpkmn_date.day);
    }

}

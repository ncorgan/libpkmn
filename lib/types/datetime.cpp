/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/types/datetime.hpp>

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

}

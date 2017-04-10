/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMN_DATETIME_INTERNAL_HPP
#define INCLUDED_PKMN_DATETIME_INTERNAL_HPP

#include <pkmn/config.hpp>

#include <pksav/common/datetime.h>

namespace pkmn {

    PKMN_INLINE pkmn::datetime pksav_date_to_libpkmn_datetime(
        const pksav_date_t* pksav_date
    ) {
        return pkmn::datetime(
                   // Games store number of years since 2000.
                   pksav_date->year + 2000,
                   pksav_date->month,
                   pksav_date->day,
                   0,0,0,
                   0
               );
    }

    void libpkmn_datetime_to_pksav_date(
        const pkmn::datetime &libpkmn_date,
        pksav_date_t* pksav_date_out
    );

}

#endif /* INCLUDED_PKMN_DATETIME_INTERNAL_HPP */

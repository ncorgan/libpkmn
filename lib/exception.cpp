/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/exception.hpp>

#include <pksav/error.h>

#include <boost/format.hpp>

namespace pkmn {

    pksav_error::pksav_error(
        int pksav_error_code
    ): std::runtime_error(
          str(boost::format("PKSav returned the following error: \"%s\"")
              % pksav_strerror(pksav_error_t(pksav_error_code)))
       ) {}

    range_error::range_error(
        const std::string &value,
        int min,
        int max
    ): std::out_of_range(
           str(boost::format("%s: valid values %d-%d")
               % value.c_str() % min % max)
       ) {}

}

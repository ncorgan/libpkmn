/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMN_PKSAV_PKSAV_CALL_HPP
#define INCLUDED_PKMN_PKSAV_PKSAV_CALL_HPP

#include <pksav/error.h>

#include <pkmn/exception.hpp>

#define PKSAV_CALL(...) \
{ \
    pksav_error_t status = __VA_ARGS__; \
    if(status != PKSAV_ERROR_NONE) { \
        throw pkmn::pksav_error(status); \
    } \
}

#endif /* INCLUDED_PKMN_PKSAV_PKSAV_CALL_HPP */

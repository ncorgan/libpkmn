/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMN_PKSAV_PKSAV_CALL_HPP
#define INCLUDED_PKMN_PKSAV_PKSAV_CALL_HPP

#include <pksav/error.h>

#include <pkmn/exception.hpp>

// TODO: remove :: when exception moved
#define PKSAV_CALL(...) \
{ \
    enum ::pksav_error error = __VA_ARGS__; \
    if(error) \
    { \
        throw pkmn::pksav_error(error); \
    } \
}

#endif /* INCLUDED_PKMN_PKSAV_PKSAV_CALL_HPP */

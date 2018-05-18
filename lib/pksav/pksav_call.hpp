/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMN_PKSAV_PKSAV_CALL_HPP
#define INCLUDED_PKMN_PKSAV_PKSAV_CALL_HPP

#include "exception_internal.hpp"

#include <pksav/error.h>

#include <pkmn/exception.hpp>

#include <boost/format.hpp>

#define PKSAV_CALL(...) \
{ \
    enum pksav_error status = __VA_ARGS__; \
    if(status != PKSAV_ERROR_NONE) \
    { \
        pkmn::throw_internal_error<std::runtime_error>( \
            str(boost::format("PKSav returned the following error: \"%s\"") \
                % pksav_strerror(status)) \
        ); \
    } \
}

#endif /* INCLUDED_PKMN_PKSAV_PKSAV_CALL_HPP */

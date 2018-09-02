/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_ERROR_H
#define PKMN_C_ERROR_H

#include <pkmn-c/config.h>

//! A list of possible error codes.
enum pkmn_error
{
    //! No error.
    PKMN_ERROR_NONE = 0,

    //! An exposed function is not implemented for a given game.
    PKMN_ERROR_UNIMPLEMENTED_ERROR = 10,
    //! A given functionality does not exist in the given game.
    PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR = 11,
    //! A user tried to use an optional feature not built into in this installation.
    PKMN_ERROR_FEATURE_NOT_IN_BUILD_ERROR = 12,
    //! The core library threw an internal error.
    PKMN_ERROR_INTERNAL_ERROR = 13,

    //! A null parameter is passed into a function.
    PKMN_ERROR_NULL_POINTER = 20,

    //! The core library threw a std::invalid_argument.
    PKMN_ERROR_INVALID_ARGUMENT = 30,
    //! The core library threw a std::domain_error.
    PKMN_ERROR_DOMAIN_ERROR = 31,
    //! The core library threw a std::length_error.
    PKMN_ERROR_LENGTH_ERROR = 32,
    //! The core library threw a std::out_of_range.
    PKMN_ERROR_OUT_OF_RANGE = 33,
    //! The core library threw a std::logic_error.
    PKMN_ERROR_LOGIC_ERROR = 39,

    //! The core library threw a std::range_error.
    PKMN_ERROR_RANGE_ERROR = 40,
    //! The core library threw a std::overflow_error.
    PKMN_ERROR_OVERFLOW_ERROR = 41,
    //! The core library threw a std::underflow_error.
    PKMN_ERROR_UNDERFLOW_ERROR = 42,
    //! The core library threw a std::runtime_error.
    PKMN_ERROR_RUNTIME_ERROR = 49,

    //! The core library threw a std::exception.
    PKMN_ERROR_STD_EXCEPTION = 50,

    //! The core library threw something not derived from a std::exception.
    PKMN_ERROR_UNKNOWN_ERROR = 60
};

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Returns an error elaboration for the last error returned by LibPKMN.
 *
 * The string returned from this function is guaranteed to be non-NULL and will
 * contain the string "None." if the previous call succeeded.
 */
PKMN_C_API const char* pkmn_strerror();

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ERROR_H */

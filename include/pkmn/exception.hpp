/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_EXCEPTION_HPP
#define PKMN_EXCEPTION_HPP

#include <pkmn/config.hpp>

#include <stdexcept>
#include <string>

#ifdef PKMN_PLATFORM_WIN32
#    pragma warning(disable: 4275) // An exported class was derived from a class that was not exported.
#endif

namespace pkmn {

    //! An exception thrown when a given functionality does not exist in the given game.
    /*!
     * As the series progressed, features were added, so when these newer features are
     * requested for older games, this is the exception thrown.
     *
     * For examples, contests and ribbons were introduced in Generation III, so calling
     * any function involving ribbons in Generation I-II will throw this exception.
     */
    class PKMN_API feature_not_in_game_error: public std::runtime_error {
        public:
            //! Throw the exception with the given message.
            feature_not_in_game_error(
                const std::string &msg
            );
            //! Throw the exception for the given feature and game.
            feature_not_in_game_error(
                const std::string &feature,
                const std::string &game
            );
    };

    //! An exception thrown when a non-success error code is returned from PKSav.
    /*!
     * LibPKMN uses PKSav, a C library, under the hood for game save parsing, and this exception
     * is used to propagate its error codes into exceptions.
     *
     * PKSav error codes can be found
     * <a href="https://github.com/ncorgan/pksav/blob/master/include/pksav/error.h">here</a>.
     */
    class PKMN_API pksav_error: public std::runtime_error {
        public:
            //! Throw an exception for the given PKSav error code.
            pksav_error(
                int pksav_error_code
            );
    };

    //! A convenience subclass for throwing std::out_of_range with a given message format.
    class PKMN_API range_error: public std::out_of_range {
        public:
            //! Throw the exception for the given property and range.
            range_error(
                const std::string &value,
                int min,
                int max
            );
    };

    //! An exception thrown when an exposed function is not implemented for a given game.
    class unimplemented_error: public std::runtime_error {
        public:
            unimplemented_error():
                std::runtime_error("Currently unimplemented.")
            {}
    };

}

#ifdef PKMN_PLATFORM_WIN32
#    pragma warning(default: 4275)
#endif

#endif /* PKMN_EXCEPTION_HPP */

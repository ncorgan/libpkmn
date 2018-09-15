/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_EXCEPTION_HPP
#define PKMN_EXCEPTION_HPP

#include <pkmn/config.hpp>

#include <pkmn/enums/game.hpp>

#include <stdexcept>
#include <string>

#ifdef PKMN_PLATFORM_WIN32
#    pragma warning(disable: 4275) // An exported class was derived from a class that was not exported.
#endif

namespace pkmn {

    //! An exception thrown when a user tries to use an optional feature not built into in this installation.
    class PKMN_API feature_not_in_build_error: public std::runtime_error
    {
        public:
            //! Throw the exception for the given feature.
            feature_not_in_build_error(
                const std::string& feature
            );
    };

    //! An exception thrown when a given functionality does not exist in the given game.
    /*!
     * As the series progressed, features were added, so when these newer features are
     * requested for older games, this is the exception thrown.
     *
     * For examples, contests and ribbons were introduced in Generation III, so calling
     * any function involving ribbons in Generation I-II will throw this exception.
     */
    class PKMN_API feature_not_in_game_error: public std::runtime_error
    {
        public:
            //! Throw the exception with the given message.
            feature_not_in_game_error(
                const std::string& msg
            );
            //! Throw the exception for the given feature and game.
            feature_not_in_game_error(
                const std::string& feature,
                const std::string& game
            );
            //! Throw the exception for the given feature and game.
            feature_not_in_game_error(
                const std::string& feature,
                pkmn::e_game game
            );
    };

    //! An exception thrown when an exposed function is not implemented for a given game.
    class unimplemented_error: public std::runtime_error
    {
        public:
            unimplemented_error():
                std::runtime_error("Currently unimplemented.")
            {}
            unimplemented_error(const std::string& feature):
                std::runtime_error("Currently unimplemented: " + feature)
            {}
    };
}

#ifdef PKMN_PLATFORM_WIN32
#    pragma warning(default: 4275)
#endif

#endif /* PKMN_EXCEPTION_HPP */

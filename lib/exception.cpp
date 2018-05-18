/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/exception.hpp>

#include <pksav/error.h>

#include <boost/format.hpp>

namespace pkmn {

    feature_not_in_build_error::feature_not_in_build_error(
        const std::string &feature
    ): std::runtime_error(
        str(boost::format("This feature is not part of this build of LibPKMN: %s")
            % feature.c_str())
       ) {}

    feature_not_in_game_error::feature_not_in_game_error(
        const std::string &msg
    ): std::runtime_error(msg) {}

    feature_not_in_game_error::feature_not_in_game_error(
        const std::string &feature,
        const std::string &game
    ): std::runtime_error(
        str(boost::format("%s not in %s")
            % feature.c_str() % game.c_str())
       ) {}
}

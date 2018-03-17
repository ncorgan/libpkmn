/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMN_PRIVATE_FUNCTIONS_HPP
#define INCLUDED_PKMN_PRIVATE_FUNCTIONS_HPP

#include <pkmn/config.hpp>

#include <string>

/*
 * Helper functions for other LibPKMN DLL's to use that require internal
 * functionality. 
 */

namespace pkmn { namespace priv {

    PKMN_API int game_name_to_generation(
        const std::string& game
    );

}}

#endif /* INCLUDED_PKMN_PRIVATE_FUNCTIONS_HPP */

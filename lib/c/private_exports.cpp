/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "error_internal.hpp"

#include "private_exports.h"
#include "private_exports.hpp"

enum pkmn_error pkmn_priv_initialize_database_connection()
{
    auto impl = [&]()
    {
        pkmn::priv::initialize_database_connection();
    };

    return pkmn::c::handle_exceptions(impl);
}

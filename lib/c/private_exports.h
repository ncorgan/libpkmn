/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_C_PRIVATE_EXPORT_H
#define PKMN_C_PRIVATE_EXPORT_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_priv_initialize_database_connection();

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_PRIVATE_EXPORT_H */

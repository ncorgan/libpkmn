/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef GBA_GAME_SAVE_TESTS_H
#define GBA_GAME_SAVE_TESTS_H

#include <pkmn-c/game_save.h>

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API void pkmntest_game_save_test_common_fields(
    pkmn_game_save_handle_t save
);

#ifdef __cplusplus
}
#endif

#endif /* GBA_GAME_SAVE_TESTS_H */

/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/game_save_test.hpp>

namespace pkmntest {

static const game_save_test_params_t params[] = {
    game_save_test_params_t("Red/Blue/Yellow", "Red", "red_blue/pokemon_red.sav"),
    game_save_test_params_t("Red/Blue/Yellow", "Yellow", "yellow/pokemon_yellow.sav"),
    game_save_test_params_t("Gold/Silver", "Gold", "gold_silver/pokemon_gold.sav"),
    game_save_test_params_t("Crystal", "Crystal", "crystal/pokemon_crystal.sav"),
    game_save_test_params_t("Ruby/Sapphire", "Ruby", "ruby_sapphire/pokemon_ruby.sav"),
    game_save_test_params_t("Emerald", "Emerald", "emerald/pokemon_emerald.sav"),
    game_save_test_params_t("FireRed/LeafGreen", "FireRed", "firered_leafgreen/pokemon_firered.sav")
};

TEST_P(game_save_test, game_save_test) {
    pkmn::game_save::sptr save = get_game_save();

    pkmntest::game_save_test_common_fields(save);
}

INSTANTIATE_TEST_CASE_P(
    cpp_game_save_test,
    game_save_test,
    ::testing::ValuesIn(params)
);

}

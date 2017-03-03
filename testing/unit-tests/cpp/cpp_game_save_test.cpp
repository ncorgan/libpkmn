/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/game_save_test.hpp>

#include <pkmn/utils/paths.hpp>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

namespace fs = boost::filesystem;

namespace pkmntest {

static const fs::path TMP_DIR(pkmn::get_tmp_dir());

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

    fs::path temp_save_path = TMP_DIR / str(boost::format("%s_%d.sav") % save->get_game().c_str() % rand());
    save->save_as(temp_save_path.string());

    pkmn::game_save::sptr save2 = pkmn::game_save::from_file(temp_save_path.string());
    pkmntest::check_two_game_saves_equal(save, save2);

    std::remove(temp_save_path.string().c_str());
}

INSTANTIATE_TEST_CASE_P(
    cpp_game_save_test,
    game_save_test,
    ::testing::ValuesIn(params)
);

}

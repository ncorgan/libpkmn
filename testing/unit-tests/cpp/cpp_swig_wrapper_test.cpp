/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "swig/modules/swig_map_wrappers.hpp"
#include "swig/modules/swig_pokemon.hpp"

#include <boost/filesystem.hpp>

#include <gtest/gtest.h>

namespace fs = boost::filesystem;

TEST(cpp_swig_wrapper_test, test_invalid_pokemon_maps)
{
    pkmn::swig::pokemon_EV_map EV_map;
    pkmn::swig::pokemon_IV_map IV_map;

    EXPECT_THROW(
        EV_map.get_EV("HP");
    , std::runtime_error);
    EXPECT_THROW(
        EV_map.set_EV("HP", 0);
    , std::runtime_error);

    EXPECT_THROW(
        IV_map.get_IV("HP");
    , std::runtime_error);
    EXPECT_THROW(
        IV_map.set_IV("HP", 0);
    , std::runtime_error);
}

TEST(cpp_swig_wrapper_test, test_pokemon_maps)
{
    pkmn::pokemon::sptr pokemon = pkmn::pokemon::make(
                                      "Bulbasaur",
                                      "FireRed",
                                      "",
                                      5
                                  );
    pkmn::swig::pokemon_EV_map EV_map(pokemon);
    pkmn::swig::pokemon_IV_map IV_map(pokemon);

    // Set EV through the Pokémon, query through the wrapper class.
    pokemon->set_EV("Attack", 25);
    EXPECT_EQ(25, pokemon->get_EVs().at("Attack"));
    EXPECT_EQ(25, EV_map.get_EV("Attack"));

    // Set EV through the wrapper class, query through the Pokémon.
    EV_map.set_EV("Defense", 5);
    EXPECT_EQ(5, EV_map.get_EV("Defense"));
    EXPECT_EQ(5, pokemon->get_EVs().at("Defense"));

    // Set IV through the Pokémon, query through the wrapper class.
    pokemon->set_IV("Attack", 11);
    EXPECT_EQ(11, pokemon->get_IVs().at("Attack"));
    EXPECT_EQ(11, IV_map.get_IV("Attack"));

    // Set IV through the wrapper class, query through the Pokémon.
    IV_map.set_IV("Defense", 2);
    EXPECT_EQ(2, IV_map.get_IV("Defense"));
    EXPECT_EQ(2, pokemon->get_IVs().at("Defense"));
}

TEST(cpp_swig_wrapper_test, test_pokemon)
{
    pkmn::swig::pokemon swig_pokemon(
                            "Bulbasaur",
                            "FireRed",
                            "",
                            5
                        );

    EXPECT_EQ("Bulbasaur", swig_pokemon.get_species());
    EXPECT_EQ("FireRed", swig_pokemon.get_game());
    EXPECT_EQ("Standard", swig_pokemon.get_form());
    EXPECT_EQ("Bulbasaur", swig_pokemon.get_database_entry().get_name());

    swig_pokemon.set_nickname("12345");
    EXPECT_EQ("12345", swig_pokemon.get_nickname());

    swig_pokemon.set_gender("Female");
    EXPECT_EQ("Female", swig_pokemon.get_gender());

    swig_pokemon.set_shininess(true);
    EXPECT_TRUE(swig_pokemon.is_shiny());

    swig_pokemon.set_held_item("Oran Berry");
    EXPECT_EQ("Oran Berry", swig_pokemon.get_held_item());

    swig_pokemon.set_trainer_name("abcdef");
    EXPECT_EQ("abcdef", swig_pokemon.get_trainer_name());

    swig_pokemon.set_trainer_public_id(0x1351);
    EXPECT_EQ(0x1351, swig_pokemon.get_trainer_public_id());

    swig_pokemon.set_trainer_secret_id(0x2135);
    EXPECT_EQ(0x2135, swig_pokemon.get_trainer_secret_id());

    swig_pokemon.set_trainer_id(0xABCDEF12);
    EXPECT_EQ(0xABCDEF12, swig_pokemon.get_trainer_id());

    swig_pokemon.set_trainer_gender("Female");
    EXPECT_EQ("Female", swig_pokemon.get_trainer_gender());

    swig_pokemon.set_friendship(100);
    EXPECT_EQ(100, swig_pokemon.get_friendship());

    swig_pokemon.set_ability("Overgrow");
    EXPECT_EQ("Overgrow", swig_pokemon.get_ability());

    swig_pokemon.set_ball("Ultra Ball");
    EXPECT_EQ("Ultra Ball", swig_pokemon.get_ball());

    swig_pokemon.set_level_met(2);
    EXPECT_EQ(2, swig_pokemon.get_level_met());

    swig_pokemon.set_original_game("Sapphire");
    EXPECT_EQ("Sapphire", swig_pokemon.get_original_game());

    swig_pokemon.set_personality(0x87654321);
    EXPECT_EQ(0x87654321, swig_pokemon.get_personality());

    swig_pokemon.set_experience(500000);
    EXPECT_EQ(500000, swig_pokemon.get_experience());

    swig_pokemon.set_level(50);
    EXPECT_EQ(50, swig_pokemon.get_level());

    // TODO: wrapped markings, ribbons, contest stats, move storage

    swig_pokemon.get_EVs().set_EV("Attack", 5);
    EXPECT_EQ(5, swig_pokemon.get_EVs().get_EV("Attack"));

    swig_pokemon.get_IVs().set_IV("Attack", 5);
    EXPECT_EQ(5, swig_pokemon.get_IVs().get_IV("Attack"));

    const std::map<std::string, int>& stats = swig_pokemon.get_stats();
    EXPECT_EQ(6, stats.size());

    EXPECT_TRUE(fs::exists(swig_pokemon.get_icon_filepath()));
    EXPECT_TRUE(fs::exists(swig_pokemon.get_sprite_filepath()));
}

/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "env.hpp"

#include "swig/modules/cpp_wrappers/attribute_maps.hpp"
#include "swig/modules/cpp_wrappers/breeding.hpp"
#include "swig/modules/cpp_wrappers/daycare.hpp"
#include "swig/modules/cpp_wrappers/daycare_helpers.hpp"
#include "swig/modules/cpp_wrappers/item_slot.hpp"
#include "swig/modules/cpp_wrappers/item_list.hpp"
#include "swig/modules/cpp_wrappers/item_bag.hpp"
#include "swig/modules/cpp_wrappers/pokedex.hpp"
#include "swig/modules/cpp_wrappers/pokedex_helpers.hpp"
#include "swig/modules/cpp_wrappers/pokemon.hpp"
#include "swig/modules/cpp_wrappers/pokemon_helpers.hpp"
#include "swig/modules/cpp_wrappers/pokemon_party.hpp"
#include "swig/modules/cpp_wrappers/pokemon_box.hpp"
#include "swig/modules/cpp_wrappers/pokemon_pc.hpp"
#include "swig/modules/cpp_wrappers/game_save.hpp"

#include <boost/filesystem.hpp>

#include <gtest/gtest.h>

namespace fs = boost::filesystem;

TEST(cpp_swig_wrapper_test, test_attribute_maps)
{
    // TODO: test with class with more attributes

    pkmn::pokemon::sptr pokemon = pkmn::pokemon::make(
                                      "Pikachu",
                                      "Red",
                                      "",
                                      5
                                  );
    pkmn::swig::numeric_attribute_map<pkmn::pokemon> attribute_map(pokemon);
    EXPECT_EQ(190, attribute_map.get_attribute("Catch rate"));
}

TEST(cpp_swig_wrapper_test, test_item_slot)
{
    pkmn::item_list::sptr item_pocket = pkmn::item_list::make(
                                            "Items",
                                            "Red"
                                        );
    pkmn::swig::item_slot first_slot(item_pocket, 0);

    // Set the item name through the native class.
    item_pocket->set_item(
        0,
        "Potion",
        50
    );
    EXPECT_EQ("Potion", first_slot.get_item());

    // Set the item through the wrapper class.
    first_slot.set_item("Master Ball");
    EXPECT_EQ("Master Ball", item_pocket->at(0).item);

    // Set the item amount through the native class.
    item_pocket->set_item(
        0,
        "Potion",
        20
    );
    EXPECT_EQ(20, first_slot.get_amount());

    // Set the amount. through the wrapper class.
    first_slot.set_amount(99);
    EXPECT_EQ(99, item_pocket->at(0).amount);

    // Add a second item so we can test both methods of deletion.
    item_pocket->set_item(
        1,
        "Repel",
        10
    );
    pkmn::swig::item_slot second_slot(item_pocket, 1);
    ASSERT_EQ("Repel", second_slot.get_item());
    ASSERT_EQ(10, second_slot.get_amount());

    // Delete an item by setting the item to "None".
    first_slot.set_item("None");
    EXPECT_EQ("Repel", first_slot.get_item());
    EXPECT_EQ(10, first_slot.get_amount());
    EXPECT_EQ("None", second_slot.get_item());
    EXPECT_EQ(0, second_slot.get_amount());

    // Delete an item by setting the amount to 0;
    first_slot.set_amount(0);
    EXPECT_EQ("None", first_slot.get_item());
    EXPECT_EQ(0, first_slot.get_amount());
    EXPECT_EQ("None", second_slot.get_item());
    EXPECT_EQ(0, second_slot.get_amount());
}

TEST(cpp_swig_wrapper_test, test_item_list)
{
    pkmn::swig::item_list swig_item_list("Items", "Red");

    EXPECT_EQ("Items", swig_item_list.get_name());
    EXPECT_EQ("Red", swig_item_list.get_game());
    EXPECT_EQ(20, swig_item_list.get_capacity());
    EXPECT_EQ(0, swig_item_list.get_num_items());

    swig_item_list.add("Potion", 1);
    EXPECT_EQ("Potion", swig_item_list.at(0).get_item());
    EXPECT_EQ(1, swig_item_list.at(0).get_amount());
    EXPECT_EQ(1, swig_item_list.get_num_items());

    swig_item_list.at(0).set_amount(0);
    EXPECT_EQ("None", swig_item_list.at(0).get_item());
    EXPECT_EQ(0, swig_item_list.at(0).get_amount());
    EXPECT_EQ(0, swig_item_list.get_num_items());

    const std::vector<std::string>& valid_items = swig_item_list.get_valid_items();
    EXPECT_GT(valid_items.size(), 0);
}

TEST(cpp_swig_wrapper_test, test_item_bag)
{
    pkmn::swig::item_bag swig_item_bag("Colosseum");

    EXPECT_EQ("Colosseum", swig_item_bag.get_game());

    const std::vector<std::string>& pocket_names = swig_item_bag.get_pocket_names();
    EXPECT_GT(pocket_names.size(), 0);

    std::vector<std::string>::const_iterator pocket_names_end = pocket_names.end();
    for(std::vector<std::string>::const_iterator pocket_name_iter = pocket_names.begin();
        pocket_name_iter != pocket_names_end;
        ++pocket_name_iter)
    {
        pkmn::swig::item_list pocket = swig_item_bag.get_pocket(*pocket_name_iter);
        EXPECT_EQ(*pocket_name_iter, pocket.get_name());
        EXPECT_EQ("Colosseum", pocket.get_game());

        std::string valid_item = pocket.get_valid_items().at(0);

        swig_item_bag.add(valid_item, 5);
        EXPECT_EQ(1, pocket.get_num_items());
        EXPECT_EQ(valid_item, pocket.at(0).get_item());
        EXPECT_EQ(5, pocket.at(0).get_amount());

        // Set through the pocket and check through another copy from the bag.
        pocket.at(0).set_amount(50);
        EXPECT_EQ(valid_item, swig_item_bag.get_pocket(*pocket_name_iter).at(0).get_item());
        EXPECT_EQ(50, swig_item_bag.get_pocket(*pocket_name_iter).at(0).get_amount());

        //Set through another copy from the bag and check through the existing pocket.
        swig_item_bag.get_pocket(*pocket_name_iter).at(0).set_amount(0);
        EXPECT_EQ("None", pocket.at(0).get_item());
        EXPECT_EQ(0, pocket.at(0).get_amount());
    }
}

TEST(cpp_swig_wrapper_test, test_pokedex)
{
    pkmn::pokedex::sptr cpp_pokedex = pkmn::pokedex::make("Red");

    pkmn::swig::pokedex swig_pokedex(cpp_pokedex);
    pkmn::swig::pokedex_has_seen_helper seen_helper = swig_pokedex.get_has_seen();
    pkmn::swig::pokedex_has_caught_helper caught_helper = swig_pokedex.get_has_caught();

    // Set from the internal class and query from the SWIG wrapper.
    cpp_pokedex->set_has_seen("Bulbasaur", true);
    cpp_pokedex->set_has_caught("Mewtwo", true);
    EXPECT_TRUE(seen_helper.get_has_seen("Bulbasaur"));
    EXPECT_TRUE(caught_helper.get_has_caught("Mewtwo"));
    EXPECT_EQ(2, swig_pokedex.get_num_seen());
    EXPECT_EQ(1, swig_pokedex.get_num_caught());
    EXPECT_EQ(2ULL, swig_pokedex.get_all_seen().size());
    EXPECT_EQ(1ULL, swig_pokedex.get_all_caught().size());

    // Set from the SWIG wrapper and check from the internal class.
    seen_helper.set_has_seen("Mewtwo", false);
    EXPECT_FALSE(cpp_pokedex->has_seen("Mewtwo"));
    EXPECT_FALSE(cpp_pokedex->has_caught("Mewtwo"));
    EXPECT_EQ(1, cpp_pokedex->get_num_seen());
    EXPECT_EQ(0, cpp_pokedex->get_num_caught());
    EXPECT_EQ(1ULL, cpp_pokedex->get_all_seen().size());
    EXPECT_EQ(0ULL, cpp_pokedex->get_all_caught().size());
}

static void test_EV_IV_keys(
    const std::vector<std::string>& map_keys
)
{
    static const std::vector<std::string> expected_keys =
    {
        "Attack",
        "Defense",
        "HP",
        "Special Attack",
        "Special Defense",
        "Speed"
    };
    EXPECT_EQ(expected_keys, map_keys);
}

TEST(cpp_swig_wrapper_test, test_pokemon_helpers)
{
    pkmn::pokemon::sptr pokemon = pkmn::pokemon::make(
                                      "Bulbasaur",
                                      "FireRed",
                                      "",
                                      5
                                  );
    pkmn::swig::EV_map EV_map(pokemon);
    pkmn::swig::IV_map IV_map(pokemon);
    pkmn::swig::marking_map marking_map(pokemon);
    pkmn::swig::ribbon_map ribbon_map(pokemon);
    pkmn::swig::contest_stat_map contest_stat_map(pokemon);

    //
    // EVs
    //

    // Test getting information on the EV map.
    EXPECT_EQ(6ULL, EV_map.size());
    test_EV_IV_keys(EV_map.keys());

    // Set EV through the Pokémon.
    pokemon->set_EV("Attack", 25);
    EXPECT_EQ(25, pokemon->get_EVs().at("Attack"));
    EXPECT_EQ(25, EV_map.get_EV("Attack"));

    // Set EV through the wrapper class.
    EV_map.set_EV("Defense", 5);
    EXPECT_EQ(5, EV_map.get_EV("Defense"));
    EXPECT_EQ(5, pokemon->get_EVs().at("Defense"));

    // Test has_key.
    EXPECT_TRUE(EV_map.has_key("HP"));
    EXPECT_FALSE(EV_map.has_key("Not a key"));

    //
    // IVs
    //

    // Test getting information on the IV map.
    EXPECT_EQ(6ULL, IV_map.size());
    test_EV_IV_keys(IV_map.keys());

    // Set IV through the Pokémon.
    pokemon->set_IV("Attack", 11);
    EXPECT_EQ(11, pokemon->get_IVs().at("Attack"));
    EXPECT_EQ(11, IV_map.get_IV("Attack"));

    // Set IV through the wrapper class.
    IV_map.set_IV("Defense", 2);
    EXPECT_EQ(2, IV_map.get_IV("Defense"));
    EXPECT_EQ(2, pokemon->get_IVs().at("Defense"));

    // Test has_key.
    EXPECT_TRUE(IV_map.has_key("HP"));
    EXPECT_FALSE(IV_map.has_key("Not a key"));

    //
    // Markings
    //

    static const std::vector<std::string> expected_markings =
    {
        "Circle",
        "Heart",
        "Square",
        "Triangle"
    };

    // Test getting information on the marking map.
    EXPECT_EQ(4ULL, marking_map.size());
    EXPECT_EQ(expected_markings, marking_map.keys());

    // Set marking through the Pokémon.
    pokemon->set_marking("Circle", true);
    EXPECT_TRUE(pokemon->get_markings().at("Circle"));
    EXPECT_TRUE(marking_map.get_marking("Circle"));

    // Set marking through the wrapper class.
    marking_map.set_marking("Square", true);
    EXPECT_TRUE(marking_map.get_marking("Square"));
    EXPECT_TRUE(pokemon->get_markings().at("Square"));

    // Test has_key.
    EXPECT_TRUE(marking_map.has_key("Circle"));
    EXPECT_FALSE(marking_map.has_key("Not a key"));

    //
    // Ribbons
    //

    // Test getting information on the ribbon map.
    // Don't bother with the 32-length vector.
    EXPECT_EQ(32ULL, ribbon_map.size());

    // Set ribbon through the Pokémon.
    pokemon->set_ribbon("Cool", true);
    EXPECT_TRUE(pokemon->get_ribbons().at("Cool"));
    EXPECT_TRUE(ribbon_map.get_ribbon("Cool"));

    // Set ribbon through the wrapper class.
    ribbon_map.set_ribbon("Champion", true);
    EXPECT_TRUE(ribbon_map.get_ribbon("Champion"));
    EXPECT_TRUE(pokemon->get_ribbons().at("Champion"));

    // Test has_key.
    EXPECT_TRUE(ribbon_map.has_key("Cool"));
    EXPECT_FALSE(ribbon_map.has_key("Not a key"));

    //
    // Contest stats
    //

    static const std::vector<std::string> expected_contest_stats =
    {
        "Beauty",
        "Cool",
        "Cute",
        "Feel",
        "Smart",
        "Tough"
    };

    // Test getting information on the contest stat map.
    EXPECT_EQ(6ULL, contest_stat_map.size());
    EXPECT_EQ(expected_contest_stats, contest_stat_map.keys());

    // Set contest stat through the Pokémon.
    pokemon->set_contest_stat("Beauty", 10);
    EXPECT_EQ(10, pokemon->get_contest_stats().at("Beauty"));
    EXPECT_EQ(10, contest_stat_map.get_contest_stat("Beauty"));

    // Set contest stat through the wrapper class.
    pokemon->set_contest_stat("Tough", 123);
    EXPECT_EQ(123, contest_stat_map.get_contest_stat("Tough"));
    EXPECT_EQ(123, pokemon->get_contest_stats().at("Tough"));

    // Test has_key.
    EXPECT_TRUE(contest_stat_map.has_key("Cool"));
    EXPECT_FALSE(contest_stat_map.has_key("Not a key"));
}

TEST(cpp_swig_wrapper_test, test_pokemon)
{
    pkmn::swig::pokemon swig_pokemon(
                             "Bulbasaur",
                             "FireRed",
                             "",
                             5
                         );

    const std::map<std::string, int>& stats = swig_pokemon.get_stats();
    EXPECT_EQ(6, stats.size());

    EXPECT_EQ("Bulbasaur", swig_pokemon.get_species());
    EXPECT_EQ("FireRed", swig_pokemon.get_game());
    EXPECT_EQ("Standard", swig_pokemon.get_form());
    EXPECT_EQ("Bulbasaur", swig_pokemon.get_database_entry().get_name());

    swig_pokemon.set_is_egg(true);
    EXPECT_TRUE(swig_pokemon.is_egg());

    swig_pokemon.set_condition("Frozen");
    EXPECT_EQ("Frozen", swig_pokemon.get_condition());

    swig_pokemon.set_nickname("12345");
    EXPECT_EQ("12345", swig_pokemon.get_nickname());

    swig_pokemon.set_gender("Female");
    EXPECT_EQ("Female", swig_pokemon.get_gender());

    swig_pokemon.set_shininess(true);
    EXPECT_TRUE(swig_pokemon.is_shiny());

    swig_pokemon.set_held_item("Oran Berry");
    EXPECT_EQ("Oran Berry", swig_pokemon.get_held_item());

    swig_pokemon.set_pokerus_duration(9);
    EXPECT_EQ(9, swig_pokemon.get_pokerus_duration());

    swig_pokemon.set_original_trainer_name("abcdef");
    EXPECT_EQ("abcdef", swig_pokemon.get_original_trainer_name());

    swig_pokemon.set_original_trainer_public_id(0x1351);
    EXPECT_EQ(0x1351, swig_pokemon.get_original_trainer_public_id());

    swig_pokemon.set_original_trainer_secret_id(0x2135);
    EXPECT_EQ(0x2135, swig_pokemon.get_original_trainer_secret_id());

    swig_pokemon.set_original_trainer_id(0xABCDEF12);
    EXPECT_EQ(0xABCDEF12, swig_pokemon.get_original_trainer_id());

    swig_pokemon.set_original_trainer_gender("Female");
    EXPECT_EQ("Female", swig_pokemon.get_original_trainer_gender());

    swig_pokemon.set_current_trainer_friendship(100);
    EXPECT_EQ(100, swig_pokemon.get_current_trainer_friendship());

    swig_pokemon.set_ability("Overgrow");
    EXPECT_EQ("Overgrow", swig_pokemon.get_ability());

    swig_pokemon.set_ball("Ultra Ball");
    EXPECT_EQ("Ultra Ball", swig_pokemon.get_ball());

    swig_pokemon.set_level_met(2);
    EXPECT_EQ(2, swig_pokemon.get_level_met());

    swig_pokemon.set_location_met("Rock Tunnel");
    EXPECT_EQ("Rock Tunnel", swig_pokemon.get_location_met());

    swig_pokemon.set_original_game("Sapphire");
    EXPECT_EQ("Sapphire", swig_pokemon.get_original_game());

    swig_pokemon.set_personality(0x87654321);
    EXPECT_EQ(0x87654321, swig_pokemon.get_personality());

    swig_pokemon.set_experience(500000);
    EXPECT_EQ(500000, swig_pokemon.get_experience());

    swig_pokemon.set_level(50);
    EXPECT_EQ(50, swig_pokemon.get_level());

    EXPECT_EQ("None", swig_pokemon.get_moves().get_move_slot(0).get_move());
    EXPECT_EQ(0, swig_pokemon.get_moves().get_move_slot(0).get_pp());

    swig_pokemon.get_moves().get_move_slot(0).set_move("Fissure");
    EXPECT_EQ("Fissure", swig_pokemon.get_moves().get_move_slot(0).get_move());
    EXPECT_EQ(5, swig_pokemon.get_moves().get_move_slot(0).get_pp());

    swig_pokemon.get_moves().get_move_slot(0).set_pp(2);
    EXPECT_EQ(2, swig_pokemon.get_moves().get_move_slot(0).get_pp());

    swig_pokemon.set_current_hp(stats.at("HP")-1);
    EXPECT_EQ(stats.at("HP")-1, swig_pokemon.get_current_hp());

    swig_pokemon.get_EVs().set_EV("Attack", 5);
    EXPECT_EQ(5, swig_pokemon.get_EVs().get_EV("Attack"));

    swig_pokemon.get_IVs().set_IV("Attack", 5);
    EXPECT_EQ(5, swig_pokemon.get_IVs().get_IV("Attack"));

    swig_pokemon.get_markings().set_marking("Triangle", true);
    EXPECT_TRUE(swig_pokemon.get_markings().get_marking("Triangle"));

    swig_pokemon.get_ribbons().set_ribbon("Cool Hyper", true);
    EXPECT_TRUE(swig_pokemon.get_ribbons().get_ribbon("Cool Hyper"));

    swig_pokemon.get_contest_stats().set_contest_stat("Smart", 5);
    EXPECT_EQ(5, swig_pokemon.get_contest_stats().get_contest_stat("Smart"));

    EXPECT_TRUE(fs::exists(swig_pokemon.get_icon_filepath()));
    EXPECT_TRUE(fs::exists(swig_pokemon.get_sprite_filepath()));
}

TEST(cpp_swig_wrapper_test, test_pokemon_party)
{
    pkmn::swig::pokemon_party swig_pokemon_party("FireRed");

    EXPECT_EQ("FireRed", swig_pokemon_party.get_game());
    EXPECT_EQ(0, swig_pokemon_party.get_num_pokemon());

    for(int i = 0; i < 6; ++i)
    {
        EXPECT_EQ("None", swig_pokemon_party.get_pokemon(i).get_species());
    }

    pkmn::swig::pokemon new_pokemon("Charmander", "FireRed", "", 10);
    swig_pokemon_party.set_pokemon(0, new_pokemon);

    EXPECT_EQ("Charmander", swig_pokemon_party.get_pokemon(0).get_species());
}

TEST(cpp_swig_wrapper_test, test_pokemon_box)
{
    pkmn::swig::pokemon_box swig_pokemon_box("FireRed");

    EXPECT_EQ("FireRed", swig_pokemon_box.get_game());
    EXPECT_EQ(0, swig_pokemon_box.get_num_pokemon());

    int capacity = swig_pokemon_box.get_capacity();

    for(int i = 0; i < capacity; ++i)
    {
        EXPECT_EQ("None", swig_pokemon_box.get_pokemon(i).get_species());
    }

    pkmn::swig::pokemon new_pokemon("Charmander", "FireRed", "", 10);
    swig_pokemon_box.set_pokemon(0, new_pokemon);

    EXPECT_EQ("Charmander", swig_pokemon_box.get_pokemon(0).get_species());
}

TEST(cpp_swig_wrapper_test, test_pokemon_pc)
{
    pkmn::swig::pokemon_pc swig_pokemon_pc("FireRed");

    EXPECT_EQ("FireRed", swig_pokemon_pc.get_game());

    for(int i = 0; i < swig_pokemon_pc.get_num_boxes(); ++i)
    {
        EXPECT_EQ(
            swig_pokemon_pc.get_box_names()[i],
            swig_pokemon_pc.get_box(i).get_name()
        );
    }

    swig_pokemon_pc.get_box(4).set_name("COOL BOX");
    EXPECT_EQ("COOL BOX", swig_pokemon_pc.get_box_names()[4]);

    swig_pokemon_pc.get_box(4).set_pokemon(4, pkmn::swig::pokemon("Charizard", "FireRed", "", 50));
    EXPECT_EQ("Charizard", swig_pokemon_pc.get_box(4).get_pokemon(4).get_species());
}

TEST(cpp_swig_wrapper_test, test_game_save)
{
    static const fs::path PKSAV_TEST_SAVES(pkmn_getenv("PKSAV_TEST_SAVES"));

    fs::path save_filepath(PKSAV_TEST_SAVES / "firered_leafgreen" / "pokemon_firered.sav");

    pkmn::swig::game_save swig_game_save(save_filepath.string());

    EXPECT_EQ(save_filepath.string(), swig_game_save.get_filepath());
    EXPECT_EQ("FireRed", swig_game_save.get_game());

    swig_game_save.set_trainer_name("foobar");
    EXPECT_EQ("foobar", swig_game_save.get_trainer_name());

    swig_game_save.set_trainer_id(0xABCD1234);
    EXPECT_EQ(0xABCD1234, swig_game_save.get_trainer_id());

    swig_game_save.set_trainer_public_id(0x9753);
    EXPECT_EQ(0x9753, swig_game_save.get_trainer_public_id());

    swig_game_save.set_trainer_secret_id(0xFCA0);
    EXPECT_EQ(0xFCA0, swig_game_save.get_trainer_secret_id());

    swig_game_save.set_trainer_gender("Female");
    EXPECT_EQ("Female", swig_game_save.get_trainer_gender());

    swig_game_save.set_rival_name("abcdef");
    EXPECT_EQ("abcdef", swig_game_save.get_rival_name());

    swig_game_save.set_money(12345);
    EXPECT_EQ(12345, swig_game_save.get_money());

    swig_game_save.get_pokedex().get_has_seen().set_has_seen("Bulbasaur", true);
    EXPECT_TRUE(swig_game_save.get_pokedex().get_has_seen().get_has_seen("Bulbasaur"));

    swig_game_save.get_pokedex().get_has_caught().set_has_caught("Charmander", true);
    EXPECT_TRUE(swig_game_save.get_pokedex().get_has_caught().get_has_caught("Charmander"));

    /*
     * These are the underlying calls for a fairly representative use case. This is the
     * equivalent C# code.
     *
     * PKMN.GameSave gameSave = new PKMN.GameSave(filepath);
     * gameSave.PokemonParty[1].EVs["Attack"] = 20;
     * gameSave.PokemonPC[5][20].IVs["Attack"] = 5;
     * gameSave.ItemBag["Items"][0].Item = "Repel";
     */
    swig_game_save.get_pokemon_party().get_pokemon(1).get_EVs().set_EV("Attack", 20);
    swig_game_save.get_pokemon_pc().get_box(5).get_pokemon(20).get_IVs().set_IV("HP", 5);
    swig_game_save.get_item_bag().get_pocket("Items").at(0).set_item("Repel");

    EXPECT_EQ(20, swig_game_save.get_pokemon_party().get_pokemon(1).get_EVs().get_EV("Attack"));
    EXPECT_EQ(5, swig_game_save.get_pokemon_pc().get_box(5).get_pokemon(20).get_IVs().get_IV("HP"));
    EXPECT_EQ("Repel", swig_game_save.get_item_bag().get_pocket("Items").at(0).get_item());
}

TEST(cpp_swig_wrapper_test, test_breeding)
{
    pkmn::pokemon::sptr mother = pkmn::pokemon::make("Illumise", "Ruby", "", 50);
    pkmn::pokemon::sptr father = pkmn::pokemon::make("Volbeat", "Ruby", "", 50);

    mother->set_move("Helping Hand", 0);

    father->set_move("Helping Hand", 0);
    father->set_move("Water Pulse", 1);

    pkmn::swig::pokemon mother_swig(mother);
    pkmn::swig::pokemon father_swig(father);

    // get_child_moves

    const std::string child_species = "Illumise";

    std::vector<std::string> child_moves_cpp = pkmn::breeding::get_child_moves(
                                                   mother,
                                                   father,
                                                   child_species
                                               );
    std::vector<std::string> child_moves_swig = pkmn::swig::breeding::get_child_moves(
                                                    mother_swig,
                                                    father_swig,
                                                    child_species
                                                );
    EXPECT_EQ(child_moves_cpp, child_moves_swig);

    // get_ideal_child_IVs

    const std::string child_gender = "Female";

    std::map<std::string, int> ideal_child_IVs_cpp = pkmn::breeding::get_ideal_child_IVs(
                                                         mother,
                                                         father,
                                                         child_gender
                                                     );
    std::map<std::string, int> ideal_child_IVs_swig = pkmn::swig::breeding::get_ideal_child_IVs(
                                                          mother_swig,
                                                          father_swig,
                                                          child_gender
                                                      );
    EXPECT_EQ(ideal_child_IVs_cpp, ideal_child_IVs_swig);
}

TEST(cpp_swig_wrapper_test, test_daycare)
{
    pkmn::daycare::sptr daycare = pkmn::daycare::make("Gold");
    const pkmn::pokemon_list_t& levelup_pokemon_list = daycare->get_levelup_pokemon();
    const pkmn::pokemon_list_t& breeding_pokemon_list = daycare->get_breeding_pokemon();

    pkmn::swig::daycare daycare_swig(daycare);
    pkmn::swig::daycare_levelup_pokemon levelup_pokemon_swig(daycare);
    pkmn::swig::daycare_breeding_pokemon breeding_pokemon_swig(daycare);

    EXPECT_EQ(
        daycare->get_levelup_pokemon_capacity(),
        levelup_pokemon_swig.get_capacity()
    );
    EXPECT_EQ(
        daycare->get_breeding_pokemon_capacity(),
        breeding_pokemon_swig.get_capacity()
    );

    pkmn::pokemon::sptr new_pokemon_cpp = pkmn::pokemon::make(
                                              "Chikorita",
                                              "Gold",
                                              "",
                                              10
                                          );
    pkmn::swig::pokemon new_pokemon_swig(
                            "Cyndaquil",
                            "Gold",
                            "",
                            10
                        );

    daycare->set_levelup_pokemon(0, new_pokemon_cpp);
    EXPECT_EQ(
        levelup_pokemon_list[0],
        levelup_pokemon_swig.get_pokemon(0).get_internal()
    );

    breeding_pokemon_swig.set_pokemon(1, new_pokemon_swig);
    EXPECT_EQ(
        breeding_pokemon_swig.get_pokemon(0).get_internal(),
        breeding_pokemon_list[0]
    );
}

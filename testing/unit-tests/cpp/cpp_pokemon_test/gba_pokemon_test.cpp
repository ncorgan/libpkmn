/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/gba_pokemon_tests.hpp>
#include <pkmntest/pokemon_tests_common.hpp>

#include <pkmn/exception.hpp>
#include <pkmn/calculations/form.hpp>

#include "pksav/pksav_call.hpp"

#include <pksav/common/markings.h>
#include <pksav/common/stats.h>
#include <pksav/gba/pokemon.h>
#include <pksav/gba/text.h>
#include <pksav/math/endian.h>

#include <gtest/gtest.h>

#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

namespace fs = boost::filesystem;

static const std::map<std::string, pkmn::move_slot> none_move_slots = boost::assign::map_list_of
    ("Ruby", pkmn::move_slot(
                 pkmn::database::move_entry("None", "Ruby"),
                 0
             ))
    ("Sapphire", pkmn::move_slot(
                     pkmn::database::move_entry("None", "Sapphire"),
                     0
                 ))
    ("Emerald", pkmn::move_slot(
                    pkmn::database::move_entry("None", "Emerald"),
                    0
                ))
    ("FireRed", pkmn::move_slot(
                    pkmn::database::move_entry("None", "FireRed"),
                    0
                ))
    ("LeafGreen", pkmn::move_slot(
                      pkmn::database::move_entry("None", "LeafGreen"),
                      0
                  ))
;

static const std::map<std::string, pkmn::database::pokemon_entry> none_pokemon_entries = boost::assign::map_list_of
    ("Ruby", pkmn::database::pokemon_entry("None", "Ruby", ""))
    ("Sapphire", pkmn::database::pokemon_entry("None", "Sapphire", ""))
    ("Emerald", pkmn::database::pokemon_entry("None", "Emerald", ""))
    ("FireRed", pkmn::database::pokemon_entry("None", "FireRed", ""))
    ("LeafGreen", pkmn::database::pokemon_entry("None", "LeafGreen", ""))
;

static const std::vector<std::string> contest_types = boost::assign::list_of
    ("Cool")("Beauty")("Cute")("Smart")("Tough")
;

static const std::vector<std::string> contest_levels = boost::assign::list_of
    ("")(" Super")(" Hyper")(" Master")
;

static const std::vector<std::string> ribbons = boost::assign::list_of
    ("Champion")("Winning")("Victory")("Artist")
    ("Effort")("Marine")("Land")("Sky")
    ("Country")("National")("Earth")("World")
;

class gba_pokemon_test: public ::testing::TestWithParam<std::string> {
    public:
        static void SetUpTestCase() {
            std::srand((unsigned int)std::time(NULL));
        }

        PKMNTEST_INLINE const std::string& get_game() {
            return _game;
        }

        PKMNTEST_INLINE const std::string& get_species() {
            return _species;
        }

        PKMNTEST_INLINE pkmn::pokemon::sptr get_pokemon() {
            return _pokemon;
        }

    protected:
        void SetUp() {
            _game = GetParam();
            _species = "Torchic";
            _pokemon = pkmn::pokemon::make(_species, _game, "", 30);

            ASSERT_NE(nullptr, _pokemon.get());
            ASSERT_EQ(_species, _pokemon->get_species());
            ASSERT_EQ(_game, _pokemon->get_game());
        }

    private:

        std::string _game, _species;
        pkmn::pokemon::sptr _pokemon;
};

static void check_initial_markings_map(
    const std::map<std::string, bool>& markings_map
) {
    EXPECT_EQ(1, markings_map.count("Circle"));
    EXPECT_FALSE(markings_map.at("Circle"));
    EXPECT_EQ(1, markings_map.count("Triangle"));
    EXPECT_FALSE(markings_map.at("Triangle"));
    EXPECT_EQ(1, markings_map.count("Square"));
    EXPECT_FALSE(markings_map.at("Square"));
    EXPECT_EQ(1, markings_map.count("Heart"));
    EXPECT_FALSE(markings_map.at("Heart"));

    EXPECT_EQ(0, markings_map.count("Star"));
    EXPECT_EQ(0, markings_map.count("Diamond"));
}

static void check_initial_ribbons_map(
    const std::map<std::string, bool>& ribbons_map
) {
    for(auto contest_type_iter = contest_types.begin();
        contest_type_iter != contest_types.end();
        ++contest_type_iter)
    {
        for(auto contest_level_iter = contest_levels.begin();
            contest_level_iter != contest_levels.end();
            ++contest_level_iter)
        {
            std::string ribbon_name = str(boost::format("%s%s")
                                          % contest_type_iter->c_str()
                                          % contest_level_iter->c_str());
            EXPECT_EQ(1, ribbons_map.count(ribbon_name));
            EXPECT_FALSE(ribbons_map.at(ribbon_name));
        }
    }

    for(auto ribbon_iter = ribbons.begin();
        ribbon_iter != ribbons.end();
        ++ribbon_iter)
    {
        EXPECT_EQ(1, ribbons_map.count(*ribbon_iter));
        EXPECT_FALSE(ribbons_map.at(*ribbon_iter));
    }
}

static void check_initial_contest_stats_map(
    const std::map<std::string, int>& contest_stats_map
) {
    EXPECT_EQ(1, contest_stats_map.count("Cool"));
    EXPECT_EQ(0, contest_stats_map.at("Cool"));
    EXPECT_EQ(1, contest_stats_map.count("Beauty"));
    EXPECT_EQ(0, contest_stats_map.at("Beauty"));
    EXPECT_EQ(1, contest_stats_map.count("Cute"));
    EXPECT_EQ(0, contest_stats_map.at("Cute"));
    EXPECT_EQ(1, contest_stats_map.count("Smart"));
    EXPECT_EQ(0, contest_stats_map.at("Smart"));
    EXPECT_EQ(1, contest_stats_map.count("Tough"));
    EXPECT_EQ(0, contest_stats_map.at("Tough"));
    EXPECT_EQ(1, contest_stats_map.count("Feel"));
    EXPECT_EQ(0, contest_stats_map.at("Feel"));

    EXPECT_EQ(0, contest_stats_map.count("Sheen"));
}

static void check_initial_stats_map(
    const std::map<std::string, int>& stats_map
) {
    EXPECT_EQ(1, stats_map.count("HP"));
    EXPECT_EQ(1, stats_map.count("Attack"));
    EXPECT_EQ(1, stats_map.count("Defense"));
    EXPECT_EQ(1, stats_map.count("Speed"));
    EXPECT_EQ(0, stats_map.count("Special"));
    EXPECT_EQ(1, stats_map.count("Special Attack"));
    EXPECT_EQ(1, stats_map.count("Special Defense"));
}

static void test_markings(
    pkmn::pokemon::sptr pokemon,
    const std::string &field
) {
    std::map<std::string, bool> markings_before = pokemon->get_markings();
    pokemon->set_marking(field, true);
    const std::map<std::string, bool>& markings_after = pokemon->get_markings();
    for(auto markings_iter = markings_after.begin();
        markings_iter != markings_after.end();
        ++markings_iter)
    {
        if(markings_iter->first == field) {
            EXPECT_TRUE(markings_iter->second);
        } else {
            EXPECT_EQ(markings_before.at(markings_iter->first), markings_iter->second);
        }
    }
}

static void test_contest_ribbons(
    pkmn::pokemon::sptr pokemon,
    const std::string &contest_type
) {
    std::string ribbon_name = contest_type;
    std::string super_ribbon_name = contest_type + std::string(" Super");
    std::string hyper_ribbon_name = contest_type + std::string(" Hyper");
    std::string master_ribbon_name = contest_type + std::string(" Master");
    const std::map<std::string, bool>& ribbons = pokemon->get_ribbons();

    pokemon->set_ribbon(hyper_ribbon_name, true);
    EXPECT_TRUE(ribbons.at(ribbon_name));
    EXPECT_TRUE(ribbons.at(super_ribbon_name));
    EXPECT_TRUE(ribbons.at(hyper_ribbon_name));
    EXPECT_FALSE(ribbons.at(master_ribbon_name));

    pokemon->set_ribbon(super_ribbon_name, false);
    EXPECT_TRUE(ribbons.at(ribbon_name));
    EXPECT_FALSE(ribbons.at(super_ribbon_name));
    EXPECT_FALSE(ribbons.at(hyper_ribbon_name));
    EXPECT_FALSE(ribbons.at(master_ribbon_name));
}

static void test_ribbons(
    pkmn::pokemon::sptr pokemon,
    const std::string &ribbon
) {
    std::map<std::string, bool> ribbons_before = pokemon->get_ribbons();
    pokemon->set_ribbon(ribbon, true);
    const std::map<std::string, bool>& ribbons_after = pokemon->get_ribbons();
    for(auto ribbons_iter = ribbons_after.begin();
        ribbons_iter != ribbons_after.end();
        ++ribbons_iter)
    {
        if(ribbons_iter->first == ribbon) {
            EXPECT_TRUE(ribbons_iter->second);
        } else {
            EXPECT_EQ(ribbons_before.at(ribbons_iter->first), ribbons_iter->second);
        }
    }
}

static void test_contest_stats(
    pkmn::pokemon::sptr pokemon,
    const std::string &field
) {
    int value = std::rand() % 255;

    std::map<std::string, int> contest_stats_before = pokemon->get_contest_stats();
    pokemon->set_contest_stat(field, value);
    const std::map<std::string, int>& contest_stats_after = pokemon->get_contest_stats();
    for(auto contest_stats_iter = contest_stats_after.begin();
        contest_stats_iter != contest_stats_after.end();
        ++contest_stats_iter)
    {
        if(contest_stats_iter->first == field) {
            EXPECT_EQ(value, contest_stats_iter->second);
        } else {
            EXPECT_EQ(contest_stats_before.at(contest_stats_iter->first), contest_stats_iter->second);
        }
    }
}

static void test_IVs(
    pkmn::pokemon::sptr pokemon,
    const std::string &field
) {
    int value = std::rand() % 31;

    std::map<std::string, int> IVs_before = pokemon->get_IVs();
    pokemon->set_IV(field, value);
    const std::map<std::string, int>& IVs_after = pokemon->get_IVs();
    for(auto IVs_iter = IVs_after.begin();
        IVs_iter != IVs_after.end();
        ++IVs_iter)
    {
        if(IVs_iter->first == field) {
            EXPECT_EQ(value, IVs_iter->second);
        } else {
            EXPECT_EQ(IVs_before.at(IVs_iter->first), IVs_iter->second);
        }
    }
}

TEST_P(gba_pokemon_test, gba_pokemon_test) {
    const std::string& game = get_game();
    const std::string& species = get_species();
    pkmn::pokemon::sptr pokemon = get_pokemon();

    /*
     * Check known starting values, and confirm that we can't query values
     * that didn't exist in Generation III.
     */
    EXPECT_EQ(species, pokemon->get_species());
    EXPECT_EQ("Standard", pokemon->get_form());
    EXPECT_EQ(game, pokemon->get_game());
    EXPECT_EQ(boost::algorithm::to_upper_copy(species), pokemon->get_nickname());
    EXPECT_EQ("None", pokemon->get_held_item().get_name());
    EXPECT_EQ(pkmn::pokemon::LIBPKMN_OT_NAME, pokemon->get_trainer_name());
    EXPECT_EQ(uint16_t(pkmn::pokemon::LIBPKMN_OT_ID & 0xFFFF), pokemon->get_trainer_public_id());
    EXPECT_EQ(uint16_t((pkmn::pokemon::LIBPKMN_OT_ID & 0xFFFF0000) >> 16), pokemon->get_trainer_secret_id());
    EXPECT_EQ(pkmn::pokemon::LIBPKMN_OT_ID, pokemon->get_trainer_id());
    EXPECT_EQ("Male", pokemon->get_trainer_gender());
    EXPECT_EQ(pokemon->get_database_entry().get_base_friendship(), pokemon->get_friendship());
    EXPECT_EQ("Blaze", pokemon->get_ability());
    EXPECT_EQ("Premier Ball", pokemon->get_ball());
    EXPECT_EQ(pokemon->get_level(), pokemon->get_level_met());
    EXPECT_THROW(
        pokemon->get_location_met(true)
    , pkmn::feature_not_in_game_error);
    EXPECT_EQ("Fateful encounter", pokemon->get_location_met(false));
    EXPECT_EQ(pokemon->get_game(), pokemon->get_original_game());
    EXPECT_EQ(
        pokemon->get_database_entry().get_experience_at_level(30),
        pokemon->get_experience()
    );
    EXPECT_EQ(30, pokemon->get_level());

    check_initial_markings_map(
        pokemon->get_markings()
    );
    check_initial_ribbons_map(
        pokemon->get_ribbons()
    );
    check_initial_contest_stats_map(
        pokemon->get_contest_stats()
    );

    const pkmn::move_slots_t& move_slots = pokemon->get_moves();
    EXPECT_EQ(4, move_slots.size());
    for(auto iter = move_slots.begin(); iter != move_slots.end(); ++iter) {
        EXPECT_TRUE((*iter) == none_move_slots.at(game));
    }

    check_initial_stats_map(
        pokemon->get_EVs()
    );
    check_initial_stats_map(
        pokemon->get_IVs()
    );
    check_initial_stats_map(
        pokemon->get_stats()
    );

    EXPECT_TRUE(fs::exists(pokemon->get_icon_filepath()));
    EXPECT_TRUE(fs::exists(pokemon->get_sprite_filepath()));

    /*
     * Make sure the getters and setters agree. Also make sure it fails when
     * expected.
     */

    EXPECT_THROW(
        pokemon->set_nickname(""),
    std::invalid_argument);
    EXPECT_THROW(
        pokemon->set_nickname("Too long nickname"),
    std::invalid_argument);
    pokemon->set_nickname("foobarbaz");
    EXPECT_EQ("foobarbaz", pokemon->get_nickname());

    // Gender and personality are tied, so make sure they affect each other.
    pokemon->set_gender("Female");
    EXPECT_LT((pokemon->get_personality() & 0xFF), 0xFF);
    pokemon->set_gender("Male");
    EXPECT_EQ(0xFF, (pokemon->get_personality() & 0xFF));

    pokemon->set_personality(0x1234AB00);
    EXPECT_EQ("Female", pokemon->get_gender());
    pokemon->set_personality(0xCD5678FF);
    EXPECT_EQ("Male", pokemon->get_gender());

    // Setting shininess should affect personality. Also check filepaths.
    std::string sprite_filepath;

    pokemon->set_shininess(false);
    EXPECT_FALSE(pokemon->is_shiny());
    uint32_t personality = pokemon->get_personality();
    sprite_filepath = pokemon->get_sprite_filepath();
    EXPECT_TRUE(fs::exists(sprite_filepath));

    // This will fail if "shiny" is anywhere in the filepath.
    EXPECT_TRUE(sprite_filepath.find("shiny") == std::string::npos);

    pokemon->set_shininess(true);
    EXPECT_TRUE(pokemon->is_shiny());
    EXPECT_NE(pokemon->get_personality(), personality);
    sprite_filepath = pokemon->get_sprite_filepath();
    EXPECT_TRUE(fs::exists(sprite_filepath));
    EXPECT_TRUE(sprite_filepath.find("shiny") != std::string::npos);

    EXPECT_THROW(
        pokemon->set_held_item("Not an item");
    , std::invalid_argument);
    EXPECT_EQ("None", pokemon->get_held_item().get_name());
    EXPECT_THROW(
        pokemon->set_held_item("Berry");
    , std::invalid_argument);
    EXPECT_EQ("None", pokemon->get_held_item().get_name());
    EXPECT_THROW(
        pokemon->set_held_item("Mach Bike");
    , std::invalid_argument);
    EXPECT_EQ("None", pokemon->get_held_item().get_name());
    pokemon->set_held_item("Razz Berry");
    EXPECT_EQ("Razz Berry", pokemon->get_held_item().get_name());

    EXPECT_THROW(
        pokemon->set_trainer_name(""),
    std::invalid_argument);
    EXPECT_THROW(
        pokemon->set_trainer_name("Too long trainer name"),
    std::invalid_argument);
    pokemon->set_trainer_name("foobar");
    EXPECT_EQ("foobar", pokemon->get_trainer_name());

    pokemon->set_trainer_id(0x1234ABCD);
    EXPECT_EQ(0x1234ABCD, pokemon->get_trainer_id());
    EXPECT_EQ(0xABCD, pokemon->get_trainer_public_id());
    EXPECT_EQ(0x1234, pokemon->get_trainer_secret_id());

    pokemon->set_trainer_public_id(0x1A2B);
    EXPECT_EQ(0x12341A2B, pokemon->get_trainer_id());
    EXPECT_EQ(0x1A2B, pokemon->get_trainer_public_id());
    EXPECT_EQ(0x1234, pokemon->get_trainer_secret_id());

    pokemon->set_trainer_secret_id(0x3C4D);
    EXPECT_EQ(0x3C4D1A2B, pokemon->get_trainer_id());
    EXPECT_EQ(0x1A2B, pokemon->get_trainer_public_id());
    EXPECT_EQ(0x3C4D, pokemon->get_trainer_secret_id());

    pokemon->set_trainer_gender("Male");
    EXPECT_EQ("Male", pokemon->get_trainer_gender());
    pokemon->set_trainer_gender("Female");
    EXPECT_EQ("Female", pokemon->get_trainer_gender());
    EXPECT_THROW(
        pokemon->set_trainer_gender("Genderless");
    , std::invalid_argument);

    pokemon->set_friendship(123);
    EXPECT_EQ(123, pokemon->get_friendship());
    EXPECT_THROW(
        pokemon->set_friendship(-1);
    , pkmn::range_error);
    EXPECT_THROW(
        pokemon->set_friendship(256);
    , pkmn::range_error);

    pokemon->set_ability("Blaze");
    EXPECT_EQ("Blaze", pokemon->get_ability());
    EXPECT_THROW(
        pokemon->set_ability("None");
    , std::invalid_argument);
    EXPECT_THROW(
        pokemon->set_ability("Torrent"); // Invalid
    , std::invalid_argument);
    EXPECT_THROW(
        pokemon->set_ability("Speed Boost"); // Hidden ability
    , std::invalid_argument);
    EXPECT_EQ("Blaze", pokemon->get_ability());

    pokemon->set_ball("Great Ball");
    EXPECT_EQ("Great Ball", pokemon->get_ball());
    EXPECT_THROW(
        pokemon->set_ball("Friend Ball"); // Not in Generation III
    , std::invalid_argument);
    EXPECT_EQ("Great Ball", pokemon->get_ball());

    pokemon->set_level_met(67);
    EXPECT_EQ(67, pokemon->get_level_met());
    EXPECT_THROW(
        pokemon->set_level_met(-1);
    , pkmn::range_error);
    EXPECT_THROW(
        pokemon->set_level_met(101);
    , pkmn::range_error);

    std::string location;
    if(game == "FireRed" or game == "LeafGreen") {
        location = "Viridian Forest";
    } else {
        location = "Petalburg Woods";
    }
    pokemon->set_location_met(location, false);
    EXPECT_EQ(location, pokemon->get_location_met(false));
    EXPECT_THROW(
        pokemon->set_location_met(location, true);
    , pkmn::feature_not_in_game_error);

    pokemon->set_original_game("Ruby");
    EXPECT_EQ("Ruby", pokemon->get_original_game());

    // Make sure we can't set invalid games.
    EXPECT_THROW(
        pokemon->set_original_game("Not a game");
    , std::invalid_argument);
    EXPECT_THROW(
        pokemon->set_original_game("Red"); // Impossible
    , std::invalid_argument);
    EXPECT_THROW(
        pokemon->set_original_game("HeartGold"); // From a later game
    , std::invalid_argument);
    EXPECT_EQ("Ruby", pokemon->get_original_game());

    pokemon->set_personality(0x7F3AB3A8);
    EXPECT_EQ(0x7F3AB3A8, pokemon->get_personality());

    EXPECT_THROW(
        pokemon->set_contest_stat("Cool", -1);
    , pkmn::range_error);
    EXPECT_THROW(
        pokemon->set_contest_stat("Cool", 256);
    , pkmn::range_error);
    EXPECT_THROW(
        pokemon->set_contest_stat("Not a stat", -1);
    , std::invalid_argument);

    // Make sure functions that affect the same PKSav field don't impact each other.
    static const std::vector<std::string> markings = boost::assign::list_of
        ("Circle")("Triangle")("Square")("Heart")
    ;
    for(auto marking_iter = markings.begin();
        marking_iter != markings.end();
        ++marking_iter)
    {
        test_markings(pokemon, *marking_iter);
    }
    for(auto contest_type_iter = contest_types.begin();
        contest_type_iter != contest_types.end();
        ++contest_type_iter)
    {
        test_contest_ribbons(pokemon, *contest_type_iter);
    }

    for(auto ribbon_iter = ribbons.begin();
        ribbon_iter != ribbons.end();
        ++ribbon_iter)
    {
        test_ribbons(pokemon, *ribbon_iter);
    }

    static const std::vector<std::string> contest_fields = boost::assign::list_of
        ("Cool")("Beauty")("Cute")("Smart")("Tough")("Feel")
    ;
    for(auto contest_field_iter = contest_fields.begin();
        contest_field_iter != contest_fields.end();
        ++contest_field_iter)
    {
        test_contest_stats(pokemon, *contest_field_iter);
    }
    EXPECT_THROW(
        pokemon->set_contest_stat("Not a stat", 10);
    , std::invalid_argument);
    EXPECT_THROW(
        pokemon->set_contest_stat("Cool", -1);
    , pkmn::range_error);
    EXPECT_THROW(
        pokemon->set_contest_stat("Cool", 256);
    , pkmn::range_error);

    EXPECT_THROW(
        pokemon->set_move("Pound", -1);
    , pkmn::range_error);
    EXPECT_THROW(
        pokemon->set_move("Pound", 4);
    , pkmn::range_error);
    EXPECT_THROW(
        pokemon->set_move("Shadow Sky", 0);
    , std::invalid_argument);

    static const std::string moves[] = {
        "Swallow", "Flamethrower", "Return", "Fire Blast"
    };
    for(int i = 0; i < 4; ++i) {
        pokemon->set_move(moves[i], i);
    }
    for(int i = 0; i < 4; ++i) {
        EXPECT_EQ(moves[i], move_slots.at(i).move.get_name());
        EXPECT_EQ(move_slots.at(i).move.get_pp(0), move_slots.at(i).pp);
    }

    pokemon->set_EV("Attack", 100);
    EXPECT_EQ(100, pokemon->get_EVs().at("Attack"));
    EXPECT_THROW(
        pokemon->set_EV("Not a stat", 100);
    , std::invalid_argument);
    EXPECT_THROW(
        pokemon->set_EV("Attack", -1);
    , pkmn::range_error);
    EXPECT_THROW(
        pokemon->set_EV("Attack", 256);
    , pkmn::range_error);

    static const std::vector<std::string> IV_fields = boost::assign::list_of
        ("HP")("Attack")("Defense")("Speed")("Special Attack")("Special Defense")
    ;
    for(auto IV_field_iter = IV_fields.begin();
        IV_field_iter != IV_fields.end();
        ++IV_field_iter)
    {
        test_IVs(pokemon, *IV_field_iter);
    }
    EXPECT_THROW(
        pokemon->set_IV("Not a stat", 10);
    , std::invalid_argument);
    EXPECT_THROW(
        pokemon->set_IV("Attack", -1);
    , pkmn::range_error);
    EXPECT_THROW(
        pokemon->set_IV("Attack", 32);
    , pkmn::range_error);

    /*
     * On the C++ level, check the underlying PKSav struct and make
     * sure our abstractions match.
     */
    const pksav_gba_pc_pokemon_t* native_pc_data = reinterpret_cast<const pksav_gba_pc_pokemon_t*>(
                                                       pokemon->get_native_pc_data()
                                                   );
    const pksav_gba_pokemon_growth_t* growth = &native_pc_data->blocks.growth;
    const pksav_gba_pokemon_attacks_t* attacks = &native_pc_data->blocks.attacks;
    const pksav_gba_pokemon_effort_t* effort = &native_pc_data->blocks.effort;
    const pksav_gba_pokemon_misc_t* misc = &native_pc_data->blocks.misc;

    EXPECT_EQ(pokemon->get_personality(), pksav_littleendian32(native_pc_data->personality));
    EXPECT_EQ(pokemon->get_trainer_id(), pksav_littleendian32(native_pc_data->ot_id.id));
    EXPECT_EQ(pokemon->get_trainer_public_id(), pksav_littleendian16(native_pc_data->ot_id.pid));
    EXPECT_EQ(pokemon->get_trainer_secret_id(), pksav_littleendian16(native_pc_data->ot_id.sid));

    char nickname[11] = {0};
    PKSAV_CALL(
        pksav_text_from_gba(
            native_pc_data->nickname,
            nickname,
            10
        );
    );
    EXPECT_EQ(pokemon->get_nickname(), std::string(nickname));

    // TODO: language

    char otname[8] = {0};
    PKSAV_CALL(
        pksav_text_from_gba(
            native_pc_data->otname,
            otname,
            7
        );
    );
    EXPECT_EQ(pokemon->get_trainer_name(), std::string(otname));

    EXPECT_TRUE(native_pc_data->markings & PKSAV_MARKING_CIRCLE);
    EXPECT_TRUE(native_pc_data->markings & PKSAV_MARKING_TRIANGLE);
    EXPECT_TRUE(native_pc_data->markings & PKSAV_MARKING_SQUARE);
    EXPECT_TRUE(native_pc_data->markings & PKSAV_MARKING_HEART);

    EXPECT_EQ(pokemon->get_database_entry().get_pokemon_index(), int(pksav_littleendian16(growth->species)));
    EXPECT_EQ(pokemon->get_held_item().get_item_index(), int(pksav_littleendian16(growth->held_item)));
    EXPECT_EQ(pokemon->get_experience(), int(pksav_littleendian32(growth->exp)));
    // TODO: PP Up
    EXPECT_EQ(pokemon->get_friendship(), int(growth->friendship));

    for(int i = 0; i < 4; ++i) {
        EXPECT_EQ(move_slots.at(i).move.get_move_id(), int(attacks->moves[i]));
        EXPECT_EQ(move_slots.at(i).pp, int(attacks->move_pps[i]));
    }

    const std::map<std::string, int>& EVs = pokemon->get_EVs();
    EXPECT_EQ(EVs.at("HP"), int(pksav_littleendian16(effort->ev_hp)));
    EXPECT_EQ(EVs.at("Attack"), int(pksav_littleendian16(effort->ev_atk)));
    EXPECT_EQ(EVs.at("Defense"), int(pksav_littleendian16(effort->ev_def)));
    EXPECT_EQ(EVs.at("Speed"), int(pksav_littleendian16(effort->ev_spd)));
    EXPECT_EQ(EVs.at("Special Attack"), int(pksav_littleendian16(effort->ev_spatk)));
    EXPECT_EQ(EVs.at("Special Defense"), int(pksav_littleendian16(effort->ev_spdef)));

    const std::map<std::string, int>& contest_stats = pokemon->get_contest_stats();
    EXPECT_EQ(contest_stats.at("Cool"), int(effort->contest_stats.cool));
    EXPECT_EQ(contest_stats.at("Beauty"), int(effort->contest_stats.beauty));
    EXPECT_EQ(contest_stats.at("Cute"), int(effort->contest_stats.cute));
    EXPECT_EQ(contest_stats.at("Smart"), int(effort->contest_stats.smart));
    EXPECT_EQ(contest_stats.at("Tough"), int(effort->contest_stats.tough));
    EXPECT_EQ(contest_stats.at("Feel"), int(effort->contest_stats.feel));

    // TODO: Pokérus
    // TODO: get location indices for what we set

    EXPECT_EQ(pokemon->get_level_met(), int(misc->origin_info & PKSAV_GBA_LEVEL_MET_MASK));

    // TODO: get ball ID for what we set
    EXPECT_TRUE(misc->origin_info & PKSAV_GBA_OTGENDER_MASK);

    const std::map<std::string, int>& IVs = pokemon->get_IVs();
    uint8_t IV = 0;

    PKSAV_CALL(
        pksav_get_IV(
            &misc->iv_egg_ability,
            PKSAV_STAT_HP,
            &IV
        );
    )
    EXPECT_EQ(IVs.at("HP"), int(IV));

    PKSAV_CALL(
        pksav_get_IV(
            &misc->iv_egg_ability,
            PKSAV_STAT_ATTACK,
            &IV
        );
    )
    EXPECT_EQ(IVs.at("Attack"), int(IV));

    PKSAV_CALL(
        pksav_get_IV(
            &misc->iv_egg_ability,
            PKSAV_STAT_DEFENSE,
            &IV
        );
    )
    EXPECT_EQ(IVs.at("Defense"), int(IV));

    PKSAV_CALL(
        pksav_get_IV(
            &misc->iv_egg_ability,
            PKSAV_STAT_SPEED,
            &IV
        );
    )
    EXPECT_EQ(IVs.at("Speed"), int(IV));

    PKSAV_CALL(
        pksav_get_IV(
            &misc->iv_egg_ability,
            PKSAV_STAT_SPATK,
            &IV
        );
    )
    EXPECT_EQ(IVs.at("Special Attack"), int(IV));

    PKSAV_CALL(
        pksav_get_IV(
            &misc->iv_egg_ability,
            PKSAV_STAT_SPDEF,
            &IV
        );
    )
    EXPECT_EQ(IVs.at("Special Defense"), int(IV));

    EXPECT_TRUE(!(misc->iv_egg_ability & PKSAV_GBA_EGG_MASK));
    EXPECT_TRUE(!(misc->iv_egg_ability & PKSAV_GBA_ABILITY_MASK));

    /*
     * Party data
     */
    const pksav_gba_pokemon_party_data_t* native_party_data = reinterpret_cast<const pksav_gba_pokemon_party_data_t*>(
                                                                  pokemon->get_native_party_data()
                                                              );
    // TODO: condition

    EXPECT_EQ(pokemon->get_level(), int(native_party_data->level));

    // TODO: Pokérus

    const std::map<std::string, int>& stats = pokemon->get_stats();
    EXPECT_EQ(stats.at("HP"), int(pksav_littleendian16(native_party_data->current_hp)));
    EXPECT_EQ(stats.at("HP"), int(pksav_littleendian16(native_party_data->max_hp)));
    EXPECT_EQ(stats.at("Attack"), int(pksav_littleendian16(native_party_data->atk)));
    EXPECT_EQ(stats.at("Defense"), int(pksav_littleendian16(native_party_data->def)));
    EXPECT_EQ(stats.at("Speed"), int(pksav_littleendian16(native_party_data->spd)));
    EXPECT_EQ(stats.at("Special Attack"), int(pksav_littleendian16(native_party_data->spatk)));
    EXPECT_EQ(stats.at("Special Defense"), int(pksav_littleendian16(native_party_data->spdef)));
}

static const std::string games[] = {
    "Ruby",
    "Sapphire",
    "Emerald",
    "FireRed",
    "LeafGreen"
};

INSTANTIATE_TEST_CASE_P(
    cpp_gba_pokemon_test,
    gba_pokemon_test,
    ::testing::ValuesIn(games)
);

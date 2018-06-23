/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/util.hpp>

#include <pkmn/database/item_entry.hpp>

#include <boost/format.hpp>

#include <gtest/gtest.h>

#include <iostream>
#include <string>

class item_entry_none_test: public ::testing::TestWithParam<pkmn::e_game> {};

TEST_P(item_entry_none_test, item_entry_none_test)
{
    pkmn::database::item_entry none_entry("None", GetParam());

    EXPECT_EQ("None", none_entry.get_name());
    EXPECT_EQ("None", none_entry.get_category());
    EXPECT_EQ("None", none_entry.get_pocket());
    EXPECT_EQ("None", none_entry.get_description());
    EXPECT_EQ(-1, none_entry.get_cost());
    EXPECT_FALSE(none_entry.holdable());
    EXPECT_EQ(-1, none_entry.get_fling_power());
    EXPECT_EQ("None", none_entry.get_fling_effect());
    EXPECT_EQ(0, none_entry.get_item_id());
    EXPECT_EQ(0, none_entry.get_item_index());
    EXPECT_EQ(0, none_entry.get_item_list_id());
}

INSTANTIATE_TEST_CASE_P(
    cpp_item_entry_none_test,
    item_entry_none_test,
    ::testing::ValuesIn(pkmntest::ALL_GAMES)
);

class item_entry_test: public ::testing::Test
{
    public:
        static void SetUpTestCase() {
            byindex_gen1 = pkmn::database::item_entry(20, 1);
            byindex_gen2 = pkmn::database::item_entry(104, 5);
            byindex_gba  = pkmn::database::item_entry(148, 9);
            byindex_gcn  = pkmn::database::item_entry(536, 20);
            byindex_gen4 = pkmn::database::item_entry(492, 16);
            byindex_gen5 = pkmn::database::item_entry(638, 21);
            byindex_gen6 = pkmn::database::item_entry(769, 25);
            byname_gen1  = pkmn::database::item_entry("Potion", pkmn::e_game::RED);
            byname_gen2  = pkmn::database::item_entry("Pink Bow", pkmn::e_game::SILVER);
            byname_gba   = pkmn::database::item_entry("Razz Berry", pkmn::e_game::EMERALD);
            byname_gcn   = pkmn::database::item_entry("Battle CD 03", pkmn::e_game::XD);
            byname_gen4  = pkmn::database::item_entry("Fast Ball", pkmn::e_game::SOULSILVER);
            byname_gen5  = pkmn::database::item_entry("Reveal Glass", pkmn::e_game::BLACK2);
            byname_gen6  = pkmn::database::item_entry("Salamencite", pkmn::e_game::OMEGA_RUBY);
        }

        static pkmn::database::item_entry byindex_gen1, byname_gen1;
        static pkmn::database::item_entry byindex_gen2, byname_gen2;
        static pkmn::database::item_entry byindex_gba,  byname_gba;
        static pkmn::database::item_entry byindex_gcn,  byname_gcn;
        static pkmn::database::item_entry byindex_gen4, byname_gen4;
        static pkmn::database::item_entry byindex_gen5, byname_gen5;
        static pkmn::database::item_entry byindex_gen6, byname_gen6;
};

pkmn::database::item_entry item_entry_test::byindex_gen1;
pkmn::database::item_entry item_entry_test::byindex_gen2;
pkmn::database::item_entry item_entry_test::byindex_gba;
pkmn::database::item_entry item_entry_test::byindex_gcn;
pkmn::database::item_entry item_entry_test::byindex_gen4;
pkmn::database::item_entry item_entry_test::byindex_gen5;
pkmn::database::item_entry item_entry_test::byindex_gen6;
pkmn::database::item_entry item_entry_test::byname_gen1;
pkmn::database::item_entry item_entry_test::byname_gen2;
pkmn::database::item_entry item_entry_test::byname_gba;
pkmn::database::item_entry item_entry_test::byname_gcn;
pkmn::database::item_entry item_entry_test::byname_gen4;
pkmn::database::item_entry item_entry_test::byname_gen5;
pkmn::database::item_entry item_entry_test::byname_gen6;

/*
 * Make sure item entries can't be instantiated for games that didn't
 * have the given item.
 */
TEST_F(item_entry_test, wrong_game_test)
{
    // Items from later generations
    EXPECT_THROW(
        pkmn::database::item_entry(
            "Pink Bow", pkmn::e_game::RED
        );
    , std::invalid_argument);
    EXPECT_THROW(
        pkmn::database::item_entry(
            "Razz Berry", pkmn::e_game::SILVER
        );
    , std::invalid_argument);
    EXPECT_THROW(
        pkmn::database::item_entry(
            "Flame Plate", pkmn::e_game::EMERALD
        );
    , std::invalid_argument);
    EXPECT_THROW(
        pkmn::database::item_entry(
            "DNA Splicers", pkmn::e_game::PEARL
        )
    , std::invalid_argument);
    EXPECT_THROW(
        pkmn::database::item_entry(
            "Mega Ring", pkmn::e_game::WHITE
        )
    , std::invalid_argument);

    // Items that were only in certain games within a generation
    EXPECT_THROW(
        pkmn::database::item_entry(
            "GS Ball", pkmn::e_game::GOLD
        )
    , std::invalid_argument);
    pkmn::database::item_entry(
        "GS Ball", pkmn::e_game::CRYSTAL
    );

    EXPECT_THROW(
        pkmn::database::item_entry(
            "F-Disk", pkmn::e_game::RUBY
        )
    , std::invalid_argument);
    pkmn::database::item_entry(
        "F-Disk", pkmn::e_game::COLOSSEUM
    );
    EXPECT_THROW(
        pkmn::database::item_entry(
            "HM01", pkmn::e_game::COLOSSEUM
        )
    , std::invalid_argument);
    pkmn::database::item_entry(
        "HM01", pkmn::e_game::RUBY
    );
    EXPECT_THROW(
        pkmn::database::item_entry(
            "Claw Fossil", pkmn::e_game::COLOSSEUM
        )
    , std::invalid_argument);
    pkmn::database::item_entry(
        "Claw Fossil", pkmn::e_game::RUBY
    );

    // Items that didn't make it into later generations
    pkmn::database::item_entry(
        "Pink Bow", pkmn::e_game::SILVER
    );
    EXPECT_THROW(
        pkmn::database::item_entry(
            "Pink Bow", pkmn::e_game::RUBY
        )
    , std::invalid_argument);
}

/*
 * Make sure item entries can be created from any name
 * the item has ever had, even from different games.
 */
TEST_F(item_entry_test, different_name_test)
{
    /*
     * Test an item that changed only once.
     */

    pkmn::database::item_entry elixir1("Elixer", pkmn::e_game::RED);
    pkmn::database::item_entry elixir2("Elixir", pkmn::e_game::RED);
    EXPECT_TRUE(elixir1 == elixir2);
    EXPECT_EQ("Elixer", elixir1.get_name());
    EXPECT_EQ("Elixer", elixir2.get_name());

    pkmn::database::item_entry elixir3("Elixir", pkmn::e_game::X);
    pkmn::database::item_entry elixir4("Elixer", pkmn::e_game::X);
    EXPECT_TRUE(elixir3 == elixir4);
    EXPECT_EQ("Elixir", elixir3.get_name());
    EXPECT_EQ("Elixir", elixir4.get_name());

    EXPECT_TRUE(elixir1 != elixir3);

    /*
     * Test an item that changed twice.
     */

    pkmn::database::item_entry dowsing_machine1("Itemfinder", pkmn::e_game::RED);
    pkmn::database::item_entry dowsing_machine2("Dowsing MCHN", pkmn::e_game::RED);
    pkmn::database::item_entry dowsing_machine3("Dowsing Machine", pkmn::e_game::RED);
    EXPECT_TRUE(dowsing_machine1 == dowsing_machine2);
    EXPECT_TRUE(dowsing_machine2 == dowsing_machine3);
    EXPECT_EQ("Itemfinder", dowsing_machine1.get_name());
    EXPECT_EQ("Itemfinder", dowsing_machine2.get_name());
    EXPECT_EQ("Itemfinder", dowsing_machine3.get_name());

    pkmn::database::item_entry dowsing_machine4("Itemfinder", pkmn::e_game::HEARTGOLD);
    pkmn::database::item_entry dowsing_machine5("Dowsing MCHN", pkmn::e_game::HEARTGOLD);
    pkmn::database::item_entry dowsing_machine6("Dowsing Machine", pkmn::e_game::HEARTGOLD);
    EXPECT_TRUE(dowsing_machine4 == dowsing_machine5);
    EXPECT_TRUE(dowsing_machine5 == dowsing_machine6);
    EXPECT_EQ("Dowsing MCHN", dowsing_machine4.get_name());
    EXPECT_EQ("Dowsing MCHN", dowsing_machine5.get_name());
    EXPECT_EQ("Dowsing MCHN", dowsing_machine6.get_name());

    pkmn::database::item_entry dowsing_machine7("Itemfinder", pkmn::e_game::X);
    pkmn::database::item_entry dowsing_machine8("Dowsing MCHN", pkmn::e_game::X);
    pkmn::database::item_entry dowsing_machine9("Dowsing Machine", pkmn::e_game::X);
    EXPECT_TRUE(dowsing_machine7 == dowsing_machine8);
    EXPECT_TRUE(dowsing_machine8 == dowsing_machine9);
    EXPECT_EQ("Dowsing Machine", dowsing_machine7.get_name());
    EXPECT_EQ("Dowsing Machine", dowsing_machine8.get_name());
    EXPECT_EQ("Dowsing Machine", dowsing_machine9.get_name());

    EXPECT_TRUE(dowsing_machine1 != dowsing_machine4);
    EXPECT_TRUE(dowsing_machine4 != dowsing_machine7);
    EXPECT_TRUE(dowsing_machine1 != dowsing_machine7);
}

static void invalid_index_test(
    const pkmn::database::item_entry &entry
)
{
    EXPECT_EQ("Invalid (0x258)", entry.get_name());
    EXPECT_EQ("Unknown", entry.get_category());
    EXPECT_EQ("Unknown", entry.get_pocket());
    EXPECT_EQ("Invalid (0x258)", entry.get_description());
    EXPECT_EQ(-1, entry.get_cost());
    EXPECT_FALSE(entry.holdable());
    EXPECT_EQ(-1, entry.get_fling_power());
    EXPECT_EQ((entry.get_game_id() == 20 ? "None" : "Unknown"), entry.get_fling_effect());
}

/*
 * Make sure item entries created from invalid item slots don't
 * try to query the database for information that doesn't
 * exist.
 */
TEST_F(item_entry_test, invalid_index_test)
{
    pkmn::database::item_entry invalid(600, 15);
    pkmn::database::item_entry invalid_gcn(600, 20);
    invalid_index_test(invalid);
    invalid_index_test(invalid_gcn);
}

static void tmhm_entry_check_move(
    const pkmn::database::item_entry &tmhm_entry,
    const std::string& move
)
{
    static boost::format tmhm_desc("Teaches the move %s.");
    EXPECT_EQ(str(tmhm_desc % move.c_str()), tmhm_entry.get_description());
}

/*
 * Make sure TM/HM item entries place the correct corresponding
 * move in the description, even if the item name is the same.
 */
TEST_F(item_entry_test, tmhm_test)
{
    pkmn::database::item_entry tm16_gen1(
        "TM16", pkmn::e_game::RED
    );
    pkmn::database::item_entry tm16_gen2(
        "TM16", pkmn::e_game::SILVER
    );
    pkmn::database::item_entry tm16_gen3(
        "TM16", pkmn::e_game::XD
    );
    tmhm_entry_check_move(tm16_gen1, "Pay Day");
    tmhm_entry_check_move(tm16_gen2, "Icy Wind");
    tmhm_entry_check_move(tm16_gen3, "Light Screen");

    pkmn::database::item_entry tm83_gen4(
        "TM83", pkmn::e_game::PLATINUM
    );
    pkmn::database::item_entry tm83_gen5(
        "TM83", pkmn::e_game::WHITE
    );
    pkmn::database::item_entry tm83_gen6(
        "TM83", pkmn::e_game::OMEGA_RUBY
    );
    tmhm_entry_check_move(tm83_gen4, "Natural Gift");
    tmhm_entry_check_move(tm83_gen5, "Work Up");
    tmhm_entry_check_move(tm83_gen6, "Infestation");

    pkmn::database::item_entry tm83_xy(
        "TM94", pkmn::e_game::Y
    );
    pkmn::database::item_entry tm83_oras(
        "TM94", pkmn::e_game::ALPHA_SAPPHIRE
    );
    tmhm_entry_check_move(tm83_xy, "Rock Smash");
    tmhm_entry_check_move(tm83_oras, "Secret Power");

    pkmn::database::item_entry hm05_gen3(
        "HM05", pkmn::e_game::EMERALD
    );
    pkmn::database::item_entry hm05_dppt(
        "HM05", pkmn::e_game::DIAMOND
    );
    pkmn::database::item_entry hm05_hgss(
        "HM05", pkmn::e_game::SOULSILVER
    );
    pkmn::database::item_entry hm05_gen5(
        "HM05", pkmn::e_game::BLACK
    );
    tmhm_entry_check_move(hm05_gen3, "Flash");
    tmhm_entry_check_move(hm05_dppt, "Defog");
    tmhm_entry_check_move(hm05_hgss, "Whirlpool");
    tmhm_entry_check_move(hm05_gen5, "Waterfall");

    // Gamecube games have no HMs
    EXPECT_THROW(
        pkmn::database::item_entry hm01_gcn(
            "HM01", pkmn::e_game::COLOSSEUM
        )
    , std::invalid_argument);
    EXPECT_THROW(
        pkmn::database::item_entry hm01_gcn(
            "HM01", pkmn::e_game::XD
        )
    , std::invalid_argument);
}

TEST_F(item_entry_test, equality_test)
{
    EXPECT_TRUE(byindex_gen1 == byname_gen1);
    EXPECT_TRUE(byindex_gen2 == byname_gen2);
    EXPECT_TRUE(byindex_gba  == byname_gba);
    EXPECT_TRUE(byindex_gcn  == byname_gcn);
    EXPECT_TRUE(byindex_gen4 == byname_gen4);
    EXPECT_TRUE(byindex_gen5 == byname_gen5);
    EXPECT_TRUE(byindex_gen6 == byname_gen6);

    EXPECT_TRUE(byindex_gen1 != byname_gen2);
    EXPECT_TRUE(byindex_gen2 != byname_gba);
    EXPECT_TRUE(byindex_gba  != byname_gcn);
    EXPECT_TRUE(byindex_gcn  != byname_gen4);
    EXPECT_TRUE(byindex_gen4 != byname_gen5);
    EXPECT_TRUE(byindex_gen5 != byname_gen6);
}

static void _item_entry_test(
    const pkmn::database::item_entry &item_entry_gen1,
    const pkmn::database::item_entry &item_entry_gen2,
    const pkmn::database::item_entry &item_entry_gba,
    const pkmn::database::item_entry &item_entry_gcn,
    const pkmn::database::item_entry &item_entry_gen4,
    const pkmn::database::item_entry &item_entry_gen5,
    const pkmn::database::item_entry &item_entry_gen6
)
{
    /*
     * For descriptions, just make sure it doesn't fail. If
     * there is no description in the database for the corresponding
     * game, it should fallback onto the X/Y description.
     */

    // Generation I entry
    EXPECT_EQ("Potion", item_entry_gen1.get_name());
    EXPECT_EQ(pkmn::e_game::RED, item_entry_gen1.get_game());
    EXPECT_EQ("Healing", item_entry_gen1.get_category());
    EXPECT_EQ("Items", item_entry_gen1.get_pocket());
    (void)item_entry_gen1.get_description();
    EXPECT_EQ(300, item_entry_gen1.get_cost());
    EXPECT_FALSE(item_entry_gen1.holdable());
    EXPECT_EQ(-1, item_entry_gen1.get_fling_power());
    EXPECT_EQ("None", item_entry_gen1.get_fling_effect());

    // Generation II entry
    EXPECT_EQ("Pink Bow", item_entry_gen2.get_name());
    EXPECT_EQ(pkmn::e_game::SILVER, item_entry_gen2.get_game());
    EXPECT_EQ("Stat boosts", item_entry_gen2.get_category());
    EXPECT_EQ("Items", item_entry_gen2.get_pocket());
    (void)item_entry_gen2.get_description();
    EXPECT_EQ(50, item_entry_gen2.get_cost());
    EXPECT_TRUE(item_entry_gen2.holdable());
    EXPECT_EQ(-1, item_entry_gen2.get_fling_power());
    EXPECT_EQ("None", item_entry_gen2.get_fling_effect());

    // GBA entry
    EXPECT_EQ("Razz Berry", item_entry_gba.get_name());
    EXPECT_EQ(pkmn::e_game::EMERALD, item_entry_gba.get_game());
    EXPECT_EQ("Baking only", item_entry_gba.get_category());
    EXPECT_EQ("Berries", item_entry_gba.get_pocket());
    (void)item_entry_gba.get_description();
    EXPECT_EQ(20, item_entry_gba.get_cost());
    EXPECT_TRUE(item_entry_gba.holdable());
    EXPECT_EQ(-1, item_entry_gba.get_fling_power());
    EXPECT_EQ("None", item_entry_gba.get_fling_effect());

    // Gamecube entry
    EXPECT_EQ("Battle CD 03", item_entry_gcn.get_name());
    EXPECT_EQ(pkmn::e_game::XD, item_entry_gcn.get_game());
    EXPECT_EQ("Battle CDs", item_entry_gcn.get_category());
    EXPECT_EQ("Battle CDs", item_entry_gcn.get_pocket());
    (void)item_entry_gcn.get_description();
    EXPECT_EQ(0, item_entry_gcn.get_cost());
    EXPECT_FALSE(item_entry_gcn.holdable());
    EXPECT_EQ(-1, item_entry_gcn.get_fling_power());
    EXPECT_EQ("None", item_entry_gcn.get_fling_effect());

    // Generation IV entry
    EXPECT_EQ("Fast Ball", item_entry_gen4.get_name());
    EXPECT_EQ(pkmn::e_game::SOULSILVER, item_entry_gen4.get_game());
    EXPECT_EQ("Apricorn balls", item_entry_gen4.get_category());
    EXPECT_EQ("Poké Balls", item_entry_gen4.get_pocket());
    (void)item_entry_gen4.get_description();
    EXPECT_EQ(300, item_entry_gen4.get_cost());
    EXPECT_TRUE(item_entry_gen4.holdable());
    EXPECT_EQ(0, item_entry_gen4.get_fling_power());
    EXPECT_EQ("None", item_entry_gen4.get_fling_effect());

    // Generation V entry
    EXPECT_EQ("Reveal Glass", item_entry_gen5.get_name());
    EXPECT_EQ(pkmn::e_game::BLACK2, item_entry_gen5.get_game());
    EXPECT_EQ("Gameplay", item_entry_gen5.get_category());
    EXPECT_EQ("Key Items", item_entry_gen5.get_pocket());
    (void)item_entry_gen5.get_description();
    EXPECT_EQ(0, item_entry_gen5.get_cost());
    EXPECT_FALSE(item_entry_gen5.holdable());
    EXPECT_EQ(0, item_entry_gen5.get_fling_power());
    EXPECT_EQ("None", item_entry_gen5.get_fling_effect());

    // Generation VI entry
    EXPECT_EQ("Salamencite", item_entry_gen6.get_name());
    EXPECT_EQ(pkmn::e_game::OMEGA_RUBY, item_entry_gen6.get_game());
    EXPECT_EQ("Mega Stones", item_entry_gen6.get_category());
    EXPECT_EQ("Items", item_entry_gen6.get_pocket());
    (void)item_entry_gen6.get_description();
    EXPECT_EQ(0, item_entry_gen6.get_cost());
    EXPECT_TRUE(item_entry_gen6.holdable());
    EXPECT_EQ(0, item_entry_gen6.get_fling_power());
    EXPECT_EQ("None", item_entry_gen6.get_fling_effect());
}

TEST_F(item_entry_test, by_index_test)
{
    _item_entry_test(
        byindex_gen1, byindex_gen2, byindex_gba,
        byindex_gcn, byindex_gen4, byindex_gen5,
        byindex_gen6
    );
}

TEST_F(item_entry_test, by_name_test)
{
    _item_entry_test(
        byname_gen1, byname_gen2, byname_gba,
        byname_gcn, byname_gen4, byname_gen5,
        byname_gen6
    );
}

/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "item_test_common.hpp"

#include <pkmntest/util.hpp>

#include "utils/misc.hpp"

#include <pkmn/exception.hpp>
#include <pkmn/database/item_entry.hpp>
#include <pkmn/database/lists.hpp>
#include "pksav/pksav_call.hpp"

#include <pksav/common/stats.h>
#include <pksav/gba/items.h>
#include <pksav/math/endian.h>

#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>

static const std::vector<std::string> all_pocket_item_names = boost::assign::list_of
    ("Potion")("Mach Bike")("Great Ball")("TM01")
    ("Aspear Berry")("Wailmer Pail")("Master Ball")("HM04")
;
static const std::vector<std::string> wrong_game_all_pocket_items = boost::assign::list_of
    ("Pink Bow")("Black Sludge")
    ("Ein File S")("Gonzap's Key")
    ("GS Ball")("Poffin Items")
    ("TM51")
    ("Berry")("Occa Berry")
;

class gba_item_list_test: public item_list_test {};

/*
 * On the C++ level, make sure the LibPKMN abstraction matches the underlying
 * PKSav struct.
 */
static void check_pksav_struct(
    const pkmn::item_slots_t& item_slots,
    pkmn::e_game game,
    int expected_num_items,
    const struct pksav_item* p_native_items
)
{
    for(int item_index = 0; item_index < expected_num_items; ++item_index)
    {
        EXPECT_EQ(
            pkmn::database::item_entry(
                item_slots.at(item_index).item,
                game
            ).get_item_index(),
            int(pksav_littleendian16(p_native_items[item_index].index))
        );
        EXPECT_EQ(
            item_slots.at(item_index).amount,
            int(pksav_littleendian16(p_native_items[item_index].count))
        );
    }

    EXPECT_EQ(0, p_native_items[expected_num_items].index);
    EXPECT_EQ(0, p_native_items[expected_num_items].count);
}

void gba_item_pocket_test(
    const pkmn::item_list::sptr& item_pocket
)
{
    ASSERT_EQ("Items", item_pocket->get_name());

    int capacity = 0;
    pkmn::e_game game = item_pocket->get_game();

    if(game == pkmn::e_game::RUBY or game == pkmn::e_game::SAPPHIRE)
    {
        capacity = 20;
    }
    else if(game == pkmn::e_game::EMERALD)
    {
        capacity = 30;
    }
    else
    {
        capacity = 42;
    }
    ASSERT_EQ(capacity, item_pocket->get_capacity());
    ASSERT_EQ(size_t(capacity), item_pocket->as_vector().size());

    // Make sure item slots start as correctly empty
    test_item_list_empty_slots(item_pocket);

    // Confirm exceptions are thrown when expected.
    test_item_list_out_of_range_error(
        item_pocket,
        "Potion"
    );

    // Make sure we can't add items from other pockets.
    static const std::vector<std::string> wrong_pocket_items = boost::assign::list_of
        ("Bicycle")("Master Ball")("HM01")("Razz Berry")
    ;
    test_item_list_invalid_items(
        item_pocket,
        wrong_pocket_items
    );

    // Make sure we can't add items from other generations.
    static const std::vector<std::string> wrong_generation_items = boost::assign::list_of
        ("Pink Bow")("Black Sludge")("Binding Band")("Beedrillite")
    ;
    test_item_list_invalid_items(
        item_pocket,
        wrong_generation_items
    );

    // Make sure we can't add items from Gamecube games.
    static const std::vector<std::string> gcn_items = boost::assign::list_of
        ("Time Flute")
        ("Poké Snack")
    ;
    test_item_list_invalid_items(
        item_pocket,
        gcn_items
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    static const std::vector<std::string> item_names = boost::assign::list_of
        ("Potion")("Orange Mail")("Lava Cookie")("Stardust")
        ("Shadow Mail")("Pink Scarf")("Antidote")("Green Shard")
    ;
    test_item_list_add_remove(
        item_pocket,
        item_names
    );

    const std::vector<std::string>& valid_items = item_pocket->get_valid_items();
    EXPECT_GT(valid_items.size(), 0);

    check_pksav_struct(
        item_pocket->as_vector(),
        item_pocket->get_game(),
        item_pocket->get_num_items(),
        static_cast<const struct pksav_item*>(item_pocket->get_native())
    );
}

void gba_key_item_pocket_test(
    const pkmn::item_list::sptr& key_item_pocket
)
{
    ASSERT_EQ("Key Items", key_item_pocket->get_name());

    pkmn::e_game game = key_item_pocket->get_game();

    int capacity = 0;
    if(game == pkmn::e_game::RUBY or game == pkmn::e_game::SAPPHIRE)
    {
        capacity = 20;
    }
    else
    {
        capacity = 30;
    }
    ASSERT_EQ(capacity, key_item_pocket->get_capacity());
    ASSERT_EQ(size_t(capacity), key_item_pocket->as_vector().size());

    // Make sure item slots start as correctly empty
    test_item_list_empty_slots(key_item_pocket);

    // Confirm exceptions are thrown when expected.
    test_item_list_out_of_range_error(
        key_item_pocket,
        "Basement Key"
    );

    // Make sure we can't add items from other pockets.
    static const std::vector<std::string> wrong_pocket_items = boost::assign::list_of
        ("Potion")("Master Ball")("HM01")("Razz Berry")
    ;
    test_item_list_invalid_items(
        key_item_pocket,
        wrong_pocket_items
    );

    // Make sure we can't add items from other generations.
    static const std::vector<std::string> wrong_generation_items = boost::assign::list_of
        ("GS Ball")("Poffin Items")("DNA Splicers")("Aqua Suit")
    ;
    test_item_list_invalid_items(
        key_item_pocket,
        wrong_generation_items
    );

    // Make sure we can't add items from incompatible Generation III games.
    static const std::vector<std::string> gcn_items = boost::assign::list_of
        ("Ein File S")("Powerup Part")
        ("Gonzap's Key")("Krane Memo 1")
    ;
    static const std::vector<std::string> frlg_items = boost::assign::list_of
        ("Helix Fossil")("Tea")("Ruby")
    ;
    static const std::vector<std::string> emerald_items = boost::assign::list_of
        ("Magma Emblem")("Old Sea Map")
    ;
    test_item_list_invalid_items(
        key_item_pocket,
        gcn_items
    );
    if(game == pkmn::e_game::RUBY or game == pkmn::e_game::SAPPHIRE) {
        test_item_list_invalid_items(
            key_item_pocket,
            frlg_items
        );
    }
    if(game != pkmn::e_game::EMERALD) {
        test_item_list_invalid_items(
            key_item_pocket,
            emerald_items
        );
    }

    // Start adding and removing stuff, and make sure the numbers are accurate.
    static const std::vector<std::string> item_names = boost::assign::list_of
        ("Wailmer Pail")("Basement Key")("Meteorite")("Old Rod")
        ("Red Orb")("Root Fossil")("Contest Pass")("Eon Ticket")
    ;
    test_item_list_add_remove(
        key_item_pocket,
        item_names
    );

    const std::vector<std::string>& valid_items = key_item_pocket->get_valid_items();
    EXPECT_GT(valid_items.size(), 0);

    check_pksav_struct(
        key_item_pocket->as_vector(),
        key_item_pocket->get_game(),
        key_item_pocket->get_num_items(),
        static_cast<const struct pksav_item*>(key_item_pocket->get_native())
    );
}

void gba_ball_pocket_test(
    const pkmn::item_list::sptr& ball_pocket
)
{
    ASSERT_EQ("Poké Balls", ball_pocket->get_name());

    pkmn::e_game game = ball_pocket->get_game();

    int capacity = 0;
    if(game == pkmn::e_game::FIRERED or game == pkmn::e_game::LEAFGREEN)
    {
        capacity = 13;
    }
    else
    {
        capacity = 16;
    }
    ASSERT_EQ(capacity, ball_pocket->get_capacity());
    ASSERT_EQ(size_t(capacity), ball_pocket->as_vector().size());

    // Make sure item slots start as correctly empty
    test_item_list_empty_slots(ball_pocket);

    // Confirm exceptions are thrown when expected.
    test_item_list_out_of_range_error(
        ball_pocket,
        "Master Ball"
    );

    // Make sure we can't add items from other pockets.
    static const std::vector<std::string> wrong_pocket_items = boost::assign::list_of
        ("Potion")("Bicycle")("HM01")("Razz Berry")
    ;
    test_item_list_invalid_items(
        ball_pocket,
        wrong_pocket_items
    );

    // Make sure we can't add items from other generations.
    static const std::vector<std::string> wrong_generation_items = boost::assign::list_of
        ("Moon Ball")("Heal Ball")("Dream Ball")
    ;
    test_item_list_invalid_items(
        ball_pocket,
        wrong_generation_items
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    static const std::vector<std::string> item_names = boost::assign::list_of
        ("Master Ball")("Ultra Ball")("Great Ball")("Poké Ball")
        ("Safari Ball")("Net Ball")("Dive Ball")("Nest Ball")
    ;
    test_item_list_add_remove(
        ball_pocket,
        item_names
    );

    const std::vector<std::string>& valid_items = ball_pocket->get_valid_items();
    EXPECT_GT(valid_items.size(), 0);

    check_pksav_struct(
        ball_pocket->as_vector(),
        ball_pocket->get_game(),
        ball_pocket->get_num_items(),
        static_cast<const struct pksav_item*>(ball_pocket->get_native())
    );
}

void gba_tmhm_pocket_test(
    const pkmn::item_list::sptr& tmhm_pocket
)
{
    int capacity = 0;

    pkmn::e_game game = tmhm_pocket->get_game();

    if(game == pkmn::e_game::FIRERED or game == pkmn::e_game::LEAFGREEN)
    {
        ASSERT_EQ("TM Case", tmhm_pocket->get_name());
        capacity = 58;
    }
    else
    {
        ASSERT_EQ("TMs & HMs", tmhm_pocket->get_name());
        capacity = 64;
    }
    ASSERT_EQ(capacity, tmhm_pocket->get_capacity());
    ASSERT_EQ(size_t(capacity), tmhm_pocket->as_vector().size());

    // Make sure item slots start as correctly empty
    test_item_list_empty_slots(tmhm_pocket);

    // Confirm exceptions are thrown when expected.
    test_item_list_out_of_range_error(
        tmhm_pocket,
        "TM01"
    );

    // Make sure we can't add items from other pockets.
    static const std::vector<std::string> wrong_pocket_items = boost::assign::list_of
        ("Potion")("Bicycle")("Great Ball")("Razz Berry")
    ;
    test_item_list_invalid_items(
        tmhm_pocket,
        wrong_pocket_items
    );

    // Make sure we can't add items from other generations.
    static const std::vector<std::string> wrong_generation_items = boost::assign::list_of
        ("TM51")
    ;
    test_item_list_invalid_items(
        tmhm_pocket,
        wrong_generation_items
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    static const std::vector<std::string> item_names = boost::assign::list_of
        ("TM01")("HM01")("TM02")("HM02")
        ("TM03")("HM03")("TM04")("HM04")
    ;
    test_item_list_add_remove(
        tmhm_pocket,
        item_names
    );

    const std::vector<std::string>& valid_items = tmhm_pocket->get_valid_items();
    EXPECT_GT(valid_items.size(), 0);

    check_pksav_struct(
        tmhm_pocket->as_vector(),
        tmhm_pocket->get_game(),
        tmhm_pocket->get_num_items(),
        static_cast<const struct pksav_item*>(tmhm_pocket->get_native())
    );
}

void gba_berry_pocket_test(
    const pkmn::item_list::sptr& berry_pocket
)
{
    int capacity = 0;

    pkmn::e_game game = berry_pocket->get_game();

    if(game == pkmn::e_game::FIRERED or game == pkmn::e_game::LEAFGREEN)
    {
        ASSERT_EQ("Berry Pouch", berry_pocket->get_name());
        capacity = 43;
    }
    else
    {
        ASSERT_EQ("Berries", berry_pocket->get_name());
        capacity = 46;
    }
    ASSERT_EQ(capacity, berry_pocket->get_capacity());
    ASSERT_EQ(size_t(capacity), berry_pocket->as_vector().size());

    // Make sure item slots start as correctly empty
    test_item_list_empty_slots(berry_pocket);

    // Confirm exceptions are thrown when expected.
    test_item_list_out_of_range_error(
        berry_pocket,
        "Razz Berry"
    );

    // Make sure we can't add items from other pockets.
    static const std::vector<std::string> wrong_pocket_items = boost::assign::list_of
        ("Potion")("Bicycle")("Great Ball")("HM02")
    ;
    test_item_list_invalid_items(
        berry_pocket,
        wrong_pocket_items
    );

    // Make sure we can't add items from other generations.
    static const std::vector<std::string> wrong_generation_items = boost::assign::list_of
        ("Berry")("Occa Berry")("Roseli Berry")
    ;
    test_item_list_invalid_items(
        berry_pocket,
        wrong_generation_items
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    static const std::vector<std::string> item_names = boost::assign::list_of
        ("Cheri Berry")("Razz Berry")("Lum Berry")("Pinap Berry")
        ("Aspear Berry")("Iapapa Berry")("Wiki Berry")("Apicot Berry")
    ;
    test_item_list_add_remove(
        berry_pocket,
        item_names
    );

    const std::vector<std::string>& valid_items = berry_pocket->get_valid_items();
    EXPECT_GT(valid_items.size(), 0);

    check_pksav_struct(
        berry_pocket->as_vector(),
        berry_pocket->get_game(),
        berry_pocket->get_num_items(),
        static_cast<const struct pksav_item*>(berry_pocket->get_native())
    );
}

void gba_item_pc_test(
    const pkmn::item_list::sptr& item_pc
)
{
    ASSERT_EQ("PC", item_pc->get_name());
    ASSERT_EQ(50, item_pc->get_capacity());
    ASSERT_EQ(50, item_pc->as_vector().size());

    // Make sure item slots start as correctly empty
    test_item_list_empty_slots(item_pc);

    // Confirm exceptions are thrown when expected.
    test_item_list_out_of_range_error(
        item_pc,
        "Potion"
    );

    // Make sure we can't add items from other generations or incompatible Generation III games.
    test_item_list_invalid_items(
        item_pc,
        wrong_game_all_pocket_items
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    test_item_list_add_remove(
        item_pc,
        all_pocket_item_names
    );

    check_pksav_struct(
        item_pc->as_vector(),
        item_pc->get_game(),
        item_pc->get_num_items(),
        static_cast<const struct pksav_item*>(item_pc->get_native())
    );

    // For FR/LG, the Berry Pouch and TM Case cannot be added to the PC, as
    // they are also bag pockets, so make sure this is reflects in the
    // get_valid_items() call.
    pkmn::e_game game = item_pc->get_game();
    const std::vector<std::string>& valid_items = item_pc->get_valid_items();
    std::vector<std::string> all_items = pkmn::database::get_item_name_list(game);

    if((game == pkmn::e_game::FIRERED) || (game == pkmn::e_game::LEAFGREEN))
    {
        EXPECT_EQ(all_items.size()-2, valid_items.size());

        EXPECT_FALSE(pkmn::does_vector_contain_value<std::string>(valid_items, "Berry Pouch"));
        EXPECT_FALSE(pkmn::does_vector_contain_value<std::string>(valid_items, "TM Case"));
    }
    else
    {
        EXPECT_EQ(all_items, valid_items);
    }
}

static const item_list_test_fcns_t gba_test_fcns = boost::assign::map_list_of
    ("Items", &gba_item_pocket_test)
    ("Key Items", &gba_key_item_pocket_test)
    ("Poké Balls", &gba_ball_pocket_test)
    ("TMs & HMs", &gba_tmhm_pocket_test)
    ("TM Case", &gba_tmhm_pocket_test)
    ("Berries", &gba_berry_pocket_test)
    ("Berry Pouch", &gba_berry_pocket_test)
    ("PC", &gba_item_pc_test)
;

TEST_P(gba_item_list_test, item_list_test)
{
    gba_test_fcns.at(get_name())(get_item_list());
}

static const std::vector<std::pair<pkmn::e_game, std::string>> item_list_params =
{
    {pkmn::e_game::RUBY, "Items"},
    {pkmn::e_game::RUBY, "Key Items"},
    {pkmn::e_game::RUBY, "Poké Balls"},
    {pkmn::e_game::RUBY, "TMs & HMs"},
    {pkmn::e_game::RUBY, "Berries"},
    {pkmn::e_game::RUBY, "PC"},
    {pkmn::e_game::SAPPHIRE, "Items"},
    {pkmn::e_game::SAPPHIRE, "Key Items"},
    {pkmn::e_game::SAPPHIRE, "Poké Balls"},
    {pkmn::e_game::SAPPHIRE, "TMs & HMs"},
    {pkmn::e_game::SAPPHIRE, "Berries"},
    {pkmn::e_game::SAPPHIRE, "PC"},
    {pkmn::e_game::EMERALD, "Items"},
    {pkmn::e_game::EMERALD, "Key Items"},
    {pkmn::e_game::EMERALD, "Poké Balls"},
    {pkmn::e_game::EMERALD, "TMs & HMs"},
    {pkmn::e_game::EMERALD, "Berries"},
    {pkmn::e_game::EMERALD, "PC"},
    {pkmn::e_game::FIRERED, "Items"},
    {pkmn::e_game::FIRERED, "Key Items"},
    {pkmn::e_game::FIRERED, "Poké Balls"},
    {pkmn::e_game::FIRERED, "TM Case"},
    {pkmn::e_game::FIRERED, "Berry Pouch"},
    {pkmn::e_game::FIRERED, "PC"},
    {pkmn::e_game::LEAFGREEN, "Items"},
    {pkmn::e_game::LEAFGREEN, "Key Items"},
    {pkmn::e_game::LEAFGREEN, "Poké Balls"},
    {pkmn::e_game::LEAFGREEN, "TM Case"},
    {pkmn::e_game::LEAFGREEN, "Berry Pouch"},
    {pkmn::e_game::LEAFGREEN, "PC"},
};

INSTANTIATE_TEST_CASE_P(
    cpp_gba_item_list_test,
    gba_item_list_test,
    ::testing::ValuesIn(item_list_params)
);

class gba_item_bag_test: public item_bag_test {};

TEST_P(gba_item_bag_test, item_bag_test)
{
    const pkmn::item_bag::sptr& bag = get_item_bag();

    pkmn::e_game game = get_game();
    bool is_frlg = (game == pkmn::e_game::FIRERED or game == pkmn::e_game::LEAFGREEN);
    std::string berry_pocket_name = is_frlg ? "Berry Pouch" : "Berries";
    std::string tmhm_pocket_name = is_frlg ? "TM Case" : "TMs & HMs";

    const pkmn::item_pockets_t& pockets = bag->get_pockets();
    ASSERT_EQ(5, pockets.size());
    ASSERT_EQ("Items", pockets.at("Items")->get_name());
    ASSERT_EQ(get_game(), pockets.at("Items")->get_game());
    ASSERT_EQ("Key Items", pockets.at("Key Items")->get_name());
    ASSERT_EQ(get_game(), pockets.at("Key Items")->get_game());
    ASSERT_EQ("Poké Balls", pockets.at("Poké Balls")->get_name());
    ASSERT_EQ(get_game(), pockets.at("Poké Balls")->get_game());
    ASSERT_EQ(berry_pocket_name, pockets.at(berry_pocket_name)->get_name());
    ASSERT_EQ(get_game(), pockets.at(berry_pocket_name)->get_game());
    ASSERT_EQ(tmhm_pocket_name, pockets.at(tmhm_pocket_name)->get_name());
    ASSERT_EQ(get_game(), pockets.at(tmhm_pocket_name)->get_game());

    const std::vector<std::string>& pocket_names = bag->get_pocket_names();
    ASSERT_EQ(5, pockets.size());
    ASSERT_NE(pocket_names.end(), std::find(pocket_names.begin(), pocket_names.end(), "Items"));
    ASSERT_NE(pocket_names.end(), std::find(pocket_names.begin(), pocket_names.end(), "Key Items"));
    ASSERT_NE(pocket_names.end(), std::find(pocket_names.begin(), pocket_names.end(), "Poké Balls"));
    ASSERT_NE(pocket_names.end(), std::find(pocket_names.begin(), pocket_names.end(), berry_pocket_name));
    ASSERT_NE(pocket_names.end(), std::find(pocket_names.begin(), pocket_names.end(), tmhm_pocket_name));

    gba_item_pocket_test(pockets.at("Items"));
    gba_key_item_pocket_test(pockets.at("Key Items"));
    gba_ball_pocket_test(pockets.at("Poké Balls"));
    gba_berry_pocket_test(pockets.at(berry_pocket_name));
    gba_tmhm_pocket_test(pockets.at(tmhm_pocket_name));
    reset();

    // Make sure adding items through the bag adds to the proper pockets.
    ASSERT_EQ(0, bag->get_pocket("Items")->get_num_items());
    ASSERT_EQ(0, bag->get_pocket("Key Items")->get_num_items());
    ASSERT_EQ(0, bag->get_pocket("Poké Balls")->get_num_items());
    if(is_frlg)
    {
        ASSERT_EQ(0, bag->get_pocket("TM Case")->get_num_items());
        ASSERT_EQ(0, bag->get_pocket("Berry Pouch")->get_num_items());
    }
    else
    {
        ASSERT_EQ(0, bag->get_pocket("TMs & HMs")->get_num_items());
        ASSERT_EQ(0, bag->get_pocket("Berries")->get_num_items());
    }
    for(int item_index = 0; item_index < 8; ++item_index)
    {
        bag->add(
            all_pocket_item_names[item_index],
            5
        );
    }

    const pkmn::item_slots_t& item_slots = bag->get_pocket("Items")->as_vector();
    const pkmn::item_slots_t& key_item_slots = bag->get_pocket("Key Items")->as_vector();
    const pkmn::item_slots_t& ball_slots = bag->get_pocket("Poké Balls")->as_vector();
    const pkmn::item_slots_t& tm_hm_slots = bag->get_pocket(tmhm_pocket_name)->as_vector();
    const pkmn::item_slots_t& berry_slots = bag->get_pocket(berry_pocket_name)->as_vector();

    EXPECT_EQ("Potion", item_slots.at(0).item);
    EXPECT_EQ(5, item_slots.at(0).amount);
    EXPECT_EQ("None", item_slots.at(1).item);
    EXPECT_EQ(0, item_slots.at(1).amount);

    EXPECT_EQ("Mach Bike", key_item_slots.at(0).item);
    EXPECT_EQ(5, key_item_slots.at(0).amount);
    EXPECT_EQ("Wailmer Pail", key_item_slots.at(1).item);
    EXPECT_EQ(5, key_item_slots.at(1).amount);
    EXPECT_EQ("None", key_item_slots.at(2).item);
    EXPECT_EQ(0, key_item_slots.at(2).amount);

    EXPECT_EQ("Great Ball", ball_slots.at(0).item);
    EXPECT_EQ(5, ball_slots.at(0).amount);
    EXPECT_EQ("Master Ball", ball_slots.at(1).item);
    EXPECT_EQ(5, ball_slots.at(1).amount);
    EXPECT_EQ("None", ball_slots.at(2).item);
    EXPECT_EQ(0, ball_slots.at(2).amount);

    EXPECT_EQ("TM01", tm_hm_slots.at(0).item);
    EXPECT_EQ(5, tm_hm_slots.at(0).amount);
    EXPECT_EQ("HM04", tm_hm_slots.at(1).item);
    EXPECT_EQ(5, tm_hm_slots.at(1).amount);
    EXPECT_EQ("None", tm_hm_slots.at(2).item);
    EXPECT_EQ(0, tm_hm_slots.at(2).amount);

    EXPECT_EQ("Aspear Berry", berry_slots.at(0).item);
    EXPECT_EQ(5, berry_slots.at(0).amount);
    EXPECT_EQ("None", berry_slots.at(1).item);
    EXPECT_EQ(0, berry_slots.at(1).amount);

    /*
     * On the C++ level, make sure the LibPKMN abstraction matches the underlying
     * PKSav struct.
     */
    const union pksav_gba_item_bag* native = static_cast<const union pksav_gba_item_bag*>(bag->get_native());
    if(game == pkmn::e_game::RUBY or game == pkmn::e_game::SAPPHIRE)
    {
        check_pksav_struct(
            item_slots,
            game,
            1,
            native->rs.items
        );
        check_pksav_struct(
            key_item_slots,
            game,
            2,
            native->rs.key_items
        );
        check_pksav_struct(
            ball_slots,
            game,
            2,
            native->rs.balls
        );
        check_pksav_struct(
            tm_hm_slots,
            game,
            2,
            native->rs.tms_hms
        );
        check_pksav_struct(
            berry_slots,
            game,
            1,
            native->rs.berries
        );
    }
    else if(game == pkmn::e_game::EMERALD)
    {
        check_pksav_struct(
            item_slots,
            game,
            1,
            native->emerald.items
        );
        check_pksav_struct(
            key_item_slots,
            game,
            2,
            native->emerald.key_items
        );
        check_pksav_struct(
            ball_slots,
            game,
            2,
            native->emerald.balls
        );
        check_pksav_struct(
            tm_hm_slots,
            game,
            2,
            native->emerald.tms_hms
        );
        check_pksav_struct(
            berry_slots,
            game,
            1,
            native->emerald.berries
        );
    }
    else
    {
        check_pksav_struct(
            item_slots,
            game,
            1,
            native->frlg.items
        );
        check_pksav_struct(
            key_item_slots,
            game,
            2,
            native->frlg.key_items
        );
        check_pksav_struct(
            ball_slots,
            game,
            2,
            native->frlg.balls
        );
        check_pksav_struct(
            tm_hm_slots,
            game,
            2,
            native->frlg.tms_hms
        );
        check_pksav_struct(
            berry_slots,
            game,
            1,
            native->frlg.berries
        );
    }

    // Make sure removing items through the bag removes from the proper pockets.
    for(int item_index = 0; item_index < 8; ++item_index)
    {
        bag->remove(
            all_pocket_item_names[item_index],
            5
        );
    }

    EXPECT_EQ("None", item_slots.at(0).item);
    EXPECT_EQ(0, item_slots.at(0).amount);
    EXPECT_EQ("None", item_slots.at(1).item);
    EXPECT_EQ(0, item_slots.at(1).amount);

    EXPECT_EQ("None", key_item_slots.at(0).item);
    EXPECT_EQ(0, key_item_slots.at(0).amount);
    EXPECT_EQ("None", key_item_slots.at(1).item);
    EXPECT_EQ(0, key_item_slots.at(1).amount);
    EXPECT_EQ("None", key_item_slots.at(2).item);
    EXPECT_EQ(0, key_item_slots.at(2).amount);

    EXPECT_EQ("None", ball_slots.at(0).item);
    EXPECT_EQ(0, ball_slots.at(0).amount);
    EXPECT_EQ("None", ball_slots.at(1).item);
    EXPECT_EQ(0, ball_slots.at(1).amount);
    EXPECT_EQ("None", ball_slots.at(2).item);
    EXPECT_EQ(0, ball_slots.at(2).amount);

    EXPECT_EQ("None", tm_hm_slots.at(0).item);
    EXPECT_EQ(0, tm_hm_slots.at(0).amount);
    EXPECT_EQ("None", tm_hm_slots.at(1).item);
    EXPECT_EQ(0, tm_hm_slots.at(1).amount);
    EXPECT_EQ("None", tm_hm_slots.at(2).item);
    EXPECT_EQ(0, tm_hm_slots.at(2).amount);

    EXPECT_EQ("None", berry_slots.at(0).item);
    EXPECT_EQ(0, berry_slots.at(0).amount);
    EXPECT_EQ("None", berry_slots.at(1).item);
    EXPECT_EQ(0, berry_slots.at(1).amount);

    // Make sure we can't add items from other generations or invalid Generation III games.
    test_item_bag_invalid_items(
        bag,
        wrong_game_all_pocket_items
    );
}

INSTANTIATE_TEST_CASE_P(
    cpp_gba_item_bag_test,
    gba_item_bag_test,
    ::testing::ValuesIn(pkmntest::GBA_GAMES)
);

/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "item_test_common.hpp"

#include <pkmntest/util.hpp>

#include <pkmn/exception.hpp>
#include <pkmn/database/item_entry.hpp>
#include <pkmn/database/lists.hpp>
#include <pkmn/enums/enum_to_string.hpp>
#include "pksav/pksav_call.hpp"

#include <pksav/common/stats.h>
#include <pksav/gen2/items.h>
#include <pksav/math/endian.h>

#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>

#include <algorithm>

// TODO: check vector sizes against PKSav #defines

static const std::vector<pkmn::e_item> ALL_POCKET_ITEMS =
{
    pkmn::e_item::POTION,
    pkmn::e_item::BICYCLE,
    pkmn::e_item::GREAT_BALL,
    pkmn::e_item::TM28,
    pkmn::e_item::BERRY,
    pkmn::e_item::SQUIRT_BOTTLE,
    pkmn::e_item::FRIEND_BALL,
    pkmn::e_item::HM01
};
static const std::vector<pkmn::e_item> CRYSTAL_ITEMS =
{
    pkmn::e_item::CLEAR_BELL,
    pkmn::e_item::GS_BALL,
    pkmn::e_item::BLUE_CARD,
    pkmn::e_item::EGG_TICKET
};
static const std::vector<pkmn::e_item> WRONG_GENERATION_ALL_POCKET_ITEMS =
{
    pkmn::e_item::BLACK_SLUDGE,
    pkmn::e_item::POFFIN_CASE,
    pkmn::e_item::TM51,
    pkmn::e_item::OCCA_BERRY
};

class gen2_item_list_test: public item_list_test {};

void gen2_item_pocket_test(
    const pkmn::item_list::sptr& item_pocket
)
{
    ASSERT_EQ("Items", item_pocket->get_name());
    ASSERT_EQ(PKSAV_GEN2_ITEM_POCKET_SIZE, item_pocket->get_capacity());
    ASSERT_EQ(PKSAV_GEN2_ITEM_POCKET_SIZE, item_pocket->as_vector().size());

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(item_pocket);

    // Confirm exceptions are thrown when expected.
    test_item_list_out_of_range_error(
        item_pocket,
        pkmn::e_item::POTION
    );

    // Make sure we can't add items from other pockets.
    static const std::vector<pkmn::e_item> wrong_pocket_items =
    {
        pkmn::e_item::BICYCLE,
        pkmn::e_item::MASTER_BALL,
        pkmn::e_item::HM01
    };
    test_item_list_invalid_items(
        item_pocket,
        wrong_pocket_items
    );

    // Make sure we can't add items from later generations.
    static const std::vector<pkmn::e_item> wrong_generation_items =
    {
        pkmn::e_item::BLACK_FLUTE,
        pkmn::e_item::BLACK_SLUDGE,
        pkmn::e_item::BINDING_BAND,
        pkmn::e_item::BEEDRILLITE
    };
    test_item_list_invalid_items(
        item_pocket,
        wrong_generation_items
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    static const std::vector<pkmn::e_item> items =
    {
        pkmn::e_item::POTION,
        pkmn::e_item::HP_UP,
        pkmn::e_item::WHITE_APRICORN,
        pkmn::e_item::LUCKY_EGG,
        pkmn::e_item::FLOWER_MAIL,
        pkmn::e_item::BURN_HEAL,
        pkmn::e_item::PSNCUREBERRY,
        pkmn::e_item::STICK
    };
    test_item_list_add_remove(
        item_pocket,
        items
    );

    const std::vector<pkmn::e_item>& valid_items = item_pocket->get_valid_items();
    const std::vector<std::string>& valid_item_names = item_pocket->get_valid_item_names();

    ASSERT_EQ(valid_items.size(), valid_item_names.size());
    EXPECT_GT(valid_item_names.size(), 0);

    /*
     * On the C++ level, make sure the LibPKMN abstraction matches the underlying
     * PKSav struct.
     */
    const pkmn::item_slots_t& item_slots = item_pocket->as_vector();
    int num_items = item_pocket->get_num_items();

    const struct pksav_gen2_item_pocket* p_native_pocket =
        static_cast<const pksav_gen2_item_pocket*>(item_pocket->get_native());
    EXPECT_EQ(num_items, int(p_native_pocket->count));
    for(int item_index = 0; item_index < num_items; ++item_index)
    {
        EXPECT_EQ(
            pkmn::database::item_entry(
                item_slots.at(item_index).item,
                item_pocket->get_game()
            ).get_item_index(),
            int(p_native_pocket->items[item_index].index)
        );
        EXPECT_EQ(
            item_slots.at(item_index).amount,
            int(p_native_pocket->items[item_index].count)
        );
    }
    EXPECT_EQ(0, p_native_pocket->items[num_items].index);
    EXPECT_EQ(0, p_native_pocket->items[num_items].count);
    EXPECT_EQ(0xFF, p_native_pocket->terminator);
}

void gen2_key_item_pocket_test(
    const pkmn::item_list::sptr& key_item_pocket
)
{
    ASSERT_EQ("KeyItems", key_item_pocket->get_name());
    ASSERT_EQ(PKSAV_GEN2_KEY_ITEM_POCKET_SIZE, key_item_pocket->get_capacity());
    ASSERT_EQ(PKSAV_GEN2_KEY_ITEM_POCKET_SIZE, key_item_pocket->as_vector().size());

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(key_item_pocket);

    // Confirm exceptions are thrown when expected.
    test_item_list_out_of_range_error(
        key_item_pocket,
        pkmn::e_item::BICYCLE
    );

    // Make sure we can't add items from other pockets.
    static const std::vector<pkmn::e_item> wrong_pocket_items =
    {
        pkmn::e_item::POTION,
        pkmn::e_item::MASTER_BALL,
        pkmn::e_item::HM01
    };
    test_item_list_invalid_items(
        key_item_pocket,
        wrong_pocket_items
    );

    // Make sure we can't add items from later generations.
    static const std::vector<pkmn::e_item> wrong_generation_items =
    {
        pkmn::e_item::MACH_BIKE,
        pkmn::e_item::JADE_ORB,
        pkmn::e_item::LIGHT_STONE,
        pkmn::e_item::AQUA_SUIT
    };
    test_item_list_invalid_items(
        key_item_pocket,
        wrong_generation_items
    );

    // Crystal-specific items.
    if(key_item_pocket->get_game() == pkmn::e_game::CRYSTAL)
    {
        for(pkmn::e_item item: CRYSTAL_ITEMS)
        {
            key_item_pocket->add(item, 1);
            key_item_pocket->remove(item, 1);
        }

        EXPECT_EQ(0, key_item_pocket->get_num_items());
    }
    else
    {
        test_item_list_invalid_items(
            key_item_pocket,
            CRYSTAL_ITEMS
        );
    }

    // Make sure we can't add or remove more than a single item.
    EXPECT_THROW(
        key_item_pocket->add(pkmn::e_item::BICYCLE, 5);
    , std::out_of_range);
    key_item_pocket->add(pkmn::e_item::BICYCLE, 1);
    EXPECT_EQ(1, key_item_pocket->get_num_items());

    EXPECT_THROW(
        key_item_pocket->remove(pkmn::e_item::BICYCLE, 5);
    , std::out_of_range);
    key_item_pocket->remove(pkmn::e_item::BICYCLE, 1);
    EXPECT_EQ(0, key_item_pocket->get_num_items());

    // Start adding and removing stuff, and make sure the numbers are accurate.
    static const std::vector<pkmn::e_item> items =
    {
        pkmn::e_item::BICYCLE,
        pkmn::e_item::BASEMENT_KEY,
        pkmn::e_item::SECRETPOTION,
        pkmn::e_item::MYSTERY_EGG,
        pkmn::e_item::SILVER_WING,
        pkmn::e_item::LOST_ITEM,
        pkmn::e_item::SQUIRT_BOTTLE,
        pkmn::e_item::RAINBOW_WING
    };
    for(int item_index = 0; item_index < 8; ++item_index)
    {
        if(item_index < 4)
        {
            key_item_pocket->add(items[item_index], 1);
        }
        else
        {
            key_item_pocket->set_item(
                item_index,
                items[item_index],
                1
            );
        }
    }

    key_item_pocket->remove(items[2], 1);
    key_item_pocket->set_item(2, pkmn::e_item::NONE, 0);
    EXPECT_EQ(6, key_item_pocket->get_num_items());

    const std::vector<pkmn::e_item>& valid_items = key_item_pocket->get_valid_items();
    const std::vector<std::string>& valid_item_names = key_item_pocket->get_valid_item_names();

    ASSERT_EQ(valid_items.size(), valid_item_names.size());
    EXPECT_LE(valid_item_names.size(), PKSAV_GEN2_KEY_ITEM_POCKET_SIZE);

    /*
     * On the C++ level, make sure the LibPKMN abstraction matches the underlying
     * PKSav struct.
     */
    const pkmn::item_slots_t& item_slots = key_item_pocket->as_vector();
    int num_items = key_item_pocket->get_num_items();

    const struct pksav_gen2_key_item_pocket* p_native_pocket =
        static_cast<const pksav_gen2_key_item_pocket*>(key_item_pocket->get_native());
    EXPECT_EQ(num_items, int(p_native_pocket->count));
    for(int item_index = 0; item_index < num_items; ++item_index)
    {
        EXPECT_EQ(
            pkmn::database::item_entry(
                item_slots.at(item_index).item,
                key_item_pocket->get_game()
            ).get_item_index(),
            int(p_native_pocket->item_indices[item_index])
        );
    }
    EXPECT_EQ(0, p_native_pocket->item_indices[num_items]);
    EXPECT_EQ(0xFF, p_native_pocket->terminator);
}

void gen2_ball_pocket_test(
    const pkmn::item_list::sptr& ball_pocket
)
{
    ASSERT_EQ("Balls", ball_pocket->get_name());
    ASSERT_EQ(PKSAV_GEN2_BALL_POCKET_SIZE, ball_pocket->get_capacity());
    ASSERT_EQ(PKSAV_GEN2_BALL_POCKET_SIZE, ball_pocket->as_vector().size());

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(ball_pocket);

    // Confirm exceptions are thrown when expected.
    test_item_list_out_of_range_error(
        ball_pocket,
        pkmn::e_item::MASTER_BALL
    );

    // Make sure we can't add items from other pockets.
    static const std::vector<pkmn::e_item> wrong_pocket_items =
    {
        pkmn::e_item::POTION,
        pkmn::e_item::BICYCLE,
        pkmn::e_item::HM01
    };
    test_item_list_invalid_items(
        ball_pocket,
        wrong_pocket_items
    );

    // Make sure we can't add items from later generations.
    static const std::vector<pkmn::e_item> wrong_generation_items =
    {
        pkmn::e_item::PREMIER_BALL,
        pkmn::e_item::HEAL_BALL,
        pkmn::e_item::DREAM_BALL
    };
    test_item_list_invalid_items(
        ball_pocket,
        wrong_generation_items
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    static const std::vector<pkmn::e_item> items =
    {
        pkmn::e_item::GREAT_BALL,
        pkmn::e_item::POKE_BALL,
        pkmn::e_item::PARK_BALL,
        pkmn::e_item::FAST_BALL,
        pkmn::e_item::MASTER_BALL,
        pkmn::e_item::FRIEND_BALL,
        pkmn::e_item::LOVE_BALL,
        pkmn::e_item::LEVEL_BALL
    };
    test_item_list_add_remove(
        ball_pocket,
        items
    );

    const std::vector<pkmn::e_item>& valid_items = ball_pocket->get_valid_items();
    const std::vector<std::string>& valid_item_names = ball_pocket->get_valid_item_names();

    ASSERT_EQ(valid_items.size(), valid_item_names.size());
    EXPECT_GT(valid_item_names.size(), 0);

    /*
     * On the C++ level, make sure the LibPKMN abstraction matches the underlying
     * PKSav struct.
     */
    const pkmn::item_slots_t& item_slots = ball_pocket->as_vector();
    int num_items = ball_pocket->get_num_items();

    const struct pksav_gen2_ball_pocket* p_native_pocket =
        static_cast<const pksav_gen2_ball_pocket*>(ball_pocket->get_native());
    EXPECT_EQ(num_items, int(p_native_pocket->count));
    for(int item_index = 0; item_index < num_items; ++item_index)
    {
        EXPECT_EQ(
            pkmn::database::item_entry(
                item_slots.at(item_index).item,
                ball_pocket->get_game()
            ).get_item_index(),
            int(p_native_pocket->items[item_index].index)
        );
        EXPECT_EQ(
            item_slots.at(item_index).amount,
            int(p_native_pocket->items[item_index].count)
        );
    }
    EXPECT_EQ(0, p_native_pocket->items[num_items].index);
    EXPECT_EQ(0, p_native_pocket->items[num_items].count);
    EXPECT_EQ(0xFF, p_native_pocket->terminator);
}

void gen2_tmhm_pocket_test(
    const pkmn::item_list::sptr& tmhm_pocket
)
{
    BOOST_STATIC_CONSTEXPR int TMHM_POCKET_SIZE = PKSAV_GEN2_TM_COUNT + PKSAV_GEN2_HM_COUNT;

    ASSERT_EQ("TM/HM", tmhm_pocket->get_name());
    ASSERT_EQ(TMHM_POCKET_SIZE, tmhm_pocket->get_capacity());
    ASSERT_EQ(TMHM_POCKET_SIZE, tmhm_pocket->as_vector().size());

    // Make sure item slots start as correctly empty.
    const pkmn::item_slots_t& item_slots = tmhm_pocket->as_vector();
    for(int tm_index = 0; tm_index < PKSAV_GEN2_TM_COUNT; ++tm_index)
    {
        pkmn::e_item expected_item = pkmn::e_item(int(pkmn::e_item::TM01) + tm_index);
        EXPECT_EQ(expected_item, item_slots[tm_index].item)
            << pkmn::item_to_string(expected_item) << " vs "
            << pkmn::item_to_string(item_slots[tm_index].item);

        EXPECT_EQ(0, item_slots[tm_index].amount)
            << pkmn::item_to_string(expected_item);
    }
    for(int hm_index = 0; hm_index < PKSAV_GEN2_HM_COUNT; ++hm_index)
    {
        pkmn::e_item expected_item = pkmn::e_item(int(pkmn::e_item::HM01) + hm_index);
        EXPECT_EQ(expected_item, item_slots[50+hm_index].item)
            << pkmn::item_to_string(expected_item) << " vs "
            << pkmn::item_to_string(item_slots[50+hm_index].item);

        EXPECT_EQ(0, item_slots[50+hm_index].amount)
            << pkmn::item_to_string(expected_item);
    }

    // Confirm exceptions are thrown when expected.
    test_item_list_out_of_range_error(
        tmhm_pocket,
        pkmn::e_item::TM10
    );

    // Make sure we can't add items from other pockets.
    static const std::vector<pkmn::e_item> wrong_pocket_items =
    {
        pkmn::e_item::POTION,
        pkmn::e_item::MASTER_BALL,
        pkmn::e_item::BICYCLE
    };
    test_item_list_invalid_items(
        tmhm_pocket,
        wrong_pocket_items
    );

    // Make sure we can't add items from later generations.
    static const std::vector<pkmn::e_item> wrong_generation_items =
    {
        pkmn::e_item::TM51
    };
    test_item_list_invalid_items(
        tmhm_pocket,
        wrong_generation_items
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    for(int tm_index = 0; tm_index < PKSAV_GEN2_TM_COUNT; ++tm_index)
    {
        pkmn::e_item item = pkmn::e_item(int(pkmn::e_item::TM01) + tm_index);

        tmhm_pocket->add(item, 50);
        EXPECT_EQ(tm_index+1, tmhm_pocket->get_num_items());
        EXPECT_EQ(item, item_slots[tm_index].item) << pkmn::item_to_string(item);
        EXPECT_EQ(50, item_slots[tm_index].amount) << pkmn::item_to_string(item);
    }
    for(int tm_index = PKSAV_GEN2_TM_COUNT-1; tm_index >= 0; --tm_index)
    {
        pkmn::e_item item = pkmn::e_item(int(pkmn::e_item::TM01) + tm_index);

        tmhm_pocket->remove(item, 50);
        EXPECT_EQ(tm_index, tmhm_pocket->get_num_items());
        EXPECT_EQ(item, item_slots[tm_index].item) << pkmn::item_to_string(item);
        EXPECT_EQ(0, item_slots[tm_index].amount) << pkmn::item_to_string(item);
    }

    /*
     * Before removing items, on the C++ level, make sure the LibPKMN abstraction
     * matches the underlying PKSav struct.
     */
    const struct pksav_gen2_tmhm_pocket* p_native_pocket =
        static_cast<const pksav_gen2_tmhm_pocket*>(tmhm_pocket->get_native());
    for(int tm_index = 0; tm_index < PKSAV_GEN2_TM_COUNT; ++tm_index)
    {
        EXPECT_EQ(
            item_slots[tm_index].amount,
            int(p_native_pocket->tm_count[tm_index])
        );
    }
    for(int hm_index = 0; hm_index < PKSAV_GEN2_HM_COUNT; ++hm_index)
    {
        EXPECT_EQ(
            item_slots[50+hm_index].amount,
            int(p_native_pocket->hm_count[hm_index])
        );
    }

    for(int hm_index = 0; hm_index < PKSAV_GEN2_HM_COUNT; ++hm_index)
    {
        pkmn::e_item item = pkmn::e_item(int(pkmn::e_item::HM01) + hm_index);

        tmhm_pocket->add(item, 1);
        EXPECT_EQ(hm_index+1, tmhm_pocket->get_num_items());
        EXPECT_EQ(item, item_slots[50+hm_index].item) << pkmn::item_to_string(item);
        EXPECT_EQ(1, item_slots[50+hm_index].amount) << pkmn::item_to_string(item);
    }
    for(int hm_index = PKSAV_GEN2_HM_COUNT-1; hm_index >= 0; --hm_index)
    {
        pkmn::e_item item = pkmn::e_item(int(pkmn::e_item::HM01) + hm_index);

        tmhm_pocket->remove(item, 1);
        EXPECT_EQ(hm_index, tmhm_pocket->get_num_items());
        EXPECT_EQ(item, item_slots[50+hm_index].item) << pkmn::item_to_string(item);
        EXPECT_EQ(0, item_slots[50+hm_index].amount) << pkmn::item_to_string(item);
    }

    const std::vector<pkmn::e_item>& valid_items = tmhm_pocket->get_valid_items();
    const std::vector<std::string>& valid_item_names = tmhm_pocket->get_valid_item_names();

    ASSERT_EQ(valid_items.size(), valid_item_names.size());
    EXPECT_EQ(TMHM_POCKET_SIZE, valid_item_names.size());
}

void gen2_item_pc_test(
    const pkmn::item_list::sptr& item_pc
)
{
    ASSERT_EQ("PC", item_pc->get_name());
    ASSERT_EQ(PKSAV_GEN2_ITEM_PC_SIZE, item_pc->get_capacity());
    ASSERT_EQ(PKSAV_GEN2_ITEM_PC_SIZE, item_pc->as_vector().size());

    // Confirm exceptions are thrown when expected.
    test_item_list_out_of_range_error(
        item_pc,
        pkmn::e_item::POTION
    );

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(item_pc);

    // Crystal-specific items.
    if(item_pc->get_game() == pkmn::e_game::CRYSTAL)
    {
        for(pkmn::e_item crystal_item: CRYSTAL_ITEMS)
        {
            item_pc->add(crystal_item, 1);
            item_pc->remove(crystal_item, 1);
        }

        EXPECT_EQ(0, item_pc->get_num_items());
    }
    else
    {
        test_item_list_invalid_items(
            item_pc,
            CRYSTAL_ITEMS
        );
    }

    // Make sure we can't add items from later generations.
    test_item_list_invalid_items(
        item_pc,
        WRONG_GENERATION_ALL_POCKET_ITEMS
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    test_item_list_add_remove(
        item_pc,
        ALL_POCKET_ITEMS
    );

    const std::vector<pkmn::e_item>& valid_items = item_pc->get_valid_items();
    const std::vector<std::string>& valid_item_names = item_pc->get_valid_item_names();

    std::vector<pkmn::e_item> full_item_list = pkmn::database::get_item_list(item_pc->get_game());
    std::vector<std::string> full_item_name_list = pkmn::database::get_item_name_list(item_pc->get_game());

    ASSERT_EQ(valid_items.size(), valid_item_names.size());
    ASSERT_EQ(full_item_list.size(), full_item_name_list.size());
    ASSERT_EQ(valid_items.size(), full_item_list.size());

    /*
     * On the C++ level, make sure the LibPKMN abstraction matches the underlying
     * PKSav struct.
     */
    const pkmn::item_slots_t& item_slots = item_pc->as_vector();
    int num_items = item_pc->get_num_items();

    const struct pksav_gen2_item_pc* p_native_pocket = static_cast<const struct pksav_gen2_item_pc*>(item_pc->get_native());
    EXPECT_EQ(num_items, int(p_native_pocket->count));
    for(int item_index = 0; item_index < num_items; ++item_index)
    {
        EXPECT_EQ(
            pkmn::database::item_entry(
                item_slots.at(item_index).item,
                item_pc->get_game()
            ).get_item_index(),
            int(p_native_pocket->items[item_index].index)
        );
        EXPECT_EQ(
            item_slots.at(item_index).amount,
            int(p_native_pocket->items[item_index].count)
        );
    }
    EXPECT_EQ(0, p_native_pocket->items[num_items].index);
    EXPECT_EQ(0, p_native_pocket->items[num_items].count);
    EXPECT_EQ(0xFF, p_native_pocket->terminator);
}

static const item_list_test_fcns_t gen2_test_fcns = boost::assign::map_list_of
    ("Items", &gen2_item_pocket_test)
    ("KeyItems", &gen2_key_item_pocket_test)
    ("Balls", &gen2_ball_pocket_test)
    ("TM/HM", &gen2_tmhm_pocket_test)
    ("PC", &gen2_item_pc_test)
;

TEST_P(gen2_item_list_test, item_list_test) {
    gen2_test_fcns.at(get_name())(get_item_list());
}

static const std::vector<std::pair<pkmn::e_game, std::string>> item_list_params =
{
    {pkmn::e_game::GOLD, "Items"},
    {pkmn::e_game::GOLD, "KeyItems"},
    {pkmn::e_game::GOLD, "Balls"},
    {pkmn::e_game::GOLD, "TM/HM"},
    {pkmn::e_game::GOLD, "PC"},
    {pkmn::e_game::SILVER, "Items"},
    {pkmn::e_game::SILVER, "KeyItems"},
    {pkmn::e_game::SILVER, "Balls"},
    {pkmn::e_game::SILVER, "TM/HM"},
    {pkmn::e_game::SILVER, "PC"},
    {pkmn::e_game::CRYSTAL, "Items"},
    {pkmn::e_game::CRYSTAL, "KeyItems"},
    {pkmn::e_game::CRYSTAL, "Balls"},
    {pkmn::e_game::CRYSTAL, "TM/HM"},
    {pkmn::e_game::CRYSTAL, "PC"},
};

INSTANTIATE_TEST_CASE_P(
    cpp_gen2_item_list_test,
    gen2_item_list_test,
    ::testing::ValuesIn(item_list_params)
);

class gen2_item_bag_test: public item_bag_test {};

TEST_P(gen2_item_bag_test, item_bag_test)
{
    const pkmn::item_bag::sptr& bag = get_item_bag();

    const pkmn::item_pockets_t& pockets = bag->get_pockets();
    ASSERT_EQ(4, pockets.size());
    ASSERT_EQ("Items", pockets.at("Items")->get_name());
    ASSERT_EQ(get_game(), pockets.at("Items")->get_game());
    ASSERT_EQ("KeyItems", pockets.at("KeyItems")->get_name());
    ASSERT_EQ(get_game(), pockets.at("KeyItems")->get_game());
    ASSERT_EQ("Balls", pockets.at("Balls")->get_name());
    ASSERT_EQ(get_game(), pockets.at("Balls")->get_game());
    ASSERT_EQ("TM/HM", pockets.at("TM/HM")->get_name());
    ASSERT_EQ(get_game(), pockets.at("TM/HM")->get_game());

    const std::vector<std::string>& pocket_names = bag->get_pocket_names();
    ASSERT_EQ(4, pockets.size());
    ASSERT_NE(pocket_names.end(), std::find(pocket_names.begin(), pocket_names.end(), "Items"));
    ASSERT_NE(pocket_names.end(), std::find(pocket_names.begin(), pocket_names.end(), "KeyItems"));
    ASSERT_NE(pocket_names.end(), std::find(pocket_names.begin(), pocket_names.end(), "Balls"));
    ASSERT_NE(pocket_names.end(), std::find(pocket_names.begin(), pocket_names.end(), "TM/HM"));

    gen2_item_pocket_test(pockets.at("Items"));
    gen2_key_item_pocket_test(pockets.at("KeyItems"));
    gen2_ball_pocket_test(pockets.at("Balls"));
    gen2_tmhm_pocket_test(pockets.at("TM/HM"));
    reset();

    // Make sure adding items through the bag adds to the proper pockets.
    ASSERT_EQ(0, bag->get_pocket("Items")->get_num_items());
    ASSERT_EQ(0, bag->get_pocket("KeyItems")->get_num_items());
    ASSERT_EQ(0, bag->get_pocket("Balls")->get_num_items());
    ASSERT_EQ(0, bag->get_pocket("TM/HM")->get_num_items());
    for(int item_index = 0; item_index < 8; ++item_index)
    {
        bag->add(
            ALL_POCKET_ITEMS[item_index],
            1
        );
    }

    const pkmn::item_slots_t& item_slots = bag->get_pocket("Items")->as_vector();
    const pkmn::item_slots_t& key_item_slots = bag->get_pocket("KeyItems")->as_vector();
    const pkmn::item_slots_t& ball_slots = bag->get_pocket("Balls")->as_vector();
    const pkmn::item_slots_t& tm_hm_slots = bag->get_pocket("TM/HM")->as_vector();

    EXPECT_EQ(pkmn::e_item::POTION, item_slots.at(0).item) << pkmn::item_to_string(item_slots.at(0).item);
    EXPECT_EQ(1, item_slots.at(0).amount);
    EXPECT_EQ(pkmn::e_item::BERRY, item_slots.at(1).item) << pkmn::item_to_string(item_slots.at(1).item);
    EXPECT_EQ(1, item_slots.at(1).amount);
    EXPECT_EQ(pkmn::e_item::NONE, item_slots.at(2).item) << pkmn::item_to_string(item_slots.at(2).item);
    EXPECT_EQ(0, item_slots.at(2).amount);

    EXPECT_EQ(pkmn::e_item::BICYCLE, key_item_slots.at(0).item) << pkmn::item_to_string(key_item_slots.at(0).item);
    EXPECT_EQ(1, key_item_slots.at(0).amount);
    EXPECT_EQ(pkmn::e_item::SQUIRT_BOTTLE, key_item_slots.at(1).item) << pkmn::item_to_string(key_item_slots.at(1).item);
    EXPECT_EQ(1, key_item_slots.at(1).amount);
    EXPECT_EQ(pkmn::e_item::NONE, key_item_slots.at(2).item) << pkmn::item_to_string(key_item_slots.at(2).item);
    EXPECT_EQ(0, key_item_slots.at(2).amount);

    EXPECT_EQ(pkmn::e_item::GREAT_BALL, ball_slots.at(0).item) << pkmn::item_to_string(ball_slots.at(0).item);
    EXPECT_EQ(1, ball_slots.at(0).amount);
    EXPECT_EQ(pkmn::e_item::FRIEND_BALL, ball_slots.at(1).item) << pkmn::item_to_string(ball_slots.at(1).item);
    EXPECT_EQ(1, ball_slots.at(1).amount);
    EXPECT_EQ(pkmn::e_item::NONE, ball_slots.at(2).item) << pkmn::item_to_string(ball_slots.at(2).item);
    EXPECT_EQ(0, ball_slots.at(2).amount);

    EXPECT_EQ(pkmn::e_item::TM01, tm_hm_slots.at(0).item) << pkmn::item_to_string(tm_hm_slots.at(0).item);
    EXPECT_EQ(0, tm_hm_slots.at(0).amount);
    EXPECT_EQ(pkmn::e_item::TM02, tm_hm_slots.at(1).item) << pkmn::item_to_string(tm_hm_slots.at(1).item);
    EXPECT_EQ(0, tm_hm_slots.at(1).amount);
    EXPECT_EQ(pkmn::e_item::TM28, tm_hm_slots.at(27).item) << pkmn::item_to_string(tm_hm_slots.at(7).item);
    EXPECT_EQ(1, tm_hm_slots.at(27).amount);
    EXPECT_EQ(pkmn::e_item::HM01, tm_hm_slots.at(50).item) << pkmn::item_to_string(tm_hm_slots.at(0).item);
    EXPECT_EQ(1, tm_hm_slots.at(50).amount);

    /*
     * On the C++ level, make sure the LibPKMN abstraction matches the underlying
     * PKSav struct.
     */
    const struct pksav_gen2_item_bag* native = reinterpret_cast<const struct pksav_gen2_item_bag*>(bag->get_native());
    EXPECT_EQ(2, native->item_pocket.count);
    for(int item_index = 0; item_index < 3; ++item_index)
    {
        EXPECT_EQ(
            pkmn::database::item_entry(
                item_slots.at(item_index).item,
                bag->get_game()
            ).get_item_index(),
            int(native->item_pocket.items[item_index].index)
        );
        EXPECT_EQ(
            item_slots.at(item_index).amount,
            int(native->item_pocket.items[item_index].count)
        );
    }
    EXPECT_EQ(0xFF, native->item_pocket.terminator);
    EXPECT_EQ(2, native->key_item_pocket.count);
    for(int item_index = 0; item_index < 3; ++item_index)
    {
        EXPECT_EQ(
            pkmn::database::item_entry(
                key_item_slots.at(item_index).item,
                bag->get_game()
            ).get_item_index(),
            int(native->key_item_pocket.item_indices[item_index])
        );
    }
    EXPECT_EQ(0xFF, native->key_item_pocket.terminator);
    EXPECT_EQ(2, native->ball_pocket.count);
    for(int item_index = 0; item_index < 3; ++item_index)
    {
        EXPECT_EQ(
            pkmn::database::item_entry(
                ball_slots.at(item_index).item,
                bag->get_game()
            ).get_item_index(),
            int(native->ball_pocket.items[item_index].index)
        );
        EXPECT_EQ(
            ball_slots.at(item_index).amount,
            int(native->ball_pocket.items[item_index].count)
        );
    }
    EXPECT_EQ(0xFF, native->ball_pocket.terminator);
    EXPECT_EQ(0, native->tmhm_pocket.tm_count[0]);
    EXPECT_EQ(0, native->tmhm_pocket.tm_count[1]);
    EXPECT_EQ(1, native->tmhm_pocket.tm_count[27]);
    EXPECT_EQ(1, native->tmhm_pocket.hm_count[0]);

    // Make sure removing items through the bag removes from the proper pockets.
    for(int item_index = 0; item_index < 8; ++item_index)
    {
        bag->remove(
            ALL_POCKET_ITEMS[item_index],
            1
        );
    }

    EXPECT_EQ(pkmn::e_item::NONE, item_slots.at(0).item);
    EXPECT_EQ(0, item_slots.at(0).amount);
    EXPECT_EQ(pkmn::e_item::NONE, item_slots.at(1).item);
    EXPECT_EQ(0, item_slots.at(1).amount);
    EXPECT_EQ(pkmn::e_item::NONE, item_slots.at(2).item);
    EXPECT_EQ(0, item_slots.at(2).amount);

    EXPECT_EQ(pkmn::e_item::NONE, key_item_slots.at(0).item);
    EXPECT_EQ(0, key_item_slots.at(0).amount);
    EXPECT_EQ(pkmn::e_item::NONE, key_item_slots.at(1).item);
    EXPECT_EQ(0, key_item_slots.at(1).amount);
    EXPECT_EQ(pkmn::e_item::NONE, key_item_slots.at(2).item);
    EXPECT_EQ(0, key_item_slots.at(2).amount);

    EXPECT_EQ(pkmn::e_item::NONE, ball_slots.at(0).item);
    EXPECT_EQ(0, ball_slots.at(0).amount);
    EXPECT_EQ(pkmn::e_item::NONE, ball_slots.at(1).item);
    EXPECT_EQ(0, ball_slots.at(1).amount);
    EXPECT_EQ(pkmn::e_item::NONE, ball_slots.at(2).item);
    EXPECT_EQ(0, ball_slots.at(2).amount);

    EXPECT_EQ(pkmn::e_item::TM01, tm_hm_slots.at(0).item);
    EXPECT_EQ(0, tm_hm_slots.at(0).amount);
    EXPECT_EQ(pkmn::e_item::TM02, tm_hm_slots.at(1).item);
    EXPECT_EQ(0, tm_hm_slots.at(1).amount);
    EXPECT_EQ(pkmn::e_item::TM28, tm_hm_slots.at(27).item);
    EXPECT_EQ(0, tm_hm_slots.at(27).amount);
    EXPECT_EQ(pkmn::e_item::HM01, tm_hm_slots.at(50).item);
    EXPECT_EQ(0, tm_hm_slots.at(50).amount);

    // Make sure we can't add/remove Crystal-specific items with a Gold/Silver bag.
    if(get_game() == pkmn::e_game::CRYSTAL)
    {
        for(int item_index = 0; item_index < 4; ++item_index)
        {
            bag->add(CRYSTAL_ITEMS[item_index], 1);
        }
        for(int item_index = 0; item_index < 4; ++item_index)
        {
            EXPECT_EQ(
                CRYSTAL_ITEMS[item_index],
                key_item_slots.at(item_index).item
            );
            EXPECT_EQ(
                1,
                key_item_slots.at(item_index).amount
            );
        }

        for(int item_index = 0; item_index < 4; ++item_index)
        {
            bag->remove(CRYSTAL_ITEMS[item_index], 1);
        }
        for(int item_index = 0; item_index < 4; ++item_index)
        {
            EXPECT_EQ(pkmn::e_item::NONE, key_item_slots.at(item_index).item);
            EXPECT_EQ(0, key_item_slots.at(item_index).amount);
        }
    }
    else
    {
        test_item_bag_invalid_items(
            bag,
            CRYSTAL_ITEMS
        );
    }

    // Make sure we can't add items from later generations.
    test_item_bag_invalid_items(
        bag,
        WRONG_GENERATION_ALL_POCKET_ITEMS
    );
}

INSTANTIATE_TEST_CASE_P(
    cpp_gen2_item_bag_test,
    gen2_item_bag_test,
    ::testing::ValuesIn(pkmntest::GEN2_GAMES)
);

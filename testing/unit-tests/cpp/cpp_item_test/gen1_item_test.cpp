/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "private_exports.hpp"

#include "item_test_common.hpp"

#include <pkmntest/util.hpp>

#include <pkmn/exception.hpp>
#include <pkmn/database/item_entry.hpp>
#include <pkmn/database/lists.hpp>
#include <pkmn/enums/enum_to_string.hpp>

#include "pksav/pksav_call.hpp"

#include <pksav/common/stats.h>
#include <pksav/gen1/items.h>
#include <pksav/math/endian.h>

#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>

#include <vector>

static const std::vector<pkmn::e_item> ITEMS =
{
    pkmn::e_item::POTION,
    pkmn::e_item::GREAT_BALL,
    pkmn::e_item::ETHER,
    pkmn::e_item::PP_UP,
    pkmn::e_item::TM34,
    pkmn::e_item::MOON_STONE,
    pkmn::e_item::BICYCLE,
    pkmn::e_item::FULL_HEAL
};
static const std::vector<pkmn::e_item> WRONG_GENERATION_ITEMS =
{
    pkmn::e_item::AMULET_COIN,
    pkmn::e_item::APICOT_BERRY,
    pkmn::e_item::AIR_MAIL,
    pkmn::e_item::AIR_BALLOON,
    pkmn::e_item::AQUA_SUIT
};
;

class gen1_item_list_test: public item_list_test {};

/*
 * On the C++ level, make sure the LibPKMN abstraction matches the underlying
 * PKSav struct.
 */
static void check_pksav_struct(
    const pkmn::item_slots_t& item_slots,
    pkmn::e_game game,
    int expected_num_items,
    const void* p_native,
    bool is_pc
)
{
    // This is safe because it has the same layout as the bag, but for the bag,
    // we won't index as far.
    const struct pksav_gen1_item_pc* p_native_pc = static_cast<const struct pksav_gen1_item_pc*>(p_native);
    ASSERT_EQ(expected_num_items, int(p_native_pc->count));
    for(int item_index = 0; item_index < expected_num_items; ++item_index)
    {
        EXPECT_EQ(
            pkmn::database::item_entry(
                item_slots.at(item_index).item,
                game
            ).get_item_index(),
            int(p_native_pc->items[item_index].index)
        );
        EXPECT_EQ(
            item_slots.at(item_index).amount,
            int(p_native_pc->items[item_index].count)
        );
    }

    EXPECT_EQ(0, p_native_pc->items[expected_num_items].index);
    EXPECT_EQ(0, p_native_pc->items[expected_num_items].count);

    if(is_pc)
    {
        EXPECT_EQ(0xFF, p_native_pc->terminator);
    }
    else
    {
        EXPECT_EQ(0xFF, reinterpret_cast<const struct pksav_gen1_item_bag*>(p_native_pc)->terminator);
    }
}

static void gen1_item_list_test_common(
    const pkmn::item_list::sptr& list
)
{
    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(list);

    // Confirm exceptions are thrown when expected.
    test_item_list_out_of_range_error(
        list,
        pkmn::e_item::POTION
    );

    // Confirm items from later generations can't be added.
    test_item_list_invalid_items(
        list,
        WRONG_GENERATION_ITEMS
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    test_item_list_add_remove(
        list,
        ITEMS
    );
    ASSERT_EQ(6, list->get_num_items());

    const std::vector<pkmn::e_item>& valid_items = list->get_valid_items();
    const std::vector<std::string>& valid_item_names = list->get_valid_item_names();
    ASSERT_EQ(valid_items.size(), valid_item_names.size());

    std::vector<pkmn::e_item> full_item_list = pkmn::database::get_item_list(list->get_game());
    EXPECT_EQ(full_item_list.size(), valid_items.size());

    std::vector<std::string> full_item_name_list = pkmn::database::get_item_name_list(list->get_game());
    EXPECT_EQ(full_item_name_list.size(), valid_item_names.size());

    check_pksav_struct(
        list->as_vector(),
        list->get_game(),
        list->get_num_items(),
        list->get_native(),
        (list->get_name() == "PC")
    );
}

static void gen1_item_pocket_test(
    const pkmn::item_list::sptr& item_pocket
)
{
    ASSERT_EQ("Items", item_pocket->get_name());
    ASSERT_EQ(20, item_pocket->get_capacity());
    ASSERT_EQ(20, item_pocket->as_vector().size());

    gen1_item_list_test_common(item_pocket);
}

static void gen1_item_pc_test(
    const pkmn::item_list::sptr& item_pc
)
{
    ASSERT_EQ("PC", item_pc->get_name());
    ASSERT_EQ(50, item_pc->get_capacity());
    ASSERT_EQ(50, item_pc->as_vector().size());

    gen1_item_list_test_common(item_pc);
}

static const item_list_test_fcns_t gen1_test_fcns = boost::assign::map_list_of
    ("Items", &gen1_item_pocket_test)
    ("PC", &gen1_item_pc_test)
;

TEST_P(gen1_item_list_test, item_list_test)
{
    gen1_test_fcns.at(get_name())(get_item_list());
}

static const std::vector<std::pair<pkmn::e_game, std::string>> item_list_params =
{
    {pkmn::e_game::RED, "Items"},
    {pkmn::e_game::RED, "PC"},
    {pkmn::e_game::BLUE, "Items"},
    {pkmn::e_game::BLUE, "PC"},
    {pkmn::e_game::YELLOW, "Items"},
    {pkmn::e_game::YELLOW, "PC"},
};

INSTANTIATE_TEST_CASE_P(
    cpp_gen1_item_list_test,
    gen1_item_list_test,
    ::testing::ValuesIn(item_list_params)
);

class gen1_item_bag_test: public item_bag_test {};

TEST_P(gen1_item_bag_test, item_bag_test)
{
    const pkmn::item_bag::sptr& bag = get_item_bag();

    const pkmn::item_pockets_t& pockets = bag->get_pockets();
    ASSERT_EQ(1, pockets.size());
    ASSERT_EQ("Items", pockets.at("Items")->get_name());
    ASSERT_EQ(get_game(), pockets.at("Items")->get_game());

    const std::vector<std::string>& pocket_names = bag->get_pocket_names();
    ASSERT_EQ(1, pockets.size());
    ASSERT_EQ("Items", pocket_names[0]);

    gen1_item_pocket_test(pockets.at("Items"));
    reset();

    // Make sure adding items through the bag adds to the pocket.
    const pkmn::item_list::sptr& item_pocket = bag->get_pocket("Items");
    const pkmn::item_slots_t& item_slots = item_pocket->as_vector();
    ASSERT_EQ(0, item_pocket->get_num_items());

    for(int item_index = 0; item_index < 8; ++item_index)
    {
        bag->add(
            ITEMS[item_index],
            item_index+1
        );
    }
    for(int item_index = 0; item_index < 8; ++item_index)
    {
        EXPECT_EQ(ITEMS[item_index], item_slots.at(item_index).item) << pkmn::item_to_string(ITEMS[item_index]);
        EXPECT_EQ(item_index+1, item_slots.at(item_index).amount) << pkmn::item_to_string(ITEMS[item_index]);
    }
    EXPECT_EQ(pkmn::e_item::NONE, item_slots.at(8).item);
    EXPECT_EQ(0, item_slots.at(8).amount);

    check_pksav_struct(
        item_slots,
        item_pocket->get_game(),
        8,
        bag->get_native(),
        false
    );

    // Make sure adding items through the bag removes from the pocket.
    for(int item_index = 0; item_index < 8; ++item_index)
    {
        bag->remove(
            ITEMS[item_index],
            item_index+1
        );
    }
    for(int item_index = 0; item_index < 9; ++item_index)
    {
        EXPECT_EQ(pkmn::e_item::NONE, item_slots.at(item_index).item);
        EXPECT_EQ(0, item_slots.at(item_index).amount);
    }
}

INSTANTIATE_TEST_CASE_P(
    cpp_gen1_item_bag_test,
    gen1_item_bag_test,
    ::testing::ValuesIn(pkmntest::GEN1_GAMES)
);

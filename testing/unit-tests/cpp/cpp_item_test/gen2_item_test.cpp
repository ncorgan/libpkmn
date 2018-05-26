/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "item_test_common.hpp"

#include <pkmn/exception.hpp>
#include <pkmn/database/item_entry.hpp>
#include <pkmn/database/lists.hpp>
#include "pksav/pksav_call.hpp"

#include <pksav/common/stats.h>
#include <pksav/gen2/items.h>
#include <pksav/math/endian.h>

#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>

#include <algorithm>

// TODO: check vector sizes against PKSav #defines

static const std::vector<std::string> all_pocket_item_names = boost::assign::list_of
    ("Potion")("Bicycle")("Great Ball")("TM28")
    ("Berry")("SquirtBottle")("Friend Ball")("HM01")
;
static const std::vector<std::string> crystal_items = boost::assign::list_of
    ("Clear Bell")("GS Ball")("Blue Card")("Egg Ticket")
;
static const std::vector<std::string> wrong_generation_all_pocket_items = boost::assign::list_of
    ("Black Sludge")("Poffin Items")("TM51")("Occa Berry")
;

class gen2_item_list_test: public item_list_test {};

void gen2_item_pocket_test(
    pkmn::item_list::sptr item_pocket
) {
    ASSERT_EQ("Items", item_pocket->get_name());
    ASSERT_EQ(20, item_pocket->get_capacity());
    ASSERT_EQ(20, item_pocket->as_vector().size());

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(item_pocket);

    // Confirm exceptions are thrown when expected.
    test_item_list_out_of_range_error(
        item_pocket,
        "Potion"
    );

    // Make sure we can't add items from other pockets.
    static const std::vector<std::string> wrong_pocket_items = boost::assign::list_of
        ("Bicycle")("Master Ball")("HM01")
    ;
    test_item_list_invalid_items(
        item_pocket,
        wrong_pocket_items
    );

    // Make sure we can't add items from later generations.
    static const std::vector<std::string> wrong_generation_items = boost::assign::list_of
        ("Black Flute")("Black Sludge")("Binding Band")("Beedrillite")
    ;
    test_item_list_invalid_items(
        item_pocket,
        wrong_generation_items
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    static const std::vector<std::string> item_names = boost::assign::list_of
        ("Potion")("HP Up")("Wht Apricorn")("Lucky Egg")
        ("Flower Mail")("Burn Heal")("PSNCureBerry")("Stick")
    ;
    test_item_list_add_remove(
        item_pocket,
        item_names
    );

    const std::vector<std::string>& valid_items = item_pocket->get_valid_items();
    EXPECT_GT(valid_items.size(), 0);

    /*
     * On the C++ level, make sure the LibPKMN abstraction matches the underlying
     * PKSav struct.
     */
    const pkmn::item_slots_t& item_slots = item_pocket->as_vector();
    int num_items = item_pocket->get_num_items();

    const struct pksav_gen2_item_pocket* native = reinterpret_cast<const pksav_gen2_item_pocket*>(item_pocket->get_native());
    EXPECT_EQ(num_items, int(native->count));
    for(int i = 0; i < num_items; ++i) {
        EXPECT_EQ(
            pkmn::database::item_entry(
                item_slots.at(i).item,
                item_pocket->get_game()
            ).get_item_index(),
            int(native->items[i].index)
        );
        EXPECT_EQ(item_slots.at(i).amount, int(native->items[i].count));
    }
    EXPECT_EQ(0, native->items[num_items].index);
    EXPECT_EQ(0, native->items[num_items].count);
    EXPECT_EQ(0xFF, native->terminator);
}

void gen2_key_item_pocket_test(
    pkmn::item_list::sptr key_item_pocket
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
        "Bicycle"
    );

    // Make sure we can't add items from other pockets.
    static const std::vector<std::string> wrong_pocket_items = boost::assign::list_of
        ("Potion")("Master Ball")("HM01")
    ;
    test_item_list_invalid_items(
        key_item_pocket,
        wrong_pocket_items
    );

    // Make sure we can't add items from later generations.
    static const std::vector<std::string> wrong_generation_items = boost::assign::list_of
        ("Mach Bike")("Jade Orb")("Light Stone")("Aqua Suit")
    ;
    test_item_list_invalid_items(
        key_item_pocket,
        wrong_generation_items
    );

    // Crystal-specific items.
    if(key_item_pocket->get_game() == "Crystal")
    {
        for(const std::string& item: crystal_items)
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
            crystal_items
        );
    }

    // Make sure we can't add or remove more than a single item.
    EXPECT_THROW(
        key_item_pocket->add("Bicycle", 5);
    , std::out_of_range);
    key_item_pocket->add("Bicycle", 1);
    EXPECT_EQ(1, key_item_pocket->get_num_items());

    EXPECT_THROW(
        key_item_pocket->remove("Bicycle", 5);
    , std::out_of_range);
    key_item_pocket->remove("Bicycle", 1);
    EXPECT_EQ(0, key_item_pocket->get_num_items());

    // Start adding and removing stuff, and make sure the numbers are accurate.
    static const std::vector<std::string> item_names = boost::assign::list_of
        ("Bicycle")("Basement Key")("SecretPotion")("Mystery Egg")
        ("Silver Wing")("Lost Item")("SquirtBottle")("Rainbow Wing")
    ;
    for(int item_index = 0; item_index < 8; ++item_index)
    {
        if(item_index < 4)
        {
            key_item_pocket->add(item_names[item_index], 1);
        }
        else
        {
            key_item_pocket->set_item(
                item_index,
                item_names[item_index],
                1
            );
        }
    }

    key_item_pocket->remove(item_names[2], 1);
    key_item_pocket->set_item(2, "None", 0);
    EXPECT_EQ(6, key_item_pocket->get_num_items());

    const std::vector<std::string>& valid_items = key_item_pocket->get_valid_items();
    EXPECT_LE(valid_items.size(), PKSAV_GEN2_KEY_ITEM_POCKET_SIZE);

    /*
     * On the C++ level, make sure the LibPKMN abstraction matches the underlying
     * PKSav struct.
     */
    const pkmn::item_slots_t& item_slots = key_item_pocket->as_vector();
    int num_items = key_item_pocket->get_num_items();

    const struct pksav_gen2_key_item_pocket* native = reinterpret_cast<const pksav_gen2_key_item_pocket*>(key_item_pocket->get_native());
    EXPECT_EQ(num_items, int(native->count));
    for(int item_index = 0; item_index < num_items; ++item_index)
    {
        EXPECT_EQ(
            pkmn::database::item_entry(
                item_slots.at(item_index).item,
                key_item_pocket->get_game()
            ).get_item_index(),
            int(native->item_indices[item_index])
        );
    }
    EXPECT_EQ(0, native->item_indices[num_items]);
    EXPECT_EQ(0xFF, native->terminator);
}

void gen2_ball_pocket_test(
    pkmn::item_list::sptr ball_pocket
) {
    ASSERT_EQ("Balls", ball_pocket->get_name());
    ASSERT_EQ(12, ball_pocket->get_capacity());
    ASSERT_EQ(12, ball_pocket->as_vector().size());

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(ball_pocket);

    // Confirm exceptions are thrown when expected.
    test_item_list_out_of_range_error(
        ball_pocket,
        "Master Ball"
    );

    // Make sure we can't add items from other pockets.
    static const std::vector<std::string> wrong_pocket_items = boost::assign::list_of
        ("Potion")("Bicycle")("HM01")
    ;
    test_item_list_invalid_items(
        ball_pocket,
        wrong_pocket_items
    );

    // Make sure we can't add items from later generations.
    static const std::vector<std::string> wrong_generation_items = boost::assign::list_of
        ("Premier Ball")("Heal Ball")("Dream Ball")
    ;
    test_item_list_invalid_items(
        ball_pocket,
        wrong_generation_items
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    static const std::vector<std::string> item_names = boost::assign::list_of
        ("Great Ball")("Poké Ball")("Park Ball")("Fast Ball")
        ("Master Ball")("Friend Ball")("Love Ball")("Level Ball")
    ;
    test_item_list_add_remove(
        ball_pocket,
        item_names
    );

    const std::vector<std::string>& valid_items = ball_pocket->get_valid_items();
    EXPECT_GT(valid_items.size(), 0);

    /*
     * On the C++ level, make sure the LibPKMN abstraction matches the underlying
     * PKSav struct.
     */
    const pkmn::item_slots_t& item_slots = ball_pocket->as_vector();
    int num_items = ball_pocket->get_num_items();

    const struct pksav_gen2_ball_pocket* native = reinterpret_cast<const pksav_gen2_ball_pocket*>(ball_pocket->get_native());
    EXPECT_EQ(num_items, int(native->count));
    for(int i = 0; i < num_items; ++i) {
        EXPECT_EQ(
            pkmn::database::item_entry(
                item_slots.at(i).item,
                ball_pocket->get_game()
            ).get_item_index(),
            int(native->items[i].index)
        );
        EXPECT_EQ(item_slots.at(i).amount, int(native->items[i].count));
    }
    EXPECT_EQ(0, native->items[num_items].index);
    EXPECT_EQ(0, native->items[num_items].count);
    EXPECT_EQ(0xFF, native->terminator);
}

void gen2_tmhm_pocket_test(
    pkmn::item_list::sptr tmhm_pocket
) {
    ASSERT_EQ("TM/HM", tmhm_pocket->get_name());
    ASSERT_EQ(57, tmhm_pocket->get_capacity());
    ASSERT_EQ(57, tmhm_pocket->as_vector().size());

    // Make sure item slots start as correctly empty.
    const pkmn::item_slots_t& item_slots = tmhm_pocket->as_vector();
    ASSERT_EQ(57, item_slots.size());
    for(int i = 1; i <= 50; ++i) {
        char name[5] = "";
        std::snprintf(name, sizeof(name), "TM%02d", i);
        EXPECT_EQ(name, item_slots[i-1].item);
        EXPECT_EQ(0, item_slots[i-1].amount);
    }
    for(int i = 1; i <= 7; ++i) {
        char name[5] = "";
        std::snprintf(name, sizeof(name), "HM%02d", i);
        EXPECT_EQ(name, item_slots[50+i-1].item);
        EXPECT_EQ(0, item_slots[50+i-1].amount);
    }

    // Confirm exceptions are thrown when expected.
    test_item_list_out_of_range_error(
        tmhm_pocket,
        "TM10"
    );

    // Make sure we can't add items from other pockets.
    static const std::vector<std::string> wrong_pocket_items = boost::assign::list_of
        ("Potion")("Master Ball")("Bicycle")
    ;
    test_item_list_invalid_items(
        tmhm_pocket,
        wrong_pocket_items
    );

    // Make sure we can't add items from later generations.
    static const std::vector<std::string> wrong_generation_items = boost::assign::list_of
        ("TM51")
    ;
    test_item_list_invalid_items(
        tmhm_pocket,
        wrong_generation_items
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    for(int i = 1; i <= 50; ++i) {
        char name[5] = "";
        std::snprintf(name, sizeof(name), "TM%02d", i);
        tmhm_pocket->add(name, 50);
        EXPECT_EQ(i, tmhm_pocket->get_num_items());
        EXPECT_EQ(name, item_slots[i-1].item);
        EXPECT_EQ(50, item_slots[i-1].amount);
    }
    for(int i = 50; i >= 1; --i) {
        char name[5] = "";
        std::snprintf(name, sizeof(name), "TM%02d", i);
        tmhm_pocket->remove(name, 50);
        EXPECT_EQ(i-1, tmhm_pocket->get_num_items());
        EXPECT_EQ(name, item_slots[i-1].item);
        EXPECT_EQ(0, item_slots[i-1].amount);
    }

    /*
     * Before removing items, on the C++ level, make sure the LibPKMN abstraction
     * matches the underlying PKSav struct.
     */
    const struct pksav_gen2_tmhm_pocket* native = reinterpret_cast<const pksav_gen2_tmhm_pocket*>(tmhm_pocket->get_native());
    for(int i = 0; i < 50; ++i) {
        EXPECT_EQ(item_slots[i].amount, int(native->tm_count[i]));
    }
    for(int i = 0; i < 7; ++i) {
        EXPECT_EQ(item_slots[50+i].amount, int(native->hm_count[i]));
    }

    for(int i = 1; i <= 7; ++i) {
        char name[5] = "";
        std::snprintf(name, sizeof(name), "HM%02d", i);
        tmhm_pocket->add(name, 1);
        EXPECT_EQ(i, tmhm_pocket->get_num_items());
        EXPECT_EQ(name, item_slots[50+i-1].item);
        EXPECT_EQ(1, item_slots[50+i-1].amount);
    }
    for(int i = 7; i >= 1; --i) {
        char name[5];
        std::snprintf(name, sizeof(name), "HM%02d", i);
        tmhm_pocket->remove(name, 1);
        EXPECT_EQ(i-1, tmhm_pocket->get_num_items());
        EXPECT_EQ(name, item_slots[50+i-1].item);
        EXPECT_EQ(0, item_slots[50+i-1].amount);
    }

    const std::vector<std::string>& valid_items = tmhm_pocket->get_valid_items();
    EXPECT_EQ(57, valid_items.size());
}

void gen2_item_pc_test(
    pkmn::item_list::sptr item_pc
) {
    ASSERT_EQ("PC", item_pc->get_name());
    ASSERT_EQ(50, item_pc->get_capacity());
    ASSERT_EQ(50, item_pc->as_vector().size());

    // Confirm exceptions are thrown when expected.
    test_item_list_out_of_range_error(
        item_pc,
        "Potion"
    );

    // Make sure item slots start as correctly empty.
    test_item_list_empty_slots(item_pc);

    // Crystal-specific items.
    if(item_pc->get_game() == "Crystal") {
        for(auto iter = crystal_items.begin(); iter != crystal_items.end(); ++iter) {
            item_pc->add(*iter, 1);
            item_pc->remove(*iter, 1);
        }

        EXPECT_EQ(0, item_pc->get_num_items());
    } else {
        test_item_list_invalid_items(
            item_pc,
            crystal_items
        );
    }

    // Make sure we can't add items from later generations.
    test_item_list_invalid_items(
        item_pc,
        wrong_generation_all_pocket_items
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    test_item_list_add_remove(
        item_pc,
        all_pocket_item_names
    );

    const std::vector<std::string>& valid_items = item_pc->get_valid_items();
    std::vector<std::string> full_item_list = pkmn::database::get_item_list(item_pc->get_game());
    EXPECT_EQ(full_item_list.size(), valid_items.size());

    /*
     * On the C++ level, make sure the LibPKMN abstraction matches the underlying
     * PKSav struct.
     */
    const pkmn::item_slots_t& item_slots = item_pc->as_vector();
    int num_items = item_pc->get_num_items();

    const struct pksav_gen2_item_pc* native = reinterpret_cast<const struct pksav_gen2_item_pc*>(item_pc->get_native());
    EXPECT_EQ(num_items, int(native->count));
    for(int i = 0; i < num_items; ++i) {
        EXPECT_EQ(
            pkmn::database::item_entry(
                item_slots.at(i).item,
                item_pc->get_game()
            ).get_item_index(),
            int(native->items[i].index)
        );
        EXPECT_EQ(item_slots.at(i).amount, int(native->items[i].count));
    }
    EXPECT_EQ(0, native->items[num_items].index);
    EXPECT_EQ(0, native->items[num_items].count);
    EXPECT_EQ(0xFF, native->terminator);
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

static const std::vector<std::pair<std::string, std::string>> item_list_params = {
    {"Gold", "Items"},
    {"Gold", "KeyItems"},
    {"Gold", "Balls"},
    {"Gold", "TM/HM"},
    {"Gold", "PC"},
    {"Silver", "Items"},
    {"Silver", "KeyItems"},
    {"Silver", "Balls"},
    {"Silver", "TM/HM"},
    {"Silver", "PC"},
    {"Crystal", "Items"},
    {"Crystal", "KeyItems"},
    {"Crystal", "Balls"},
    {"Crystal", "TM/HM"},
    {"Crystal", "PC"},
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
            all_pocket_item_names[item_index],
            1
        );
    }

    const pkmn::item_slots_t& item_slots = bag->get_pocket("Items")->as_vector();
    const pkmn::item_slots_t& key_item_slots = bag->get_pocket("KeyItems")->as_vector();
    const pkmn::item_slots_t& ball_slots = bag->get_pocket("Balls")->as_vector();
    const pkmn::item_slots_t& tm_hm_slots = bag->get_pocket("TM/HM")->as_vector();

    EXPECT_EQ("Potion", item_slots.at(0).item);
    EXPECT_EQ(1, item_slots.at(0).amount);
    EXPECT_EQ("Berry", item_slots.at(1).item);
    EXPECT_EQ(1, item_slots.at(1).amount);
    EXPECT_EQ("None", item_slots.at(2).item);
    EXPECT_EQ(0, item_slots.at(2).amount);

    EXPECT_EQ("Bicycle", key_item_slots.at(0).item);
    EXPECT_EQ(1, key_item_slots.at(0).amount);
    EXPECT_EQ("SquirtBottle", key_item_slots.at(1).item);
    EXPECT_EQ(1, key_item_slots.at(1).amount);
    EXPECT_EQ("None", key_item_slots.at(2).item);
    EXPECT_EQ(0, key_item_slots.at(2).amount);

    EXPECT_EQ("Great Ball", ball_slots.at(0).item);
    EXPECT_EQ(1, ball_slots.at(0).amount);
    EXPECT_EQ("Friend Ball", ball_slots.at(1).item);
    EXPECT_EQ(1, ball_slots.at(1).amount);
    EXPECT_EQ("None", ball_slots.at(2).item);
    EXPECT_EQ(0, ball_slots.at(2).amount);

    EXPECT_EQ("TM01", tm_hm_slots.at(0).item);
    EXPECT_EQ(0, tm_hm_slots.at(0).amount);
    EXPECT_EQ("TM02", tm_hm_slots.at(1).item);
    EXPECT_EQ(0, tm_hm_slots.at(1).amount);
    EXPECT_EQ("TM28", tm_hm_slots.at(27).item);
    EXPECT_EQ(1, tm_hm_slots.at(27).amount);
    EXPECT_EQ("HM01", tm_hm_slots.at(50).item);
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
            all_pocket_item_names[item_index],
            1
        );
    }

    EXPECT_EQ("None", item_slots.at(0).item);
    EXPECT_EQ(0, item_slots.at(0).amount);
    EXPECT_EQ("None", item_slots.at(1).item);
    EXPECT_EQ(0, item_slots.at(1).amount);
    EXPECT_EQ("None", item_slots.at(2).item);
    EXPECT_EQ(0, item_slots.at(2).amount);

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

    EXPECT_EQ("TM01", tm_hm_slots.at(0).item);
    EXPECT_EQ(0, tm_hm_slots.at(0).amount);
    EXPECT_EQ("TM02", tm_hm_slots.at(1).item);
    EXPECT_EQ(0, tm_hm_slots.at(1).amount);
    EXPECT_EQ("TM28", tm_hm_slots.at(27).item);
    EXPECT_EQ(0, tm_hm_slots.at(27).amount);
    EXPECT_EQ("HM01", tm_hm_slots.at(50).item);
    EXPECT_EQ(0, tm_hm_slots.at(50).amount);

    // Make sure we can't add/remove Crystal-specific items with a Gold/Silver bag.
    if(get_game() == "Crystal")
    {
        for(int item_index = 0; item_index < 4; ++item_index)
        {
            bag->add(crystal_items[item_index], 1);
        }
        for(int item_index = 0; item_index < 4; ++item_index)
        {
            EXPECT_EQ(
                crystal_items[item_index],
                key_item_slots.at(item_index).item
            );
            EXPECT_EQ(
                1,
                key_item_slots.at(item_index).amount
            );
        }

        for(int item_index = 0; item_index < 4; ++item_index)
        {
            bag->remove(crystal_items[item_index], 1);
        }
        for(int item_index = 0; item_index < 4; ++item_index)
        {
            EXPECT_EQ("None", key_item_slots.at(item_index).item);
            EXPECT_EQ(0, key_item_slots.at(item_index).amount);
        }
    }
    else
    {
        test_item_bag_invalid_items(
            bag,
            crystal_items
        );
    }

    // Make sure we can't add items from later generations.
    test_item_bag_invalid_items(
        bag,
        wrong_generation_all_pocket_items
    );
}

static const std::vector<std::string> item_bag_params =
{
    "Gold", "Silver", "Crystal"
};

INSTANTIATE_TEST_CASE_P(
    cpp_gen2_item_bag_test,
    gen2_item_bag_test,
    ::testing::ValuesIn(item_bag_params)
);

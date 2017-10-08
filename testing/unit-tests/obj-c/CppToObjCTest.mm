/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#import <PKMN-ObjC/Database/PKLists.h>
#import "CppToObjC.h"

#import <Foundation/Foundation.h>

#include <gtest/gtest.h>

TEST(CppToObjCTest, PKItemDatabaseEntryTest)
{
    @autoreleasepool
    {
        pkmn::database::item_entry item_entry_cpp("Potion", "Emerald");
        PKItemDatabaseEntry* itemEntryFromCpp = [CppToObjC createItemDatabaseEntryFromCpp:item_entry_cpp];

        EXPECT_STREQ(item_entry_cpp.get_name().c_str(), [[itemEntryFromCpp getName] UTF8String]);
        EXPECT_STREQ(item_entry_cpp.get_game().c_str(), [[itemEntryFromCpp getGame] UTF8String]);
        EXPECT_STREQ(item_entry_cpp.get_category().c_str(), [[itemEntryFromCpp getCategory] UTF8String]);
        EXPECT_STREQ(item_entry_cpp.get_pocket().c_str(), [[itemEntryFromCpp getPocket] UTF8String]);
        EXPECT_STREQ(item_entry_cpp.get_description().c_str(), [[itemEntryFromCpp getDescription] UTF8String]);
        EXPECT_EQ(item_entry_cpp.get_cost(), [[itemEntryFromCpp getCost] intValue]);
        EXPECT_EQ(item_entry_cpp.holdable(), [itemEntryFromCpp holdable]);
        EXPECT_EQ(item_entry_cpp.get_fling_power(), [[itemEntryFromCpp getFlingPower] intValue]);
        EXPECT_STREQ(item_entry_cpp.get_fling_effect().c_str(), [[itemEntryFromCpp getFlingEffect] UTF8String]);

        // Make sure when creating an entry from the Objective-C API itself, the equality check works.
        PKItemDatabaseEntry* itemEntryObjC = [[PKItemDatabaseEntry alloc] initWithName:@"Potion" andGame:@"Emerald"];
        EXPECT_TRUE([itemEntryObjC isEqual:itemEntryFromCpp]);
        EXPECT_TRUE([itemEntryObjC isEqualToEntry:itemEntryFromCpp]);
    }
}

TEST(CppToObjCTest, PKItemListTest)
{
    @autoreleasepool
    {
        pkmn::item_list::sptr item_list_cpp = pkmn::item_list::make("Items", "Emerald");
        PKItemList* itemListFromCpp = [CppToObjC createItemListFromCpp:item_list_cpp];

        EXPECT_STREQ(item_list_cpp->get_name().c_str(), [[itemListFromCpp getName] UTF8String]);
        EXPECT_STREQ(item_list_cpp->get_game().c_str(), [[itemListFromCpp getGame] UTF8String]);

        // Add an item through the C++ instance, and make sure it's reflected in the Objective-C instance.
        item_list_cpp->add("Potion", 39);
        EXPECT_EQ(1, [[itemListFromCpp getNumItems] intValue]);

        // TODO: figure out subscripting
        const pkmn::item_slot& item_slot_cpp = item_list_cpp->at(0);
        PKItemSlot* itemSlotObjC = (PKItemSlot*)[itemListFromCpp objectAtIndexedSubscript:0];
        EXPECT_STREQ(item_slot_cpp.item.c_str(), [itemSlotObjC->item UTF8String]);
        EXPECT_EQ(item_slot_cpp.amount, [itemSlotObjC->amount intValue]);

        // Remove item through the Objective-C instance, and make sure it's reflected in the C++ instance.
        [itemListFromCpp remove:@"Potion" numItems:@39];
        EXPECT_EQ(0, item_list_cpp->get_num_items());
        itemSlotObjC = (PKItemSlot*)[itemListFromCpp objectAtIndexedSubscript:0];
        EXPECT_STREQ(item_slot_cpp.item.c_str(), [itemSlotObjC->item UTF8String]);
        EXPECT_EQ(item_slot_cpp.amount, [itemSlotObjC->amount intValue]);

        // Make sure when creating an item list from the Objective-C API itself, it behaves as expected.
        PKItemList* itemListObjC = [[PKItemList alloc] initWithName:@"Berries" andGame:@"Emerald"];
        EXPECT_STREQ("Berries", [[itemListObjC getName] UTF8String]);
        EXPECT_STREQ("Emerald", [[itemListObjC getGame] UTF8String]);
        [itemListObjC add:@"Cheri Berry" numItems:@39];
        EXPECT_EQ(1, [[itemListObjC getNumItems] intValue]);
        itemSlotObjC = (PKItemSlot*)[itemListObjC objectAtIndexedSubscript:0];
        EXPECT_STREQ("Cheri Berry", [itemSlotObjC->item UTF8String]);
        EXPECT_EQ(39, [itemSlotObjC->amount intValue]);
    }
}

TEST(CppToObjCTest, PKItemSlotTest)
{
    @autoreleasepool
    {
        pkmn::item_slot item_slot_cpp("TM01", 50);
        PKItemSlot* itemSlotFromCpp = [CppToObjC createItemSlotFromCpp:item_slot_cpp];

        EXPECT_STREQ(item_slot_cpp.item.c_str(), [itemSlotFromCpp->item UTF8String]);
        EXPECT_EQ(item_slot_cpp.amount, [itemSlotFromCpp->amount intValue]);

        // Make sure when creating an item slot from the Objective-C API itself, the equality check works.
        PKItemSlot* itemSlotObjC = [[PKItemSlot alloc] initWithItem:@"TM01" andAmount:@50];
        EXPECT_TRUE([itemSlotObjC isEqual:itemSlotObjC]);
        EXPECT_TRUE([itemSlotObjC isEqualToItemSlot:itemSlotObjC]);
    }
}

TEST(CppToObjCTest, PKLevelUpMoveTest)
{
    @autoreleasepool
    {
        pkmn::database::levelup_move levelup_move_cpp(
                                         pkmn::database::move_entry("Scratch", "Emerald"),
                                         3
                                     );
        PKLevelupMove* levelupMoveFromCpp = [CppToObjC createLevelupMoveFromCpp:levelup_move_cpp];

        EXPECT_STREQ(levelup_move_cpp.move.get_name().c_str(), [[levelupMoveFromCpp->move getName] UTF8String]);
        EXPECT_STREQ(levelup_move_cpp.move.get_game().c_str(), [[levelupMoveFromCpp->move getGame] UTF8String]);
        EXPECT_EQ(levelup_move_cpp.level, [levelupMoveFromCpp->level intValue]);

        // Make sure when creating a level-up move from the Objective-C API itself, the equality check works.
        PKMoveDatabaseEntry* moveEntryObjC = [[PKMoveDatabaseEntry alloc] initWithName:@"Scratch" andGame:@"Emerald"];
        PKLevelupMove* levelupMoveObjC = [[PKLevelupMove alloc] initWithMove:moveEntryObjC andLevel:@3];
        EXPECT_TRUE([levelupMoveObjC isEqual:levelupMoveFromCpp]);
        EXPECT_TRUE([levelupMoveObjC isEqualToLevelupMove:levelupMoveFromCpp]);
    }
}

TEST(CppToObjCTest, PKMoveDatabaseEntryTest)
{
    @autoreleasepool
    {
        pkmn::database::move_entry move_entry_cpp("Scratch", "Emerald");
        PKMoveDatabaseEntry* moveEntryFromCpp = [CppToObjC createMoveDatabaseEntryFromCpp:move_entry_cpp];

        EXPECT_STREQ(move_entry_cpp.get_name().c_str(), [[moveEntryFromCpp getName] UTF8String]);
        EXPECT_STREQ(move_entry_cpp.get_game().c_str(), [[moveEntryFromCpp getGame] UTF8String]);
        EXPECT_STREQ(move_entry_cpp.get_description().c_str(), [[moveEntryFromCpp getDescription] UTF8String]);
        EXPECT_STREQ(move_entry_cpp.get_target().c_str(), [[moveEntryFromCpp getTarget] UTF8String]);
        EXPECT_STREQ(move_entry_cpp.get_damage_class().c_str(), [[moveEntryFromCpp getDamageClass] UTF8String]);
        EXPECT_EQ(move_entry_cpp.get_base_power(), [[moveEntryFromCpp getBasePower] intValue]);

        for(int num_pp = 0; num_pp < 4; ++num_pp)
        {
            EXPECT_EQ(move_entry_cpp.get_pp(num_pp), [[moveEntryFromCpp getPP:[NSNumber numberWithInt:num_pp]] intValue]);
        }

        EXPECT_DOUBLE_EQ(move_entry_cpp.get_accuracy(), [[moveEntryFromCpp getAccuracy] floatValue]);
        EXPECT_EQ(move_entry_cpp.get_priority(), [[moveEntryFromCpp getPriority] intValue]);
        EXPECT_STREQ(move_entry_cpp.get_effect().c_str(), [[moveEntryFromCpp getEffect] UTF8String]);
        EXPECT_STREQ(move_entry_cpp.get_contest_type().c_str(), [[moveEntryFromCpp getContestType] UTF8String]);
        EXPECT_STREQ(move_entry_cpp.get_contest_effect().c_str(), [[moveEntryFromCpp getContestEffect] UTF8String]);
        EXPECT_STREQ(move_entry_cpp.get_super_contest_effect().c_str(), [[moveEntryFromCpp getSuperContestEffect] UTF8String]);

        // Make sure when creating an entry from the Objective-C API itself, the equality check works.
        PKMoveDatabaseEntry* moveEntryObjC = [[PKMoveDatabaseEntry alloc] initWithName:@"Scratch" andGame:@"Emerald"];
        EXPECT_TRUE([moveEntryObjC isEqual:moveEntryFromCpp]);
        EXPECT_TRUE([moveEntryObjC isEqualToEntry:moveEntryFromCpp]);
    }
}

TEST(CppToObjCTest, PKMoveDatabaseEntryArrayTest)
{
    @autoreleasepool
    {
        pkmn::database::move_list_t move_list_cpp =
        {
            pkmn::database::move_entry("Scratch", "FireRed"),
            pkmn::database::move_entry("Swallow", "LeafGreen"),
            pkmn::database::move_entry("Toxic", "Emerald")
        };
        PKMoveDatabaseEntryArray* moveEntryArrayFromCpp = [CppToObjC createMoveDatabaseEntryArrayFromCpp:
                                                               pkmn::database::move_list_t(move_list_cpp)
                                                          ];
        EXPECT_EQ(move_list_cpp.size(), [[moveEntryArrayFromCpp count] unsignedLongLongValue]);
        for(size_t i = 0; i < move_list_cpp.size(); ++i)
        {
            EXPECT_STREQ(
                move_list_cpp[i].get_name().c_str(),
                [[[moveEntryArrayFromCpp objectAtIndexedSubscript:i] getName] UTF8String]
            );
            EXPECT_STREQ(
                move_list_cpp[i].get_game().c_str(),
                [[[moveEntryArrayFromCpp objectAtIndexedSubscript:i] getGame] UTF8String]
            );
        }

        // Add an entry through the Objective-C API, and make sure we can get it again.
        PKMoveDatabaseEntry* moveDatabaseEntry = [[PKMoveDatabaseEntry alloc] initWithName:@"Frenzy Plant"
                                                                              andGame:@"Ruby"];
        [moveEntryArrayFromCpp addEntry:moveDatabaseEntry];
        EXPECT_STREQ(
            [[moveDatabaseEntry getName] UTF8String],
            [[[moveEntryArrayFromCpp objectAtIndexedSubscript:3] getName] UTF8String]
        );
        EXPECT_STREQ(
            [[moveDatabaseEntry getGame] UTF8String],
            [[[moveEntryArrayFromCpp objectAtIndexedSubscript:3] getGame] UTF8String]
        );

        // Recreate this array purely through Objective-C, and check for equality.
        PKMoveDatabaseEntryArray* moveEntryArrayObjC = [[PKMoveDatabaseEntryArray alloc] init];

        PKMoveDatabaseEntry* entry1 = [[PKMoveDatabaseEntry alloc] initWithName:@"Scratch" andGame:@"FireRed"];
        PKMoveDatabaseEntry* entry2 = [[PKMoveDatabaseEntry alloc] initWithName:@"Swallow" andGame:@"LeafGreen"];
        PKMoveDatabaseEntry* entry3 = [[PKMoveDatabaseEntry alloc] initWithName:@"Toxic" andGame:@"Emerald"];
        PKMoveDatabaseEntry* entry4 = [[PKMoveDatabaseEntry alloc] initWithName:@"Frenzy Plant" andGame:@"Ruby"];

        [moveEntryArrayObjC addEntry:entry1];
        [moveEntryArrayObjC addEntry:entry2];
        [moveEntryArrayObjC addEntry:entry3];
        [moveEntryArrayObjC addEntry:entry4];

        EXPECT_TRUE([moveEntryArrayObjC isEqualToArray:moveEntryArrayFromCpp]);
    }
}

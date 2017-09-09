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

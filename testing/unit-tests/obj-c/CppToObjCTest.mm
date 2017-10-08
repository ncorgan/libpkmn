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

TEST(CppToObjCTest, PKPokemonDatabaseEntryTest)
{
    @autoreleasepool
    {
        pkmn::database::pokemon_entry pokemon_entry_cpp("Pikachu", "Omega Ruby", "");
        PKPokemonDatabaseEntry* pokemonEntryFromCpp = [CppToObjC createPokemonDatabaseEntryFromCpp:pokemon_entry_cpp];

        EXPECT_STREQ(pokemon_entry_cpp.get_name().c_str(), [[pokemonEntryFromCpp getName] UTF8String]);
        EXPECT_STREQ(pokemon_entry_cpp.get_game().c_str(), [[pokemonEntryFromCpp getGame] UTF8String]);
        EXPECT_STREQ(pokemon_entry_cpp.get_species().c_str(), [[pokemonEntryFromCpp getSpecies] UTF8String]);
        EXPECT_STREQ(pokemon_entry_cpp.get_pokedex_entry().c_str(), [[pokemonEntryFromCpp getPokedexEntry] UTF8String]);
        EXPECT_STREQ(pokemon_entry_cpp.get_form().c_str(), [[pokemonEntryFromCpp getForm] UTF8String]);
        EXPECT_DOUBLE_EQ(pokemon_entry_cpp.get_height(), [[pokemonEntryFromCpp getHeight] floatValue]);
        EXPECT_DOUBLE_EQ(pokemon_entry_cpp.get_weight(), [[pokemonEntryFromCpp getWeight] floatValue]);
        EXPECT_EQ(pokemon_entry_cpp.has_gender_differences(), [pokemonEntryFromCpp hasGenderDifferences]);
        EXPECT_EQ(pokemon_entry_cpp.get_base_friendship(), [[pokemonEntryFromCpp getBaseFriendship] intValue]);

        std::pair<std::string, std::string> cpp_types = pokemon_entry_cpp.get_types();
        PKStringPair* objcTypes = [pokemonEntryFromCpp getTypes];
        EXPECT_STREQ(cpp_types.first.c_str(), [objcTypes->first UTF8String]);
        EXPECT_STREQ(cpp_types.second.c_str(), [objcTypes->second UTF8String]);

        std::pair<std::string, std::string> cpp_abilities = pokemon_entry_cpp.get_abilities();
        PKStringPair* objcAbilities = [pokemonEntryFromCpp getAbilities];
        EXPECT_STREQ(cpp_abilities.first.c_str(), [objcAbilities->first UTF8String]);
        EXPECT_STREQ(cpp_abilities.second.c_str(), [objcAbilities->second UTF8String]);

        EXPECT_STREQ(pokemon_entry_cpp.get_hidden_ability().c_str(), [[pokemonEntryFromCpp getHiddenAbility] UTF8String]);

        std::pair<std::string, std::string> cpp_egg_groups = pokemon_entry_cpp.get_egg_groups();
        PKStringPair* objcEggGroups = [pokemonEntryFromCpp getEggGroups];
        EXPECT_STREQ(cpp_egg_groups.first.c_str(), [objcEggGroups->first UTF8String]);
        EXPECT_STREQ(cpp_egg_groups.second.c_str(), [objcEggGroups->second UTF8String]);

        std::map<std::string, int> cpp_base_stats = pokemon_entry_cpp.get_base_stats();
        PKStringNumberDictionary* objCBaseStats = [pokemonEntryFromCpp getBaseStats];
        for(auto iter = cpp_base_stats.begin(); iter != cpp_base_stats.end(); ++iter)
        {
            EXPECT_EQ(iter->second, [(NSNumber*)[objCBaseStats objectForKeyedSubscript:@(iter->first.c_str())] intValue]);
        }

        std::map<std::string, int> cpp_EV_yields = pokemon_entry_cpp.get_EV_yields();
        PKStringNumberDictionary* objCEVYields = [pokemonEntryFromCpp getEVYields];
        for(auto iter = cpp_EV_yields.begin(); iter != cpp_EV_yields.end(); ++iter)
        {
            EXPECT_EQ(iter->second, [(NSNumber*)[objCEVYields objectForKeyedSubscript:@(iter->first.c_str())] intValue]);
        }

        EXPECT_EQ(pokemon_entry_cpp.get_experience_yield(), [[pokemonEntryFromCpp getExperienceYield] intValue]);
        EXPECT_EQ(pokemon_entry_cpp.get_experience_at_level(50), [[pokemonEntryFromCpp getExperienceAtLevel:@50] intValue]);
        EXPECT_EQ(pokemon_entry_cpp.get_level_at_experience(50000), [[pokemonEntryFromCpp getLevelAtExperience:@50000] intValue]);

        pkmn::database::levelup_moves_t cpp_levelup_moves = pokemon_entry_cpp.get_levelup_moves();
        PKLevelupMoveArray* objCLevelupMoves = [pokemonEntryFromCpp getLevelupMoves];
        EXPECT_EQ(cpp_levelup_moves.size(), [[objCLevelupMoves count] unsignedLongLongValue]);

        pkmn::database::move_list_t cpp_tmhm_moves = pokemon_entry_cpp.get_tm_hm_moves();
        PKMoveDatabaseEntryArray* objCTMHMMoves = [pokemonEntryFromCpp getTMHMMoves];
        EXPECT_EQ(cpp_tmhm_moves.size(), [[objCTMHMMoves count] unsignedLongLongValue]);

        pkmn::database::move_list_t cpp_egg_moves = pokemon_entry_cpp.get_egg_moves();
        PKMoveDatabaseEntryArray* objCEggMoves = [pokemonEntryFromCpp getEggMoves];
        EXPECT_EQ(cpp_egg_moves.size(), [[objCEggMoves count] unsignedLongLongValue]);

        pkmn::database::move_list_t cpp_tutor_moves = pokemon_entry_cpp.get_tutor_moves();
        PKMoveDatabaseEntryArray* objCTutorMoves = [pokemonEntryFromCpp getTutorMoves];
        EXPECT_EQ(cpp_tutor_moves.size(), [[objCTutorMoves count] unsignedLongLongValue]);

        std::vector<std::string> cpp_forms = pokemon_entry_cpp.get_forms();
        PKStringArray* objCForms = [pokemonEntryFromCpp getForms];
        EXPECT_EQ(cpp_forms.size(), [[objCForms count] unsignedLongLongValue]);

        pkmn::database::pokemon_entries_t cpp_evolutions = pokemon_entry_cpp.get_evolutions();
        PKPokemonDatabaseEntryArray* objCEvolutions = [pokemonEntryFromCpp getEvolutions];
        EXPECT_EQ(cpp_evolutions.size(), [[objCEvolutions count] unsignedLongLongValue]);

        // Make sure when creating an entry from the Objective-C API itself, the equality check works.
        PKPokemonDatabaseEntry* pokemonEntryObjC = [[PKPokemonDatabaseEntry alloc] initWithName:@"Pikachu"
                                                                                   andGame:@"Omega Ruby"
                                                                                   andForm:@""];
        EXPECT_TRUE([pokemonEntryObjC isEqual:pokemonEntryFromCpp]);
        EXPECT_TRUE([pokemonEntryObjC isEqualToEntry:pokemonEntryFromCpp]);
    }
}

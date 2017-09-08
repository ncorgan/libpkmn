/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "objc_test_common.h"

#import <PKMN-ObjC/Database/PKLists.h>

#import <Foundation/Foundation.h>

//
// TODO: test exception cases when they don't crash anymore
//

static void AbilityListTest() {
    @try {
        PKStringArray* abilities = [PKLists getAbilityList:@6];

        NSUInteger size = [abilities count];
        if(size != 191) {
            @throw [NSException
                        exceptionWithName:@"NSRangeException"
                                   reason:@"size != 191"
                                 userInfo:nil
                   ];
        }

        NSString* first = [abilities objectAtIndexedSubscript:0];
        if(![first isEqual:@"Adaptability"]) {
            @throw [NSException
                        exceptionWithName:@"NSRangeException"
                                   reason:@"abilities[0] != \"Adaptability\""
                                 userInfo:nil
                   ];
        }

        NSString* last = [abilities objectAtIndexedSubscript:(size-1)];
        if(![last isEqual:@"Zen Mode"]) {
            @throw [NSException
                        exceptionWithName:@"NSRangeException"
                                   reason:@"abilities[0] != \"Zen Mode\""
                                 userInfo:nil
                   ];
        }
    }
    @catch (NSException* e) {
        NSLog(@"%s", [[e reason] UTF8String]);
        TEST_ASSERT(false);
    }
}

static void GameListTest() {
}

static void ItemListTest() {
}

static void LocationListTest() {
}

static void MoveListTest() {
}

static void NatureListTest() {
}

static void PokemonListTest() {
}

static void RegionListTest() {
}

// TODO
static void RibbonListTest() {
}

static void SuperTrainingMedalListTest() {
}

static void TypeListTest() {
}

PKMN_OBJC_TEST_MAIN(
    PKMN_OBJC_TEST(AbilityListTest)
    PKMN_OBJC_TEST(GameListTest)
    PKMN_OBJC_TEST(ItemListTest)
    PKMN_OBJC_TEST(LocationListTest)
    PKMN_OBJC_TEST(MoveListTest)
    PKMN_OBJC_TEST(NatureListTest)
    PKMN_OBJC_TEST(PokemonListTest)
    PKMN_OBJC_TEST(RegionListTest)
    PKMN_OBJC_TEST(RibbonListTest)
    PKMN_OBJC_TEST(SuperTrainingMedalListTest)
    PKMN_OBJC_TEST(TypeListTest)
)

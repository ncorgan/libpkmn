/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_OBJC_CPPTOOBJC_H
#define PKMN_OBJC_CPPTOOBJC_H

#import <PKMN-ObjC/PKItemSlot.h>
#import <PKMN-ObjC/Database/PKItemDatabaseEntry.h>
#import <PKMN-ObjC/PKItemSlotArray.h>
#import <PKMN-ObjC/Database/PKLevelupMove.h>
#import <PKMN-ObjC/Database/PKLevelupMoveArray.h>
#import <PKMN-ObjC/Database/PKMoveDatabaseEntry.h>
#import <PKMN-ObjC/Database/PKMoveDatabaseEntryArray.h>
#import <PKMN-ObjC/Database/PKPokemonDatabaseEntry.h>
#import <PKMN-ObjC/Database/PKPokemonDatabaseEntryArray.h>
#import <PKMN-ObjC/Types/PKStringArray.h>
#import <PKMN-ObjC/Types/PKStringNumberDictionary.h>
#import <PKMN-ObjC/Types/PKStringPair.h>

#include <pkmn/item_slot.hpp>
#include <pkmn/database/item_entry.hpp>
#include <pkmn/database/levelup_move.hpp>
#include <pkmn/database/move_entry.hpp>
#include <pkmn/database/pokemon_entry.hpp>

#import <Foundation/Foundation.h>

#include <cstdio>
#include <map>
#include <stdexcept>
#include <utility>
#include <vector>

#include "CXXException.h"

// andSecond:[NSString stringWithUTF8String:cppInstance.second.c_str()]];

#define PKMN_CPP_TO_OBJC(...) \
{ \
    @try { \
        __VA_ARGS__; \
    } \
    @catch(CXXException* cppException) { \
        printf("Caught CXXException\n"); \
        std::string errorMessage = [CppToObjC getMessageFromCXXException:cppException]; \
        @throw [NSException exceptionWithName:@"NSException" \
                            reason:[NSString stringWithUTF8String:errorMessage.c_str()] \
                            userInfo:nil \
               ]; \
    } \
    @catch(id anyObject) { \
        (void)anyObject; \
        @throw; \
    } \
    @finally { \
    } \
}

@interface PKItemDatabaseEntryFromCpp: PKItemDatabaseEntry

- (PKItemDatabaseEntryFromCpp*)initFromCpp: (const pkmn::database::item_entry&)cppInstance;

- (void)dealloc;

@end

@interface PKItemSlotArrayFromCpp: PKItemSlotArray

- (PKItemSlotArrayFromCpp*)initFromCpp: (pkmn::item_slots_t&)cppInstance;

- (void)dealloc;

@end

@interface PKLevelupMoveArrayFromCpp: PKLevelupMoveArray

- (PKLevelupMoveArrayFromCpp*)initFromCpp: (pkmn::database::levelup_moves_t&)cppInstance;

- (void)dealloc;

@end

@interface PKMoveDatabaseEntryFromCpp: PKMoveDatabaseEntry

- (PKMoveDatabaseEntryFromCpp*)initFromCpp: (const pkmn::database::move_entry&)cppInstance;

- (void)dealloc;

@end

@interface PKMoveDatabaseEntryArrayFromCpp: PKMoveDatabaseEntryArray

- (PKMoveDatabaseEntryArrayFromCpp*)initFromCpp: (pkmn::database::move_list_t&)cppInstance;

- (void)dealloc;

@end

@interface PKPokemonDatabaseEntryFromCpp: PKPokemonDatabaseEntry

- (PKPokemonDatabaseEntryFromCpp*)initFromCpp: (const pkmn::database::pokemon_entry&)cppInstance;

- (void)dealloc;

@end

@interface PKPokemonDatabaseEntryArrayFromCpp: PKPokemonDatabaseEntryArray

- (PKPokemonDatabaseEntryArrayFromCpp*)initFromCpp: (pkmn::database::pokemon_entries_t&)cppInstance;

- (void)dealloc;

@end

@interface PKStringArrayFromCpp: PKStringArray

- (PKStringArrayFromCpp*)initFromCpp: (std::vector<std::string>&)cppInstance;

- (void)dealloc;

@end

@interface PKStringNumberDictionaryFromCpp: PKStringNumberDictionary

- (PKStringNumberDictionaryFromCpp*)initFromCpp: (std::map<std::string, int>&)cppInstance;

- (void)dealloc;

@end

@interface CppToObjC: NSObject

+ (std::string)getMessageFromCXXException: (CXXException*)cxxException;

+ (PKItemDatabaseEntry*)createItemDatabaseEntryFromCpp: (const pkmn::database::item_entry&)cppInstance;

+ (PKItemSlot*)createItemSlotFromCpp: (const pkmn::item_slot&)cppInstance;

+ (PKItemSlotArray*)createItemSlotArrayFromCpp: (pkmn::item_slots_t&)cppInstance;

+ (PKLevelupMove*)createLevelupMoveFromCpp: (const pkmn::database::levelup_move&)cppInstance;

+ (PKLevelupMoveArray*)createLevelupMoveArrayFromCpp: (pkmn::database::levelup_moves_t&)cppInstance;

+ (PKMoveDatabaseEntry*)createMoveDatabaseEntryFromCpp: (const pkmn::database::move_entry&)cppInstance;

+ (PKMoveDatabaseEntryArray*)createMoveDatabaseEntryArrayFromCpp: (pkmn::database::move_list_t&)cppInstance;

+ (PKPokemonDatabaseEntry*)createPokemonDatabaseEntryFromCpp: (const pkmn::database::pokemon_entry&)cppInstance;

+ (PKPokemonDatabaseEntryArray*)createPokemonDatabaseEntryArrayFromCpp: (pkmn::database::pokemon_entries_t&)cppInstance;

+ (PKStringArray*)createStringArrayFromCpp: (std::vector<std::string>&)cppInstance;

+ (PKStringNumberDictionary*)createStringNumberDictionaryFromCpp: (std::map<std::string, int>&)cppInstance;

+ (PKStringPair*)createStringPairFromCpp: (const std::pair<std::string, std::string>&)cppInstance;

@end

#endif /* PKMN_OBJC_CPPTOOBJC_H */

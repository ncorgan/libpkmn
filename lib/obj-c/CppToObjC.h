/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_OBJC_CPPTOOBJC_H
#define PKMN_OBJC_CPPTOOBJC_H

#import <PKMN-ObjC/Database/PKItemDatabaseEntry.h>
#import <PKMN-ObjC/Database/PKLevelupMove.h>
#import <PKMN-ObjC/Database/PKMoveDatabaseEntry.h>
#import <PKMN-ObjC/Database/PKMoveDatabaseEntryArray.h>
#import <PKMN-ObjC/Types/PKStringArray.h>
#import <PKMN-ObjC/Types/PKStringNumberDictionary.h>
#import <PKMN-ObjC/Types/PKStringPair.h>

#include <pkmn/database/item_entry.hpp>
#include <pkmn/database/levelup_move.hpp>
#include <pkmn/database/move_entry.hpp>

#import <Foundation/Foundation.h>

#include <map>
#include <stdexcept>
#include <utility>
#include <vector>

#define PKMN_CPP_TO_OBJC(...) \
{ \
    NSString* errMsg; \
    NSString* objCErrName; \
    bool thrown = false; \
    @try { \
        try { __VA_ARGS__ } \
        catch(const std::out_of_range &e) { \
            errMsg = [NSString stringWithUTF8String:e.what()]; \
            objCErrName = @"NSRangeException"; \
            thrown = true; \
        } catch(const std::logic_error &e) { \
            errMsg = [NSString stringWithUTF8String:e.what()]; \
            objCErrName = @"NSInvalidArgumentException"; \
            thrown = true; \
        } catch(const std::runtime_error &e) { \
            errMsg = [NSString stringWithUTF8String:e.what()]; \
            objCErrName = @"NSGenericException"; \
            thrown = true; \
        } catch(const std::exception &e) { \
            errMsg = [NSString stringWithUTF8String:e.what()]; \
            objCErrName = @"NSGenericException"; \
            thrown = true; \
        } catch(...) { \
            errMsg = @"Unknown error."; \
            objCErrName = @"NSGenericException"; \
            thrown = true; \
        } \
        if(thrown) { \
            @throw [NSException \
                        exceptionWithName:objCErrName \
                        reason:errMsg \
                        userInfo:nil \
                   ]; \
        } \
    } \
    @catch(NSException* e) { \
        @throw; \
    } \
    @finally { \
    } \
}

@interface PKItemDatabaseEntryFromCpp: PKItemDatabaseEntry

- (PKItemDatabaseEntryFromCpp*)initFromCpp: (const pkmn::database::item_entry&)cppInstance;

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

@interface PKStringArrayFromCpp: PKStringArray

- (PKStringArrayFromCpp*)initFromCpp: (std::vector<std::string>&)cppInstance;

- (void)dealloc;

@end

@interface PKStringNumberDictionaryFromCpp: PKStringNumberDictionary

- (PKStringNumberDictionaryFromCpp*)initFromCpp: (std::map<std::string, int>&)cppInstance;

- (void)dealloc;

@end

@interface CppToObjC: NSObject

+ (PKItemDatabaseEntry*)createItemDatabaseEntryFromCpp: (const pkmn::database::item_entry&)cppInstance;

+ (PKLevelupMove*)createLevelupMoveFromCpp: (const pkmn::database::levelup_move&)cppInstance;

+ (PKMoveDatabaseEntry*)createMoveDatabaseEntryFromCpp: (const pkmn::database::move_entry&)cppInstance;

+ (PKMoveDatabaseEntryArray*)createMoveDatabaseEntryArrayFromCpp: (pkmn::database::move_list_t&)cppInstance;

+ (PKStringArray*)createStringArrayFromCpp: (std::vector<std::string>&)cppInstance;

+ (PKStringNumberDictionary*)createStringNumberDictionaryFromCpp: (std::map<std::string, int>&)cppInstance;

+ (PKStringPair*)createStringPairFromCpp: (const std::pair<std::string, std::string>&)cppInstance;

@end

#endif /* PKMN_OBJC_CPPTOOBJC_H */

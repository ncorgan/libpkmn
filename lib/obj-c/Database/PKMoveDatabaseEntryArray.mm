/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#import <PKMN-ObjC/Database/PKMoveDatabaseEntryArray.h>

#import "CppToObjC.h"

#include <pkmn/config.hpp>
#include <pkmn/database/move_entry.hpp>

#define CAST_TO_CPP(arr) reinterpret_cast<pkmn::database::move_list_t*>(arr->_internal)

/*
 * Ugly helper function to extract the internal C++ move entry from the Objective-C
 * wrapper. Objective-C has no friend classes, so we must use the Objective-C runtime
 * to access the protected variable.
 */
static pkmn::database::move_entry* getInternalMoveEntry(
    PKMoveDatabaseEntry* objC
) {
    id object = (id)objC;
    Ivar ivar = class_getInstanceVariable(object_getClass(object), "_internal");
    void* ivar_ptr = (uint8_t*)(__bridge void*)object + ivar_getOffset(ivar);

    pkmn::database::move_entry* ret_ptr = *((pkmn::database::move_entry**)ivar_ptr);
    return ret_ptr;
}

@implementation PKMoveDatabaseEntryArray

- (PKMoveDatabaseEntryArray*)init {
    PKMN_CPP_TO_OBJC(
        _internal = reinterpret_cast<void*>(
                        new pkmn::database::move_list_t
                    );
        return self;
    )
}

- (PKMoveDatabaseEntryArray*)initWithSize:(NSNumber*)size {
    PKMN_CPP_TO_OBJC(
        _internal = reinterpret_cast<void*>(
                        new pkmn::database::move_list_t(
                               [size unsignedLongLongValue]
                            )
                    );
        return self;
    )
}

- (void)dealloc {
    PKMN_CPP_TO_OBJC(
        delete CAST_TO_CPP(self);
        [super dealloc];
    )
}

- (id)objectAtIndexedSubscript:(NSUInteger)idx {
    PKMN_CPP_TO_OBJC(
        const pkmn::database::move_entry& cpp = (*CAST_TO_CPP(self))[idx];
        return [CppToObjC createMoveDatabaseEntryFromCpp:cpp];
    )
}

- (void)setObject:(id)obj atIndexedSubscript:(NSUInteger)idx {
    PKMN_CPP_TO_OBJC(
        pkmn::database::move_entry* cppPtr = getInternalMoveEntry((PKMoveDatabaseEntry*)obj);
        (*CAST_TO_CPP(self))[idx] = *cppPtr;
    )
}

- (NSNumber*)count {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->size());
    )
}

- (void)insertEntry:(PKMoveDatabaseEntry*)entry atIndex:(NSNumber*)idx {
    PKMN_CPP_TO_OBJC(
        pkmn::database::move_entry* cppPtr = getInternalMoveEntry(entry);
        CAST_TO_CPP(self)->insert(
            CAST_TO_CPP(self)->begin() + [idx unsignedLongLongValue],
            *cppPtr
        );
    )
}

- (void)removeEntryAtIndex:(NSNumber*)idx {
    PKMN_CPP_TO_OBJC(
        CAST_TO_CPP(self)->erase(
            CAST_TO_CPP(self)->begin() + [idx unsignedLongLongValue]
        );
    )
}

- (void)replaceEntryAtIndex:(NSNumber*)idx withEntry:(PKMoveDatabaseEntry*)entry {
    PKMN_CPP_TO_OBJC(
        pkmn::database::move_entry* cppPtr = getInternalMoveEntry(entry);
        (*CAST_TO_CPP(self))[[idx unsignedLongLongValue]] = *cppPtr;
    )
}

- (void)addEntry:(PKMoveDatabaseEntry*)entry {
    PKMN_CPP_TO_OBJC(
        pkmn::database::move_entry* cppPtr = getInternalMoveEntry(entry);
        CAST_TO_CPP(self)->push_back(*cppPtr);
    )
}

- (void)removeAllEntries {
    PKMN_CPP_TO_OBJC(
        CAST_TO_CPP(self)->clear();
    )
}

- (void)removeLastEntry {
    PKMN_CPP_TO_OBJC(
        CAST_TO_CPP(self)->pop_back();
    )
}

- (BOOL)isEqual:(id)object {
    if([object isKindOfClass:[PKMoveDatabaseEntryArray class]]) {
        PKMoveDatabaseEntryArray* other = object;
        return [self isEqualToArray:other];
    } else {
        return NO;
    }
}

- (BOOL)isEqualToArray:(PKMoveDatabaseEntryArray*)other {
    PKMN_CPP_TO_OBJC(
        return (*CAST_TO_CPP(self) == *CAST_TO_CPP(other));
    )
}

@end

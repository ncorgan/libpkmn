/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#import <PKMN-ObjC/Database/PKLevelupMoveArray.h>

#import "CppToObjC.h"

#include <pkmn/config.hpp>
#include <pkmn/database/levelup_move.hpp>

#define CAST_TO_CPP(arr) reinterpret_cast<pkmn::database::levelup_moves_t*>(arr->_internal)

/*
 * Ugly helper function to extract the internal C++ levelup move from the Objective-C
 * wrapper. Objective-C has no friend classes, so we must use the Objective-C runtime
 * to access the protected variable.
 */
static PKMN_INLINE pkmn::database::levelup_move* getInternalLevelupMove(
    PKLevelupMove* objC
) {
    void* cpp;
    object_getInstanceVariable(objC, "_internal", &cpp);
    return reinterpret_cast<pkmn::database::levelup_move*>(cpp);
}

@implementation PKLevelupMoveArray

- (PKLevelupMoveArray*)init {
    PKMN_CPP_TO_OBJC(
        _internal = reinterpret_cast<void*>(
                        new pkmn::database::levelup_moves_t
                    );
        return self;
    )
}

- (PKLevelupMoveArray*)initWithSize:(NSNumber*)size {
    PKMN_CPP_TO_OBJC(
        _internal = reinterpret_cast<void*>(
                        new pkmn::database::levelup_moves_t(
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

- (id)objectAtIndexedSubscript:(NSNumber*)idx {
    PKMN_CPP_TO_OBJC(
        const pkmn::database::levelup_move& cpp = (*CAST_TO_CPP(self))[[idx unsignedLongLongValue]];
        return [CppToObjC createLevelupMoveFromCpp:cpp];
    )
}

- (void)setObject:(id)obj atIndexedSubscript:(NSNumber*)idx {
    PKMN_CPP_TO_OBJC(
        pkmn::database::levelup_move* cppPtr = getInternalLevelupMove((PKLevelupMove*)obj);
        (*CAST_TO_CPP(self))[[idx unsignedLongLongValue]] = *cppPtr;
    )
}

- (NSNumber*)count {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->size());
    )
}

- (void)insertLevelupMove:(PKLevelupMove*)entry atIndex:(NSNumber*)idx {
    PKMN_CPP_TO_OBJC(
        pkmn::database::levelup_move* cppPtr = getInternalLevelupMove(entry);
        CAST_TO_CPP(self)->insert(
            CAST_TO_CPP(self)->begin() + [idx unsignedLongLongValue],
            *cppPtr
        );
    )
}

- (void)removeLevelupMoveAtIndex:(NSNumber*)idx {
    PKMN_CPP_TO_OBJC(
        CAST_TO_CPP(self)->erase(
            CAST_TO_CPP(self)->begin() + [idx unsignedLongLongValue]
        );
    )
}

- (void)replaceLevelupMoveAtIndex:(NSNumber*)idx withLevelupMove:(PKLevelupMove*)entry {
    PKMN_CPP_TO_OBJC(
        pkmn::database::levelup_move* cppPtr = getInternalLevelupMove(entry);
        (*CAST_TO_CPP(self))[[idx unsignedLongLongValue]] = *cppPtr;
    )
}

- (void)addLevelupMove:(PKLevelupMove*)entry {
    PKMN_CPP_TO_OBJC(
        pkmn::database::levelup_move* cppPtr = getInternalLevelupMove(entry);
        CAST_TO_CPP(self)->push_back(*cppPtr);
    )
}

- (void)removeAllEntries {
    PKMN_CPP_TO_OBJC(
        CAST_TO_CPP(self)->clear();
    )
}

- (void)removeLastLevelupMove {
    PKMN_CPP_TO_OBJC(
        CAST_TO_CPP(self)->pop_back();
    )
}

- (BOOL)isEqual:(id)object {
    if([object isKindOfClass:[PKLevelupMoveArray class]]) {
        PKLevelupMoveArray* other = object;
        return [self isEqualToArray:other];
    } else {
        return NO;
    }
}

- (BOOL)isEqualToArray:(PKLevelupMoveArray*)other {
    PKMN_CPP_TO_OBJC(
        return (*CAST_TO_CPP(self) == *CAST_TO_CPP(other));
    )
}

@end

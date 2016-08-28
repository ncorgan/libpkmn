/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#import <PKMN-ObjC/Database/PKMoveDatabaseEntryArray.h>

#import "CppToObjC.h"

#include <vector>

#define CAST_TO_CPP(arr) reinterpret_cast<pkmn::database::move_list_t*>(arr->_internal)

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

- (id)objectAtIndexedSubscript:(NSNumber*)idx {
    PKMN_CPP_TO_OBJC(
        pkmn::database::move_entry& cpp = (*CAST_TO_CPP(self))[[idx unsignedLongLongValue]];
        return [CppToObjC createMoveDatabaseEntryFromCpp:cpp];
    )
}

- (void)setObject:(id)obj atIndexedSubscript:(NSNumber*)idx {
    /*PKMN_CPP_TO_OBJC(
        PKMoveDatabaseEntry* str = (PKMoveDatabaseEntry*)obj;
        (*CAST_TO_CPP(self))[[idx unsignedLongLongValue]] = [str UTF8Entry];
    )*/
}

- (NSNumber*)count {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->size());
    )
}

- (void)insertEntry:(PKMoveDatabaseEntry*)entry atIndex:(NSNumber*)idx {
    /*PKMN_CPP_TO_OBJC(
        CAST_TO_CPP(self)->insert(
            CAST_TO_CPP(self)->begin() + [idx unsignedLongLongValue],
            [str UTF8Entry]
        );
    )*/
}

- (void)removeEntryAtIndex:(NSNumber*)idx {
    PKMN_CPP_TO_OBJC(
        CAST_TO_CPP(self)->erase(
            CAST_TO_CPP(self)->begin() + [idx unsignedLongLongValue]
        );
    )
}

- (void)replaceEntryAtIndex:(NSNumber*)idx withEntry:(PKMoveDatabaseEntry*)entry {
    /*PKMN_CPP_TO_OBJC(
        (*CAST_TO_CPP(self))[[idx unsignedLongLongValue]] = [str UTF8Entry];
    )*/
}

- (void)addEntry:(PKMoveDatabaseEntry*)entry {
    /*PKMN_CPP_TO_OBJC(
        CAST_TO_CPP(self)->push_back([str UTF8Entry]);
    )*/
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

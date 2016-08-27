/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#import <PKMN-ObjC/Database/PKItemDatabaseEntry.h>

#import "CppToObjC.h"

#include <pkmn/database/item_entry.hpp>

#define CAST_TO_CPP(entry) reinterpret_cast<pkmn::database::item_entry*>(entry->_internal)

@implementation PKItemDatabaseEntry

- (PKItemDatabaseEntry*)initNoneEntry {
    PKMN_CPP_TO_OBJC(
        _internal = reinterpret_cast<void*>(
                        new pkmn::database::item_entry
                    );
        return self;
    )
}

- (PKItemDatabaseEntry*)initWithName:(NSString*)itemName andGame:(NSString*)itemGame {
    PKMN_CPP_TO_OBJC(
        _internal = reinterpret_cast<void*>(
                        new pkmn::database::item_entry(
                            [itemName UTF8String], [itemGame UTF8String]
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

- (NSString*)getName {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_name().c_str());
    )
}

- (NSString*)getGame {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_game().c_str());
    )
}

- (NSString*)getCategory {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_category().c_str());
    )
}

- (NSString*)getPocket {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_pocket().c_str());
    )
}

- (NSString*)getDescription {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_description().c_str());
    )
}

- (NSNumber*)getCost {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_cost());
    )
}

- (BOOL)holdable {
    PKMN_CPP_TO_OBJC(
        return CAST_TO_CPP(self)->holdable();
    )
}

- (NSNumber*)getFlingPower {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_fling_power());
    )
}

- (NSString*)getFlingEffect {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_fling_effect().c_str());
    )
}

- (BOOL)isEqual:(id)object {
    if([object isKindOfClass:[PKItemDatabaseEntry class]]) {
        PKItemDatabaseEntry* other = object;
        return [self isEqualToEntry:other];
    } else {
        return NO;
    }
}

- (BOOL)isEqualToEntry:(PKItemDatabaseEntry*)other {
    PKMN_CPP_TO_OBJC(
        return (*CAST_TO_CPP(self) == *CAST_TO_CPP(other));
    )
}

@end

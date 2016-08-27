/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#import <PKMN-ObjC/Database/PKMoveDatabaseEntry.h>

#import "CppToObjC.h"

#include <pkmn/database/move_entry.hpp>

#define CAST_TO_CPP(entry) reinterpret_cast<pkmn::database::move_entry*>(entry->_internal)

@implementation PKMoveDatabaseEntry

- (PKMoveDatabaseEntry*)initNoneEntry {
    PKMN_CPP_TO_OBJC(
        _internal = reinterpret_cast<void*>(
                        new pkmn::database::move_entry
                    );
        return self;
    )
}

- (PKMoveDatabaseEntry*)initWithName:(NSString*)moveName andGame:(NSString*)gameName {
    PKMN_CPP_TO_OBJC(
        _internal = reinterpret_cast<void*>(
                        new pkmn::database::move_entry(
                            [moveName UTF8String], [gameName UTF8String]
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

- (NSString*)getType {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_type().c_str());
    )
}

- (NSString*)getDescription {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_description().c_str());
    )
}

- (NSString*)getDamageClass {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_damage_class().c_str());
    )
}

- (NSNumber*)getBasePower {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_base_power());
    )
}

- (NSNumber*)getPP: (NSNumber*)numPPUps {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_pp([numPPUps intValue]));
    )
}

- (NSNumber*)getAccuracy {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_accuracy());
    )
}

- (NSNumber*)getPriority {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_priority());
    )
}

- (NSString*)getEffect {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_effect().c_str());
    )
}

- (NSString*)getContestType {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_contest_type().c_str());
    )
}

- (NSString*)getContestEffect {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_contest_effect().c_str());
    )
}

- (NSString*)getSuperContestEffect {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_super_contest_effect().c_str());
    )
}

- (BOOL)isEqual:(id)object {
    if([object isKindOfClass:[PKMoveDatabaseEntry class]]) {
        PKMoveDatabaseEntry* other = object;
        return [self isEqualToEntry:other];
    } else {
        return NO;
    }
}

- (BOOL)isEqualToEntry:(PKMoveDatabaseEntry*)other {
    PKMN_CPP_TO_OBJC(
        return (*CAST_TO_CPP(self) == *CAST_TO_CPP(other));
    )
}

@end

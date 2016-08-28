/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#import <PKMN-ObjC/Database/PKLevelupMove.h>

#import "CppToObjC.h"

@implementation PKLevelupMove

- (PKLevelupMove*)init {
    move = [[PKMoveDatabaseEntry alloc] init];
    level = [NSNumber alloc];
    return self;
}

// TODO: add Obj-C level copy constructor
- (PKLevelupMove*)initWithMove:(PKMoveDatabaseEntry*)lmove
                      andLevel:(NSNumber*)moveLevel {

    move = [[PKMoveDatabaseEntry alloc] initWithName:[lmove getName]
                                             andGame:[lmove getGame]];
    level = [moveLevel copy];
    return self;
}

- (void)dealloc {
    [move dealloc];
    [level dealloc];
    [super dealloc];
}

- (BOOL)isEqual:(id)object {
    if([object isKindOfClass:[PKLevelupMove class]]) {
        PKLevelupMove* other = object;
        return [self isEqualToLevelupMove:other];
    } else {
        return NO;
    }
}

- (BOOL)isEqualToLevelupMove:(PKLevelupMove*)other {
    PKMN_CPP_TO_OBJC(
        return ([self->move isEqual:other->move] &&
                [self->level isEqual:other->level]);
    )
}

@end

/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#import <PKMN-ObjC/PKItemSlot.h>

#import "CppToObjC.h"

@implementation PKItemSlot

- (PKItemSlot*)init {
    item = [PKItemDatabaseEntry alloc];
    amount = [NSNumber alloc];
    return self;
}

// TODO: PKItemSlot copy constructor
- (PKItemSlot*)initWithItem:(PKItemDatabaseEntry*)slotItem
                  andAmount:(NSNumber*)slotAmount {
    item = [[PKItemDatabaseEntry alloc] initWithName:[slotItem getName]
                                             andGame:[slotItem getGame]];
    amount = [slotAmount copy];
    return self;
}

- (void)dealloc {
    [item dealloc];
    [amount dealloc];
    [super dealloc];
}

- (BOOL)isEqual:(id)object {
    if([object isKindOfClass:[PKItemSlot class]]) {
        PKItemSlot* other = object;
        return [self isEqualToItemSlot:other];
    } else {
        return NO;
    }
}

- (BOOL)isEqualToItemSlot:(PKItemSlot*)other {
    PKMN_CPP_TO_OBJC(
        return ([self->item isEqual:other->item] &&
                [self->amount isEqual:other->amount]);
    )
}

@end

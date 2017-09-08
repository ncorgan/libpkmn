/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#import <PKMN-ObjC/PKItemSlot.h>

#import "CppToObjC.h"

@implementation PKItemSlot

- (PKItemSlot*)init {
    item = [NSString alloc];
    amount = [NSNumber alloc];
    return self;
}

- (PKItemSlot*)initWithItem:(NSString*)slotItem
                  andAmount:(NSNumber*)slotAmount {
    item = [slotItem copy];
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
    return ([self->item isEqual:other->item] &&
            [self->amount isEqual:other->amount]);
}

@end

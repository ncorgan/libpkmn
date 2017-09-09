/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_OBJC_PKITEMSLOT_H
#define INCLUDED_PKMN_OBJC_PKITEMSLOT_H

#import <Foundation/Foundation.h>

@interface PKItemSlot: NSObject {
    @public
    NSString* item;
    NSNumber* amount;
}

- (PKItemSlot*)init;

- (PKItemSlot*)initWithItem:(NSString*)slotItem
                  andAmount:(NSNumber*)slotAmount;

- (void)dealloc;

- (BOOL)isEqual:(id)object;

- (BOOL)isEqualToItemSlot:(PKItemSlot*)other;

@end

#endif /* INCLUDED_PKMN_OBJC_PKITEMSLOT_H */

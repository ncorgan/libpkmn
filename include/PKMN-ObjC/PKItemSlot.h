/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_OBJC_PKITEMSLOT_H
#define INCLUDED_PKMN_OBJC_PKITEMSLOT_H

#import <PKMN-ObjC/Database/PKItemDatabaseEntry.h>

#import <Foundation/Foundation.h>

@interface PKItemSlot: NSObject {
    @public
    PKItemDatabaseEntry* item;
    NSNumber* amount;
}

- (PKItemSlot*)init;

- (PKItemSlot*)initWithItem:(PKItemDatabaseEntry*)slotItem
                  andAmount:(NSNumber*)moveLevel;

- (void)dealloc;

- (BOOL)isEqual:(id)object;

- (BOOL)isEqualToItemSlot:(PKItemSlot*)other;

@end

#endif /* INCLUDED_PKMN_OBJC_PKITEMSLOT_H */

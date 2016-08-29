/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_OBJC_ITEMSLOTARRAY_H
#define INCLUDED_PKMN_OBJC_ITEMSLOTARRAY_H

#import <PKMN-ObjC/PKItemSlot.h>

#import <Foundation/Foundation.h>

@interface PKItemSlotArray : NSObject {
    @protected
    void* _internal;
}

- (PKItemSlotArray*)init;

- (PKItemSlotArray*)initWithSize:(NSNumber*)size;

- (void)dealloc;

// Allow getting with []
- (id)objectAtIndexedSubscript:(NSNumber*)idx;

// Allow setting with []
- (void)setObject:(id)obj atIndexedSubscript:(NSNumber*)idx;

- (NSNumber*)count;

- (void)insertItemSlot:(PKItemSlot*)itemSlot atIndex:(NSNumber*)idx;

- (void)removeItemSlotAtIndex:(NSNumber*)idx;

- (void)replaceItemSlotAtIndex:(NSNumber*)idx withItemSlot:(PKItemSlot*)itemSlot;

- (void)addItemSlot:(PKItemSlot*)itemSlot;

- (void)removeAllEntries;

- (void)removeLastItemSlot;

- (BOOL)isEqual:(id)object;

- (BOOL)isEqualToArray:(PKItemSlotArray*)other;

@end

#endif /* INCLUDED_PKMN_OBJC_ITEMSLOTARRAY_H */

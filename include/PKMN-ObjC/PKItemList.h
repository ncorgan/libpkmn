/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_OBJC_ITEMLIST_H
#define INCLUDED_PKMN_OBJC_ITEMLIST_H

#import <Foundation/Foundation.h>

#import <PKMN-ObjC/PKItemSlot.h>
#import <PKMN-ObjC/PKItemSlotArray.h>
#import <PKMN-ObjC/Types/PKStringArray.h>

@interface PKItemList : NSObject {
    @protected
    void* _internal;
}

- (PKItemList*)initWithName:(NSString*)itemName
                    andGame:(NSString*)gameName;

- (void)dealloc;

// Allow getting with []
- (id)objectAtIndexedSubscript:(NSUInteger)idx;

// Allow setting with [] (TODO: after set_item is merged)

- (NSString*)getName;

- (NSString*)getGame;

- (NSNumber*)getCapacity;

- (NSNumber*)getNumItems;

- (PKItemSlot*)at:(NSNumber*)position;

- (void)add:(NSString*)itemName numItems:(NSNumber*)amount;

- (void)remove:(NSString*)itemName numItems:(NSNumber*)amount;

- (void)move:(NSNumber*)oldPosition
          to:(NSNumber*)newPosition;

- (PKStringArray*)getValidItems;

- (BOOL)isEqual:(id)object;

- (BOOL)isEqualToItemList:(PKItemList*)other;

@end

#endif /* INCLUDED_PKMN_OBJC_ITEMLIST_H */

/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#import <PKMN-ObjC/PKItemSlotArray.h>

#import "CppToObjC.h"

#include <pkmn/config.hpp>
#include <pkmn/item_slot.hpp>

#define CAST_TO_CPP(arr) reinterpret_cast<pkmn::item_slots_t*>(arr->_internal)

static PKMN_INLINE pkmn::item_slot item_slot_from_objc(
    PKItemSlot* objC
) {
    return pkmn::item_slot(
               [objC->item UTF8String],
               [objC->amount intValue]
           );
}

@implementation PKItemSlotArray

- (PKItemSlotArray*)init {
    PKMN_CPP_TO_OBJC(
        _internal = reinterpret_cast<void*>(
                        new pkmn::item_slots_t
                    );
        return self;
    )
}

- (PKItemSlotArray*)initWithSize:(NSNumber*)size {
    PKMN_CPP_TO_OBJC(
        _internal = reinterpret_cast<void*>(
                        new pkmn::item_slots_t(
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
        const pkmn::item_slot& cpp = (*CAST_TO_CPP(self))[[idx unsignedLongLongValue]];
        return [CppToObjC createItemSlotFromCpp:cpp];
    )
}

- (void)setObject:(id)obj atIndexedSubscript:(NSNumber*)idx {
    PKMN_CPP_TO_OBJC(
        pkmn::item_slot cpp(item_slot_from_objc((PKItemSlot*)obj));
        (*CAST_TO_CPP(self))[[idx unsignedLongLongValue]] = cpp;
    )
}

- (NSNumber*)count {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->size());
    )
}

- (void)insertItemSlot:(PKItemSlot*)itemSlot atIndex:(NSNumber*)idx {
    PKMN_CPP_TO_OBJC(
        pkmn::item_slot cpp(item_slot_from_objc(itemSlot));
        CAST_TO_CPP(self)->insert(
            CAST_TO_CPP(self)->begin() + [idx unsignedLongLongValue],
            cpp
        );
    )
}

- (void)removeItemSlotAtIndex:(NSNumber*)idx {
    PKMN_CPP_TO_OBJC(
        CAST_TO_CPP(self)->erase(
            CAST_TO_CPP(self)->begin() + [idx unsignedLongLongValue]
        );
    )
}

- (void)replaceItemSlotAtIndex:(NSNumber*)idx withItemSlot:(PKItemSlot*)itemSlot {
    PKMN_CPP_TO_OBJC(
        pkmn::item_slot cpp(item_slot_from_objc(itemSlot));
        (*CAST_TO_CPP(self))[[idx unsignedLongLongValue]] = cpp;
    )
}

- (void)addItemSlot:(PKItemSlot*)itemSlot {
    PKMN_CPP_TO_OBJC(
        pkmn::item_slot cpp(item_slot_from_objc(itemSlot));
        CAST_TO_CPP(self)->push_back(cpp);
    )
}

- (void)removeAllEntries {
    PKMN_CPP_TO_OBJC(
        CAST_TO_CPP(self)->clear();
    )
}

- (void)removeLastItemSlot {
    PKMN_CPP_TO_OBJC(
        CAST_TO_CPP(self)->pop_back();
    )
}

- (BOOL)isEqual:(id)object {
    if([object isKindOfClass:[PKItemSlotArray class]]) {
        PKItemSlotArray* other = object;
        return [self isEqualToArray:other];
    } else {
        return NO;
    }
}

- (BOOL)isEqualToArray:(PKItemSlotArray*)other {
    PKMN_CPP_TO_OBJC(
        return (*CAST_TO_CPP(self) == *CAST_TO_CPP(other));
    )
}

@end

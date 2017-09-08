/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#import <PKMN-ObjC/PKItemList.h>

#import "CppToObjC.h"

#include <pkmn/item_list.hpp>

#define CAST_TO_CPP(list) (*reinterpret_cast<pkmn::item_list::sptr*>(list))

@implementation PKItemList

- (PKItemList*)initWithName:(NSString*)itemName
                    andGame:(NSString*)gameName {
    PKMN_CPP_TO_OBJC(
        _internal = reinterpret_cast<void*>(
                        new pkmn::item_list::sptr(
                                pkmn::item_list::make(
                                    [itemName UTF8String],
                                    [gameName UTF8String]
                                )
                            )
                    );
    )
}

- (void)dealloc {
    delete &CAST_TO_CPP(self);
    [super dealloc];
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

- (NSNumber*)getCapacity {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_capacity());
    )
}

- (NSNumber*)getNumItems {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_num_items());
    )
}

- (PKItemSlot*)at:(NSNumber*)position {
    PKMN_CPP_TO_OBJC(
        const pkmn::item_slot& cpp = CAST_TO_CPP(self)->at(
                                         [position intValue]
                                     );
        return [CppToObjC createItemSlotFromCpp:cpp];
    )
}

- (void)add:(NSString*)itemName numItems:(NSNumber*)amount {
    PKMN_CPP_TO_OBJC(
        CAST_TO_CPP(self)->add(
            [itemName UTF8String],
            [amount intValue]
        );
    )
}

- (void)remove:(NSString*)itemName numItems:(NSNumber*)amount {
    PKMN_CPP_TO_OBJC(
        CAST_TO_CPP(self)->remove(
            [itemName UTF8String],
            [amount intValue]
        );
    )
}

- (void)move:(NSNumber*)oldPosition
          to:(NSNumber*)newPosition {
    PKMN_CPP_TO_OBJC(
        CAST_TO_CPP(self)->move(
            [oldPosition intValue],
            [newPosition intValue]
        );
    )
}

- (PKStringArray*)getValidItems
{
    PKMN_CPP_TO_OBJC(
        std::vector<std::string> valid_items = CAST_TO_CPP(self)->get_valid_items();
        return [CppToObjC createStringArrayFromCpp:std::move(valid_items)];
    )
}

- (PKItemSlotArray*)asArray {
    PKMN_CPP_TO_OBJC(
        pkmn::item_slots_t cpp = CAST_TO_CPP(self)->as_vector();
        return [CppToObjC createItemSlotArrayFromCpp:cpp];
    )
}

- (BOOL)isEqual:(id)object {
    if([object isKindOfClass:[PKItemList class]]) {
        PKItemList* other = object;
        return [self isEqualToItemList:other];
    } else {
        return NO;
    }
}

- (BOOL)isEqualToItemList:(PKItemSlot*)other {
    PKMN_CPP_TO_OBJC(
        return (CAST_TO_CPP(self) == CAST_TO_CPP(other));
    )
}

@end

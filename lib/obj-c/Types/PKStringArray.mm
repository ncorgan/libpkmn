/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#import <PKMN-ObjC/Types/PKStringArray.h>

#import "CppToObjC.h"

#include <vector>

#define CAST_TO_CPP(arr) reinterpret_cast<std::vector<std::string>*>(arr->_internal)

@implementation PKStringArray

- (PKStringArray*)init {
    PKMN_CPP_TO_OBJC(
        _internal = reinterpret_cast<void*>(
                        new std::vector<std::string>
                    );
        return self;
    )
}

- (PKStringArray*)initWithSize:(NSNumber*)size {
    PKMN_CPP_TO_OBJC(
        _internal = reinterpret_cast<void*>(
                        new std::vector<std::string>(
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
        const std::string& cpp = CAST_TO_CPP(self)->at(
                                     [idx unsignedLongLongValue]
                                 );
        return @(cpp.c_str());
    )
}

- (void)setObject:(id)obj atIndexedSubscript:(NSNumber*)idx {
    PKMN_CPP_TO_OBJC(
        NSString* str = (NSString*)obj;
        (*CAST_TO_CPP(self))[[idx unsignedLongLongValue]] = [str UTF8String];
    )
}

- (NSNumber*)count {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->size());
    )
}

- (void)insertString:(NSString*)str atIndex:(NSNumber*)idx {
    PKMN_CPP_TO_OBJC(
        CAST_TO_CPP(self)->insert(
            CAST_TO_CPP(self)->begin() + [idx unsignedLongLongValue],
            [str UTF8String]
        );
    )
}

- (void)removeStringAtIndex:(NSNumber*)idx {
    PKMN_CPP_TO_OBJC(
        CAST_TO_CPP(self)->erase(
            CAST_TO_CPP(self)->begin() + [idx unsignedLongLongValue]
        );
    )
}

- (void)replaceStringAtIndex:(NSNumber*)idx withString:(NSString*)str {
    PKMN_CPP_TO_OBJC(
        (*CAST_TO_CPP(self))[[idx unsignedLongLongValue]] = [str UTF8String];
    )
}

- (void)addString:(NSString*)str {
    PKMN_CPP_TO_OBJC(
        CAST_TO_CPP(self)->push_back([str UTF8String]);
    )
}

- (void)removeAllStrings {
    PKMN_CPP_TO_OBJC(
        CAST_TO_CPP(self)->clear();
    )
}

- (void)removeLastString {
    PKMN_CPP_TO_OBJC(
        CAST_TO_CPP(self)->pop_back();
    )
}

- (BOOL)isEqual:(id)object {
    if([object isKindOfClass:[PKStringArray class]]) {
        PKStringArray* other = object;
        return [self isEqualToArray:other];
    } else {
        return NO;
    }
}

- (BOOL)isEqualToArray:(PKStringArray*)other {
    PKMN_CPP_TO_OBJC(
        return (*CAST_TO_CPP(self) == *CAST_TO_CPP(other));
    )
}

@end

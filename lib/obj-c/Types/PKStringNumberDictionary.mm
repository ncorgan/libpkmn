/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#import <PKMN-ObjC/Types/PKStringNumberDictionary.h>

#import "CppToObjC.h"

#include <map>

#define CAST_TO_CPP(arr) reinterpret_cast<std::map<std::string,int>*>(arr->_internal)

@implementation PKStringNumberDictionary

- (PKStringNumberDictionary*)init {
    PKMN_CPP_TO_OBJC(
        _internal = reinterpret_cast<void*>(
                        new std::map<std::string,int>
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

- (id)objectForKeyedSubscript:(NSString*)key {
    PKMN_CPP_TO_OBJC(
        return (id)[self valueForKey:key];
    )
}

- (void)setObject:(id)obj forKeyedSubscript:(NSString*)key {
    PKMN_CPP_TO_OBJC(
        NSNumber* num = (NSNumber*)obj;
        [self setValue:num forKey:key];
    )
}

- (NSNumber*)count {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->size());
    )
}

- (void)setValue:(NSNumber*)val forKey:(NSString*)key {
    PKMN_CPP_TO_OBJC(
        (*CAST_TO_CPP(self))[[key UTF8String]] = [val intValue];
    )
}

- (NSNumber*)valueForKey:(NSString*)key {
    PKMN_CPP_TO_OBJC(
        int cpp = CAST_TO_CPP(self)->at([key UTF8String]);
        return [NSNumber numberWithInt:cpp];
    )
}

- (BOOL)isEqual:(id)object {
    if([object isKindOfClass:[PKStringNumberDictionary class]]) {
        PKStringNumberDictionary* other = object;
        return [self isEqualToDictionary:other];
    } else {
        return NO;
    }
}

- (BOOL)isEqualToDictionary:(PKStringNumberDictionary*)other {
    PKMN_CPP_TO_OBJC(
        return (*CAST_TO_CPP(self) == *CAST_TO_CPP(other));
    )
}

@end

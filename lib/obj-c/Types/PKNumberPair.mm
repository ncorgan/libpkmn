/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#import <PKMN-ObjC/Types/PKNumberPair.h>

#import "CppToObjC.h"

@implementation PKNumberPair

- (PKNumberPair*)init {
    first = [NSNumber alloc];
    second = [NSNumber alloc];
    return self;
}

- (PKNumberPair*)initWithFirst:(NSNumber*)firstNumber andSecond:(NSNumber*)secondNumber {
    first = [firstNumber copy];
    second = [secondNumber copy];
    return self;
}

- (void)dealloc {
    [first dealloc];
    [second dealloc];
    [super dealloc];
}

- (BOOL)isEqual:(id)object {
    if([object isKindOfClass:[PKNumberPair class]]) {
        PKNumberPair* other = object;
        return [self isEqualToPair:other];
    } else {
        return NO;
    }
}

- (BOOL)isEqualToPair:(PKNumberPair*)other {
    PKMN_CPP_TO_OBJC(
        return ([self->first isEqual:other->first] &&
                [self->second isEqual:other->second]);
    )
}

@end

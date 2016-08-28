/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#import <PKMN-ObjC/Types/PKStringPair.h>

#import "CppToObjC.h"

@implementation PKStringPair

- (PKStringPair*)init {
    first = [NSString alloc];
    second = [NSString alloc];
    return self;
}

- (PKStringPair*)initWithFirst:(NSString*)firstString andSecond:(NSString*)secondString {
    first = [firstString copy];
    second = [secondString copy];
    return self;
}

- (void)dealloc {
    [first dealloc];
    [second dealloc];
    [super dealloc];
}

- (BOOL)isEqual:(id)object {
    if([object isKindOfClass:[PKStringPair class]]) {
        PKStringPair* other = object;
        return [self isEqualToPair:other];
    } else {
        return NO;
    }
}

- (BOOL)isEqualToPair:(PKStringPair*)other {
    PKMN_CPP_TO_OBJC(
        return ([self->first isEqual:other->first] &&
                [self->second isEqual:other->second]);
    )
}

@end

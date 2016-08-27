/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#import <PKMN-ObjC/Types/PKStringPair.h>

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

@end

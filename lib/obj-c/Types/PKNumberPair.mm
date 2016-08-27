/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#import <PKMN-ObjC/Types/PKNumberPair.h>

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

@end

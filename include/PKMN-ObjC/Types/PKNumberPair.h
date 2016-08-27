/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_OBJC_TYPES_PKNUMBERPAIR_H
#define INCLUDED_PKMN_OBJC_TYPES_PKNUMBERPAIR_H

#import <Foundation/Foundation.h>

@interface PKNumberPair: NSObject {
    @public
    NSNumber* first;
    NSNumber* second;
}

- (PKNumberPair*)init;

- (PKNumberPair*)initWithFirst:(NSNumber*)firstNumber andSecond:(NSNumber*)secondNumber;

- (void)dealloc;

@end

#endif /* INCLUDED_PKMN_OBJC_TYPES_PKNUMBERPAIR_H */

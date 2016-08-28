/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_OBJC_TYPES_PKSTRINGPAIR_H
#define INCLUDED_PKMN_OBJC_TYPES_PKSTRINGPAIR_H

#import <Foundation/Foundation.h>

@interface PKStringPair: NSObject {
    @public
    NSString* first;
    NSString* second;
}

- (PKStringPair*)init;

- (PKStringPair*)initWithFirst:(NSString*)firstString andSecond:(NSString*)secondString;

- (void)dealloc;

- (BOOL)isEqual:(id)object;

- (BOOL)isEqualToPair:(PKStringPair*)other;

@end

#endif /* INCLUDED_PKMN_OBJC_TYPES_PKSTRINGPAIR_H */

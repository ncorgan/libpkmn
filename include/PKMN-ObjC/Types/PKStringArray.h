/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_OBJC_TYPES_PKSTRINGARRAY_H
#define INCLUDED_PKMN_OBJC_TYPES_PKSTRINGARRAY_H

#import <Foundation/Foundation.h>

@interface PKStringArray : NSObject {
    @protected
    void* _internal;
}

- (PKStringArray*)init;

- (PKStringArray*)initWithSize:(NSNumber*)size;

- (void)dealloc;

// Allow getting with []
- (id)objectAtIndexedSubscript:(NSNumber*)idx;

// Allow setting with []
- (void)setObject:(id)obj atIndexedSubscript:(NSNumber*)idx;

- (NSNumber*)count;

- (void)insertString:(NSString*)str atIndex:(NSNumber*)idx;

- (void)removeStringAtIndex:(NSNumber*)idx;

- (void)replaceStringAtIndex:(NSNumber*)idx withString:(NSString*)str;

- (void)addString:(NSString*)str;

- (void)removeAllStrings;

- (void)removeLastString;

- (BOOL)isEqual:(id)object;

- (BOOL)isEqualToArray:(PKStringArray*)other;

@end

#endif /* INCLUDED_PKMN_OBJC_TYPES_PKSTRINGARRAY_H */

/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_OBJC_TYPES_PKSTRINGNUMBERDICTIONARY_H
#define INCLUDED_PKMN_OBJC_TYPES_PKSTRINGNUMBERDICTIONARY_H

#import <Foundation/Foundation.h>

@interface PKStringNumberDictionary : NSObject {
    @protected
    void* _internal;
}

- (PKStringNumberDictionary*)init;

- (void)dealloc;

// Allow getting with []
- (id)objectForKeyedSubscript:(NSString*)key;

// Allow setting with []
- (void)setObject:(id)obj forKeyedSubscript:(NSString*)key;

- (NSNumber*)count;

- (void)setValue:(NSNumber*)val forKey:(NSString*)key;

- (NSNumber*)valueForKey:(NSString*)key;

- (BOOL)isEqual:(id)object;

- (BOOL)isEqualToDictionary:(PKStringNumberDictionary*)other;

@end

#endif /* INCLUDED_PKMN_OBJC_TYPES_PKSTRINGNUMBERDICTIONARY_H */

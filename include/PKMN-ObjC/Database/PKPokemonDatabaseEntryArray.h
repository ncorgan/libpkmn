/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_OBJC_DATABASE_PKPOKEMONDATABASEENTRYARRAY_H
#define INCLUDED_PKMN_OBJC_DATABASE_PKPOKEMONDATABASEENTRYARRAY_H

#import <PKMN-ObjC/Database/PKPokemonDatabaseEntry.h>

#import <Foundation/Foundation.h>

@interface PKPokemonDatabaseEntryArray : NSObject {
    @protected
    void* _internal;
}

- (PKPokemonDatabaseEntryArray*)init;

- (PKPokemonDatabaseEntryArray*)initWithSize:(NSNumber*)size;

- (void)dealloc;

// Allow getting with []
- (id)objectAtIndexedSubscript:(NSNumber*)idx;

// Allow setting with []
- (void)setObject:(id)obj atIndexedSubscript:(NSNumber*)idx;

- (NSNumber*)count;

- (void)insertEntry:(PKPokemonDatabaseEntry*)entry atIndex:(NSNumber*)idx;

- (void)removeEntryAtIndex:(NSNumber*)idx;

- (void)replaceEntryAtIndex:(NSNumber*)idx withEntry:(PKPokemonDatabaseEntry*)entry;

- (void)addEntry:(PKPokemonDatabaseEntry*)entry;

- (void)removeAllEntries;

- (void)removeLastEntry;

- (BOOL)isEqual:(id)object;

- (BOOL)isEqualToArray:(PKPokemonDatabaseEntryArray*)other;

@end

#endif /* INCLUDED_PKMN_OBJC_DATABASE_PKPOKEMONDATABASEENTRYARRAY_H */

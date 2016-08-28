/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_OBJC_DATABASE_PKMOVEDATABASEENTRYARRAY_H
#define INCLUDED_PKMN_OBJC_DATABASE_PKMOVEDATABASEENTRYARRAY_H

#import <PKMN-ObjC/Database/PKMoveDatabaseEntry.h>

#import <Foundation/Foundation.h>

@interface PKMoveDatabaseEntryArray : NSObject {
    @protected
    void* _internal;
}

- (PKMoveDatabaseEntryArray*)init;

- (PKMoveDatabaseEntryArray*)initWithSize:(NSNumber*)size;

- (void)dealloc;

// Allow getting with []
- (id)objectAtIndexedSubscript:(NSNumber*)idx;

// Allow setting with []
- (void)setObject:(id)obj atIndexedSubscript:(NSNumber*)idx;

- (NSNumber*)count;

- (void)insertEntry:(PKMoveDatabaseEntry*)entry atIndex:(NSNumber*)idx;

- (void)removeEntryAtIndex:(NSNumber*)idx;

- (void)replaceEntryAtIndex:(NSNumber*)idx withEntry:(PKMoveDatabaseEntry*)entry;

- (void)addEntry:(PKMoveDatabaseEntry*)entry;

- (void)removeAllEntries;

- (void)removeLastEntry;

- (BOOL)isEqual:(id)object;

- (BOOL)isEqualToArray:(PKMoveDatabaseEntryArray*)other;

@end

#endif /* INCLUDED_PKMN_OBJC_DATABASE_PKMOVEDATABASEENTRYARRAY_H */

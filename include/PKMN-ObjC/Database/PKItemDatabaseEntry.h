/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_OBJC_DATABASE_PKITEMDATABASE_ENTRY_H
#define INCLUDED_PKMN_OBJC_DATABASE_PKITEMDATABASE_ENTRY_H

#import <Foundation/Foundation.h>

@interface PKItemDatabaseEntry : NSObject {
    @protected
    void* _internal;
}

- (PKItemDatabaseEntry*)init;

- (PKItemDatabaseEntry*)initWithName:(NSString*)itemName andGame:(NSString*)gameName;

- (void)dealloc;

- (NSString*)getName;

- (NSString*)getGame;

- (NSString*)getCategory;

- (NSString*)getPocket;

- (NSString*)getDescription;

- (NSNumber*)getCost;

- (BOOL)holdable;

- (NSNumber*)getFlingPower;

- (NSString*)getFlingEffect;

- (BOOL)isEqual:(id)object;

- (BOOL)isEqualToEntry:(PKItemDatabaseEntry*)other;

@end

#endif /* INCLUDED_PKMN_OBJC_DATABASE_PKITEMDATABASE_ENTRY_H */

/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_OBJC_DATABASE_PKMOVEDATABASE_ENTRY_H
#define INCLUDED_PKMN_OBJC_DATABASE_PKMOVEDATABASE_ENTRY_H

#import <Foundation/Foundation.h>

@interface PKMoveDatabaseEntry : NSObject {
    @protected
    void* _internal;
}

- (PKMoveDatabaseEntry*)initNoneEntry;

- (PKMoveDatabaseEntry*)initWithName:(NSString*)moveName andGame:(NSString*)gameName;

- (void)dealloc;

- (NSString*)getName;

- (NSString*)getGame;

- (NSString*)getType;

- (NSString*)getDescription;

- (NSString*)getDamageClass;

- (NSNumber*)getBasePower;

- (NSNumber*)getPP:(NSNumber*)numPPUps;

- (NSNumber*)getAccuracy;

- (NSNumber*)getPriority;

- (NSString*)getEffect;

- (NSString*)getContestType;

- (NSString*)getContestEffect;

- (NSString*)getSuperContestEffect;

- (BOOL)isEqual:(id)object;

- (BOOL)isEqualToEntry:(PKMoveDatabaseEntry*)other;

@end

#endif /* INCLUDED_PKMN_OBJC_DATABASE_PKMOVEDATABASE_ENTRY_H */

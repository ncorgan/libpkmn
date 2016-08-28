/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_OBJC_DATABASE_PKPOKEMONDATABASE_ENTRY_H
#define INCLUDED_PKMN_OBJC_DATABASE_PKPOKEMONDATABASE_ENTRY_H

#import <PKMN-ObjC/Database/PKLevelupMoveArray.h>
#import <PKMN-ObjC/Database/PKMoveDatabaseEntryArray.h>
#import <PKMN-ObjC/Types/PKStringArray.h>
#import <PKMN-ObjC/Types/PKStringNumberDictionary.h>
#import <PKMN-ObjC/Types/PKStringPair.h>

#import <Foundation/Foundation.h>

@interface PKPokemonDatabaseEntry : NSObject {
    @protected
    void* _internal;
}

- (PKPokemonDatabaseEntry*)init;

- (PKPokemonDatabaseEntry*)initWithName:(NSString*)pokemonName
                                andGame:(NSString*)gameName
                                andForm:(NSString*)formName;

- (void)dealloc;

- (NSString*)getName;

- (NSString*)getGame;

- (NSString*)getSpecies;

- (NSString*)getPokedexEntry;

- (NSString*)getForm;

- (NSNumber*)getHeight;

- (NSNumber*)getWeight;

- (NSNumber*)getChanceMale;

- (NSNumber*)getChanceFemale;

- (BOOL)hasGenderDifferences;

- (NSNumber*)getBaseHappiness;

- (PKStringPair*)getTypes;

- (PKStringPair*)getAbilities;

- (NSString*)getHiddenAbility;

- (PKStringPair*)getEggGroups;

- (PKStringNumberDictionary*)getBaseStats;

- (PKStringNumberDictionary*)getEVYields;

- (NSNumber*)getExperienceYield;

- (NSNumber*)getExperienceAtLevel:(NSNumber*)level;

- (NSNumber*)getLevelAtExperience:(NSNumber*)experience;

- (PKLevelupMoveArray*)getLevelupMoves;

- (PKMoveDatabaseEntryArray*)getTMHMMoves;

- (PKMoveDatabaseEntryArray*)getEggMoves;

- (PKMoveDatabaseEntryArray*)getTutorMoves;

- (PKStringArray*)getForms;

// TODO: evolutions

- (BOOL)isEqual:(id)object;

- (BOOL)isEqualToEntry:(PKPokemonDatabaseEntry*)other;

@end

#endif /* INCLUDED_PKMN_OBJC_DATABASE_PKPOKEMONDATABASE_ENTRY_H */

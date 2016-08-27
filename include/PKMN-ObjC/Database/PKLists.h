/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_OBJC_DATABASE_PKLISTS_H
#define INCLUDED_PKMN_OBJC_DATABASE_PKLISTS_H

#import <PKMN-ObjC/Types/PKStringArray.h>

#import <Foundation/Foundation.h>

@interface PKLists : NSObject

+ (PKStringArray*)getAbilityList:(NSNumber*)generation;

+ (PKStringArray*)getGameList:(NSNumber*)generation includePrevious:(BOOL)includePrevious;

+ (PKStringArray*)getItemList:(NSString*)game;

+ (PKStringArray*)getLocationList:(NSString*)game wholeGeneration:(BOOL)wholeGeneration;

+ (PKStringArray*)getMoveList:(NSString*)game;

+ (PKStringArray*)getNatureList;

+ (PKStringArray*)getPokemonList:(NSNumber*)generation includePrevious:(BOOL)includePrevious;

+ (PKStringArray*)getRegionList;

+ (PKStringArray*)getRibbonList:(NSNumber*)generation;

+ (PKStringArray*)getSuperTrainingMedalList;

+ (PKStringArray*)getTypeList:(NSString*)game;

@end

#endif /* INCLUDED_PKMN_OBJC_DATABASE_PKLISTS_H */

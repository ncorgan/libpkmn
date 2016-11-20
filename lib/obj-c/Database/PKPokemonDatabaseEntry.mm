/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#import <PKMN-ObjC/Database/PKPokemonDatabaseEntry.h>

#import "CppToObjC.h"

#include <pkmn/database/pokemon_entry.hpp>

#define CAST_TO_CPP(entry) reinterpret_cast<pkmn::database::pokemon_entry*>(entry->_internal)

@implementation PKPokemonDatabaseEntry

- (PKPokemonDatabaseEntry*)init {
    PKMN_CPP_TO_OBJC(
        _internal = reinterpret_cast<void*>(
                        new pkmn::database::pokemon_entry
                    );
        return self;
    )
}

- (PKPokemonDatabaseEntry*)initWithName:(NSString*)pokemonName
                                andGame:(NSString*)gameName
                                andForm:(NSString*)formName {
    PKMN_CPP_TO_OBJC(
        _internal = reinterpret_cast<void*>(
                        new pkmn::database::pokemon_entry(
                            [pokemonName UTF8String],
                            [gameName UTF8String],
                            [formName UTF8String]
                        )
                    );
        return self;
    )
}

- (void)dealloc {
    PKMN_CPP_TO_OBJC(
        delete CAST_TO_CPP(self);
        [super dealloc];
    )
}

- (NSString*)getName {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_name().c_str());
    )
}

- (NSString*)getGame {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_game().c_str());
    )
}

- (NSString*)getSpecies {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_species().c_str());
    )
}

- (NSString*)getPokedexEntry {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_pokedex_entry().c_str());
    )
}

- (NSString*)getForm {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_form().c_str());
    )
}

- (NSNumber*)getHeight {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_height());
    )
}

- (NSNumber*)getWeight {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_weight());
    )
}

- (NSNumber*)getChanceMale {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_chance_male());
    )
}

- (NSNumber*)getChanceFemale {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_chance_female());
    )
}

- (BOOL)hasGenderDifferences {
    PKMN_CPP_TO_OBJC(
        return CAST_TO_CPP(self)->has_gender_differences();
    )
}

- (NSNumber*)getBaseHappiness {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_base_happiness());
    )
}

- (PKStringPair*)getTypes {
    PKMN_CPP_TO_OBJC(
        std::pair<std::string, std::string> cpp = CAST_TO_CPP(self)->get_types();
        return [CppToObjC createStringPairFromCpp:cpp];
    )
}

- (PKStringPair*)getAbilities {
    PKMN_CPP_TO_OBJC(
        std::pair<std::string, std::string> cpp = CAST_TO_CPP(self)->get_abilities();
        return [CppToObjC createStringPairFromCpp:cpp];
    )
}

- (NSString*)getHiddenAbility {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_hidden_ability().c_str());
    )
}

- (PKStringPair*)getEggGroups {
    PKMN_CPP_TO_OBJC(
        std::pair<std::string, std::string> cpp = CAST_TO_CPP(self)->get_egg_groups();
        return [CppToObjC createStringPairFromCpp:cpp];
    )
}

- (PKStringNumberDictionary*)getBaseStats {
    PKMN_CPP_TO_OBJC(
        std::map<std::string, int> cpp = CAST_TO_CPP(self)->get_base_stats();
        return [CppToObjC createStringNumberDictionaryFromCpp:cpp];
    )
}

- (PKStringNumberDictionary*)getEVYields {
    PKMN_CPP_TO_OBJC(
        std::map<std::string, int> cpp = CAST_TO_CPP(self)->get_EV_yields();
        return [CppToObjC createStringNumberDictionaryFromCpp:cpp];
    )
}

- (NSNumber*)getExperienceYield {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_experience_yield());
    )
}

- (NSNumber*)getExperienceAtLevel:(NSNumber*)level {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_experience_at_level(
                     [level intValue]
                 ));
    )
}

- (NSNumber*)getLevelAtExperience:(NSNumber*)experience {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->get_level_at_experience(
                     [experience intValue]
                 ));
    )
}

- (PKLevelupMoveArray*)getLevelupMoves {
    PKMN_CPP_TO_OBJC(
        pkmn::database::levelup_moves_t cpp = CAST_TO_CPP(self)->get_levelup_moves();
        return [CppToObjC createLevelupMoveArrayFromCpp:cpp];
    )
}

- (PKMoveDatabaseEntryArray*)getTMHMMoves {
    PKMN_CPP_TO_OBJC(
        pkmn::database::move_list_t cpp = CAST_TO_CPP(self)->get_tm_hm_moves();
        return [CppToObjC createMoveDatabaseEntryArrayFromCpp:cpp];
    )
}

- (PKMoveDatabaseEntryArray*)getEggMoves {
    PKMN_CPP_TO_OBJC(
        pkmn::database::move_list_t cpp = CAST_TO_CPP(self)->get_egg_moves();
        return [CppToObjC createMoveDatabaseEntryArrayFromCpp:cpp];
    )
}

- (PKMoveDatabaseEntryArray*)getTutorMoves {
    PKMN_CPP_TO_OBJC(
        pkmn::database::move_list_t cpp = CAST_TO_CPP(self)->get_tutor_moves();
        return [CppToObjC createMoveDatabaseEntryArrayFromCpp:cpp];
    )
}

- (PKStringArray*)getForms {
    PKMN_CPP_TO_OBJC(
        std::vector<std::string> cpp = CAST_TO_CPP(self)->get_forms();
        return [CppToObjC createStringArrayFromCpp:cpp];
    )
}

- (PKPokemonDatabaseEntryArray*)getEvolutions {
    PKMN_CPP_TO_OBJC(
        pkmn::database::pokemon_entries_t cpp = CAST_TO_CPP(self)->get_evolutions();
        return [CppToObjC createPokemonDatabaseEntryArrayFromCpp:cpp];
    )
}

- (BOOL)isEqual:(id)object {
    if([object isKindOfClass:[PKPokemonDatabaseEntry class]]) {
        PKPokemonDatabaseEntry* other = object;
        return [self isEqualToEntry:other];
    } else {
        return NO;
    }
}

- (BOOL)isEqualToEntry:(PKPokemonDatabaseEntry*)other {
    PKMN_CPP_TO_OBJC(
        return (*CAST_TO_CPP(self) == *CAST_TO_CPP(other));
    )
}

@end

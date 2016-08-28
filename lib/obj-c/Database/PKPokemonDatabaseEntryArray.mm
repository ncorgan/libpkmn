/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#import <PKMN-ObjC/Database/PKPokemonDatabaseEntryArray.h>

#import "CppToObjC.h"

#include <pkmn/config.hpp>
#include <pkmn/database/pokemon_entry.hpp>

#define CAST_TO_CPP(arr) reinterpret_cast<pkmn::database::pokemon_entries_t*>(arr->_internal)

/*
 * Ugly helper function to extract the internal C++ Pokémon entry from the Objective-C
 * wrapper. Objective-C has no friend classes, so we must use the Objective-C runtime
 * to access the protected variable.
 */
static PKMN_INLINE pkmn::database::pokemon_entry* getInternalPokemonEntry(
    PKPokemonDatabaseEntry* objC
) {
    void* cpp;
    object_getInstanceVariable(objC, "_internal", &cpp);
    return reinterpret_cast<pkmn::database::pokemon_entry*>(cpp);
}

@implementation PKPokemonDatabaseEntryArray

- (PKPokemonDatabaseEntryArray*)init {
    PKMN_CPP_TO_OBJC(
        _internal = reinterpret_cast<void*>(
                        new pkmn::database::pokemon_entries_t
                    );
        return self;
    )
}

- (PKPokemonDatabaseEntryArray*)initWithSize:(NSNumber*)size {
    PKMN_CPP_TO_OBJC(
        _internal = reinterpret_cast<void*>(
                        new pkmn::database::pokemon_entries_t(
                               [size unsignedLongLongValue]
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

- (id)objectAtIndexedSubscript:(NSNumber*)idx {
    PKMN_CPP_TO_OBJC(
        const pkmn::database::pokemon_entry& cpp = (*CAST_TO_CPP(self))[[idx unsignedLongLongValue]];
        return [CppToObjC createPokemonDatabaseEntryFromCpp:cpp];
    )
}

- (void)setObject:(id)obj atIndexedSubscript:(NSNumber*)idx {
    PKMN_CPP_TO_OBJC(
        pkmn::database::pokemon_entry* cppPtr = getInternalPokemonEntry((PKPokemonDatabaseEntry*)obj);
        (*CAST_TO_CPP(self))[[idx unsignedLongLongValue]] = *cppPtr;
    )
}

- (NSNumber*)count {
    PKMN_CPP_TO_OBJC(
        return @(CAST_TO_CPP(self)->size());
    )
}

- (void)insertEntry:(PKPokemonDatabaseEntry*)entry atIndex:(NSNumber*)idx {
    PKMN_CPP_TO_OBJC(
        pkmn::database::pokemon_entry* cppPtr = getInternalPokemonEntry(entry);
        CAST_TO_CPP(self)->insert(
            CAST_TO_CPP(self)->begin() + [idx unsignedLongLongValue],
            *cppPtr
        );
    )
}

- (void)removeEntryAtIndex:(NSNumber*)idx {
    PKMN_CPP_TO_OBJC(
        CAST_TO_CPP(self)->erase(
            CAST_TO_CPP(self)->begin() + [idx unsignedLongLongValue]
        );
    )
}

- (void)replaceEntryAtIndex:(NSNumber*)idx withEntry:(PKPokemonDatabaseEntry*)entry {
    PKMN_CPP_TO_OBJC(
        pkmn::database::pokemon_entry* cppPtr = getInternalPokemonEntry(entry);
        (*CAST_TO_CPP(self))[[idx unsignedLongLongValue]] = *cppPtr;
    )
}

- (void)addEntry:(PKPokemonDatabaseEntry*)entry {
    PKMN_CPP_TO_OBJC(
        pkmn::database::pokemon_entry* cppPtr = getInternalPokemonEntry(entry);
        CAST_TO_CPP(self)->push_back(*cppPtr);
    )
}

- (void)removeAllEntries {
    PKMN_CPP_TO_OBJC(
        CAST_TO_CPP(self)->clear();
    )
}

- (void)removeLastEntry {
    PKMN_CPP_TO_OBJC(
        CAST_TO_CPP(self)->pop_back();
    )
}

- (BOOL)isEqual:(id)object {
    if([object isKindOfClass:[PKPokemonDatabaseEntryArray class]]) {
        PKPokemonDatabaseEntryArray* other = object;
        return [self isEqualToArray:other];
    } else {
        return NO;
    }
}

- (BOOL)isEqualToArray:(PKPokemonDatabaseEntryArray*)other {
    PKMN_CPP_TO_OBJC(
        return (*CAST_TO_CPP(self) == *CAST_TO_CPP(other));
    )
}

@end

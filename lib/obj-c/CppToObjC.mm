/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#import "CppToObjC.h"

@implementation PKItemDatabaseEntryFromCpp

- (PKItemDatabaseEntryFromCpp*)initFromCpp: (pkmn::database::item_entry&)cppInstance {
    PKMN_CPP_TO_OBJC(
        _internal = reinterpret_cast<void*>(
                        new pkmn::database::item_entry((pkmn::database::item_entry&&)cppInstance)
                    );
        return self;
    )
}

- (void)dealloc {
    [super dealloc];
}

@end

@implementation PKMoveDatabaseEntryFromCpp

- (PKMoveDatabaseEntryFromCpp*)initFromCpp: (pkmn::database::move_entry&)cppInstance {
    PKMN_CPP_TO_OBJC(
        _internal = reinterpret_cast<void*>(
                        new pkmn::database::move_entry((pkmn::database::move_entry&&)cppInstance)
                    );
        return self;
    )
}

- (void)dealloc {
    [super dealloc];
}

@end

@implementation PKStringArrayFromCpp

- (PKStringArrayFromCpp*)initFromCpp: (std::vector<std::string>&)cppInstance {
    PKMN_CPP_TO_OBJC(
        _internal = reinterpret_cast<void*>(
                        new std::vector<std::string>((std::vector<std::string>&&)cppInstance)
                    );
        return self;
    )
}

- (void)dealloc {
    [super dealloc];
}

@end

@implementation PKStringNumberDictionaryFromCpp

- (PKStringNumberDictionaryFromCpp*)initFromCpp: (std::map<std::string, int>&)cppInstance {
    PKMN_CPP_TO_OBJC(
        _internal = reinterpret_cast<void*>(
                        new std::map<std::string, int>((std::map<std::string, int>&&)cppInstance)
                    );
        return self;
    )
}

- (void)dealloc {
    [super dealloc];
}

@end

@implementation CppToObjC

+ (PKItemDatabaseEntry*)createItemDatabaseEntryFromCpp: (pkmn::database::item_entry&)cppInstance {
    PKMN_CPP_TO_OBJC(
        return (PKItemDatabaseEntry*)[[PKItemDatabaseEntryFromCpp alloc] initFromCpp:cppInstance];
    )
}

+ (PKLevelupMove*)createLevelupMoveFromCpp: (pkmn::database::levelup_move&)cppInstance {
    PKMN_CPP_TO_OBJC(
        PKLevelupMove* ret = [[PKLevelupMove alloc] init];
        ret->move = [CppToObjC createMoveDatabaseEntryFromCpp:cppInstance.move];
        ret->level = @(cppInstance.level);

        return ret;
    )
}

+ (PKMoveDatabaseEntry*)createMoveDatabaseEntryFromCpp: (pkmn::database::move_entry&)cppInstance {
    PKMN_CPP_TO_OBJC(
        return (PKMoveDatabaseEntry*)[[PKMoveDatabaseEntryFromCpp alloc] initFromCpp:cppInstance];
    )
}

+ (PKStringArray*)createStringArrayFromCpp: (std::vector<std::string>&)cppInstance {
    PKMN_CPP_TO_OBJC(
        return (PKStringArray*)[[PKStringArrayFromCpp alloc] initFromCpp:cppInstance];
    )
}

+ (PKStringNumberDictionary*)createStringNumberDictionaryFromCpp: (std::map<std::string, int>&)cppInstance {
    PKMN_CPP_TO_OBJC(
        return (PKStringNumberDictionary*)[[PKStringNumberDictionaryFromCpp alloc] initFromCpp:cppInstance];
    )
}

+ (PKStringPair*)createStringPairFromCpp: (const std::pair<std::string, std::string>&)cppInstance {
    return [[PKStringPair alloc] initWithFirst:[NSString stringWithUTF8String:cppInstance.first.c_str()]
                                 andSecond:[NSString stringWithUTF8String:cppInstance.second.c_str()]];
}

@end

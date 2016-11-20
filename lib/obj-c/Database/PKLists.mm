/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#import <PKMN-ObjC/Database/PKLists.h>

#import "CppToObjC.h"

#include <pkmn/database/lists.hpp>

@implementation PKLists

+ (PKStringArray*)getAbilityList:(NSNumber*)generation {
    PKMN_CPP_TO_OBJC(
        std::vector<std::string> abilities = pkmn::database::get_ability_list(
                                                 [generation intValue]
                                             );

        return [CppToObjC createStringArrayFromCpp:abilities];
    )
}

+ (PKStringArray*)getGameList:(NSNumber*)generation includePrevious:(BOOL)includePrevious {
    PKMN_CPP_TO_OBJC(
        std::vector<std::string> games = pkmn::database::get_game_list(
                                             [generation intValue], includePrevious
                                         );

        return [CppToObjC createStringArrayFromCpp:games];
    )
}

+ (PKStringArray*)getItemList:(NSString*)game {
    PKMN_CPP_TO_OBJC(
        std::vector<std::string> items = pkmn::database::get_item_list(
                                             [game UTF8String]
                                         );

        return [CppToObjC createStringArrayFromCpp:items];
    )
}

+ (PKStringArray*)getLocationList:(NSString*)game wholeGeneration:(BOOL)wholeGeneration {
    PKMN_CPP_TO_OBJC(
        std::vector<std::string> locations = pkmn::database::get_location_list(
                                                 [game UTF8String], wholeGeneration
                                             );

        return [CppToObjC createStringArrayFromCpp:locations];
    )
}

+ (PKStringArray*)getMoveList:(NSString*)game {
    PKMN_CPP_TO_OBJC(
        std::vector<std::string> moves = pkmn::database::get_move_list(
                                             [game UTF8String]
                                         );

        return [CppToObjC createStringArrayFromCpp:moves];
    )
}

+ (PKStringArray*)getNatureList {
    PKMN_CPP_TO_OBJC(
        std::vector<std::string> natures = pkmn::database::get_nature_list();

        return [CppToObjC createStringArrayFromCpp:natures];
    )
}

+ (PKStringArray*)getPokemonList:(NSNumber*)generation includePrevious:(BOOL)includePrevious {
    PKMN_CPP_TO_OBJC(
        std::vector<std::string> pokemon = pkmn::database::get_pokemon_list(
                                               [generation intValue], includePrevious
                                           );

        return [CppToObjC createStringArrayFromCpp:pokemon];
    )
}

+ (PKStringArray*)getRegionList {
    PKMN_CPP_TO_OBJC(
        std::vector<std::string> regions = pkmn::database::get_region_list();

        return [CppToObjC createStringArrayFromCpp:regions];
    )
}

+ (PKStringArray*)getRibbonList:(NSNumber*)generation {
    PKMN_CPP_TO_OBJC(
        std::vector<std::string> ribbons = pkmn::database::get_ribbon_list(
                                               [generation intValue]
                                           );

        return [CppToObjC createStringArrayFromCpp:ribbons];
    )
}

+ (PKStringArray*)getSuperTrainingMedalList {
    PKMN_CPP_TO_OBJC(
        std::vector<std::string> super_training_medals = pkmn::database::get_super_training_medal_list();

        return [CppToObjC createStringArrayFromCpp:super_training_medals];
    )
}

+ (PKStringArray*)getTypeList:(NSString*)game {
    PKMN_CPP_TO_OBJC(
        std::vector<std::string> types = pkmn::database::get_type_list(
                                             [game UTF8String]
                                         );

        return [CppToObjC createStringArrayFromCpp:types];
    )
}

@end

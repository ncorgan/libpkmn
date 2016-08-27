/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#import "CppToObjC.h"

#import <PKMN-ObjC/Utils/PKPaths.h>

#include <pkmn/utils/paths.hpp>

@implementation PKPaths

+ (NSString*)getAppdataDir {
    PKMN_CPP_TO_OBJC(
        return [NSString stringWithUTF8String:pkmn::get_appdata_dir().c_str()];
    )
}

+ (NSString*)getDatabasePath {
    PKMN_CPP_TO_OBJC(
        return [NSString stringWithUTF8String:pkmn::get_database_path().c_str()];
    )
}

+ (NSString*)getImagesDir {
    PKMN_CPP_TO_OBJC(
        return [NSString stringWithUTF8String:pkmn::get_images_dir().c_str()];
    )
}

+ (NSString*)getTmpDir {
    PKMN_CPP_TO_OBJC(
        return [NSString stringWithUTF8String:pkmn::get_tmp_dir().c_str()];
    )
}

@end

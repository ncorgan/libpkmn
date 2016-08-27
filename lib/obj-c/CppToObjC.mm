/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#import "CppToObjC.h"

@implementation CppToObjC

+ (PKStringArray*)createStringArrayFromCpp: (const std::vector<std::string>&)cppInstance {
    PKMN_CPP_TO_OBJC(
        PKStringArrayFromCpp* fromCpp = [[PKStringArrayFromCpp alloc] initFromCpp:cppInstance];
        return (PKStringArray*)fromCpp;
    )
}

@end

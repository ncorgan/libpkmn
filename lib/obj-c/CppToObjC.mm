/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#import "CppToObjC.h"

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
    PKMN_CPP_TO_OBJC(
        [super dealloc];
    )
}

@end

@implementation CppToObjC

+ (PKStringArray*)createStringArrayFromCpp: (std::vector<std::string>&)cppInstance {
    PKMN_CPP_TO_OBJC(
        PKStringArrayFromCpp* fromCpp = [[PKStringArrayFromCpp alloc] initFromCpp:cppInstance];
        return (PKStringArray*)fromCpp;
    )
}

@end

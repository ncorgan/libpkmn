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
    [super dealloc];
}

@end

@implementation PKStringNumberDictionaryFromCpp

- (PKStringNumberDictionary*)initFromCpp: (std::map<std::string, int>&)cppInstance {
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

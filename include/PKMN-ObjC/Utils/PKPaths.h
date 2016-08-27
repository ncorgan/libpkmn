/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_OBJC_UTILS_PKPATHS_H
#define INCLUDED_PKMN_OBJC_UTILS_PKPATHS_H

#import <Foundation/NSString.h>

@interface PKPaths: NSObject

+ (NSString*)getAppdataDir;

+ (NSString*)getDatabasePath;

+ (NSString*)getImagesDir;

+ (NSString*)getTmpDir;

@end

#endif /* INCLUDED_PKMN_OBJC_UTILS_PKPATHS_H */

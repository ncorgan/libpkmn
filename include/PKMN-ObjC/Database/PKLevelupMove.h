/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_OBJC_DATABASE_PKLEVELUPMOVE_H
#define INCLUDED_PKMN_OBJC_DATABASE_PKLEVELUPMOVE_H

#import <PKMN-ObjC/Database/PKMoveDatabaseEntry.h>

#import <Foundation/Foundation.h>

@interface PKLevelupMove: NSObject {
    @public
    PKMoveDatabaseEntry* move;
    NSNumber* level;
}

- (PKLevelupMove*)init;

- (PKLevelupMove*)initWithMove:(PKMoveDatabaseEntry*)lmove
                      andLevel:(NSNumber*)moveLevel;

- (void)dealloc;

- (BOOL)isEqual:(id)object;

- (BOOL)isEqualToLevelupMove:(PKLevelupMove*)other;

@end

#endif /* INCLUDED_PKMN_OBJC_DATABASE_PKLEVELUPMOVE_H */

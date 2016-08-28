/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_OBJC_DATABASE_PKLEVELUPMOVEARRAY_H
#define INCLUDED_PKMN_OBJC_DATABASE_PKLEVELUPMOVEARRAY_H

#import <PKMN-ObjC/Database/PKLevelupMove.h>

#import <Foundation/Foundation.h>

@interface PKLevelupMoveArray : NSObject {
    @protected
    void* _internal;
}

- (PKLevelupMoveArray*)init;

- (PKLevelupMoveArray*)initWithSize:(NSNumber*)size;

- (void)dealloc;

// Allow getting with []
- (id)objectAtIndexedSubscript:(NSNumber*)idx;

// Allow setting with []
- (void)setObject:(id)obj atIndexedSubscript:(NSNumber*)idx;

- (NSNumber*)count;

- (void)insertLevelupMove:(PKLevelupMove*)levelupMove atIndex:(NSNumber*)idx;

- (void)removeLevelupMoveAtIndex:(NSNumber*)idx;

- (void)replaceLevelupMoveAtIndex:(NSNumber*)idx withLevelupMove:(PKLevelupMove*)levelupMove;

- (void)addLevelupMove:(PKLevelupMove*)levelupMove;

- (void)removeAllEntries;

- (void)removeLastLevelupMove;

- (BOOL)isEqual:(id)object;

- (BOOL)isEqualToArray:(PKLevelupMoveArray*)other;

@end

#endif /* INCLUDED_PKMN_OBJC_DATABASE_PKLEVELUPMOVEARRAY_H */

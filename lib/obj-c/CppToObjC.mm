/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#import "CppToObjC.h"

/** From the CodeSourcery ABI Spec, with C++ pointers turned to void*, and
 * other parts abridged. */

typedef enum
{
  _URC_FOREIGN_EXCEPTION_CAUGHT = 1
} _Unwind_Reason_Code;

struct _Unwind_Exception;

typedef void (*_Unwind_Exception_Cleanup_Fn) (_Unwind_Reason_Code,
                                              struct _Unwind_Exception *);
struct _Unwind_Exception
{
  uint64_t exception_class;
  _Unwind_Exception_Cleanup_Fn exception_cleanup;
  unsigned long private_1;
  unsigned long private_2;
} __attribute__((__aligned__));

_Unwind_Reason_Code _Unwind_Resume_or_Rethrow(struct _Unwind_Exception *);


struct __cxa_exception
{
  void *exceptionType;
  void (*exceptionDestructor) (void *);
  void (*unexpectedHandler) (void *);
  void (*terminateHandler) (void *);
  void *nextException;

  int                   handlerCount;
  int                   handlerSwitchValue;
  const char *          actionRecord;
  const char *          languageSpecificData;
  void *                        catchTemp;
  void *                        adjustedPtr;
  struct _Unwind_Exception      unwindHeader;
};

@implementation PKItemDatabaseEntryFromCpp

- (PKItemDatabaseEntryFromCpp*)initFromCpp: (const pkmn::database::item_entry&)cppInstance {
    PKMN_CPP_TO_OBJC(
        _internal = reinterpret_cast<void*>(
                        new pkmn::database::item_entry(cppInstance)
                    );
        return self;
    )
}

- (void)dealloc {
    [super dealloc];
}

@end

@implementation PKItemSlotArrayFromCpp: PKItemSlotArray

- (PKItemSlotArrayFromCpp*)initFromCpp: (pkmn::item_slots_t&)cppInstance {
    PKMN_CPP_TO_OBJC(
        _internal = reinterpret_cast<void*>(
                        new pkmn::item_slots_t(
                                (pkmn::item_slots_t&&)cppInstance
                            )
                    );
    )
}

- (void)dealloc {
    [super dealloc];
}

@end

@implementation PKLevelupMoveArrayFromCpp: PKLevelupMoveArray

- (PKLevelupMoveArrayFromCpp*)initFromCpp: (pkmn::database::levelup_moves_t&)cppInstance {
    PKMN_CPP_TO_OBJC(
        _internal = reinterpret_cast<void*>(
                        new pkmn::database::levelup_moves_t(
                                (pkmn::database::levelup_moves_t&&)cppInstance
                            )
                    );
        return self;
    )
}

- (void)dealloc {
    [super dealloc];
}

@end

@implementation PKMoveDatabaseEntryFromCpp

- (PKMoveDatabaseEntryFromCpp*)initFromCpp: (const pkmn::database::move_entry&)cppInstance {
    PKMN_CPP_TO_OBJC(
        _internal = reinterpret_cast<void*>(
                        new pkmn::database::move_entry(cppInstance)
                    );
        return self;
    )
}

- (void)dealloc {
    [super dealloc];
}

@end

@implementation PKMoveDatabaseEntryArrayFromCpp

- (PKMoveDatabaseEntryArrayFromCpp*)initFromCpp: (pkmn::database::move_list_t&)cppInstance {
    PKMN_CPP_TO_OBJC(
        _internal = reinterpret_cast<void*>(
                        new pkmn::database::move_list_t(
                                (pkmn::database::move_list_t&&)cppInstance
                            )
                    );
        return self;
    )
}

- (void)dealloc {
    [super dealloc];
}

@end

@implementation PKPokemonDatabaseEntryFromCpp

- (PKPokemonDatabaseEntryFromCpp*)initFromCpp: (const pkmn::database::pokemon_entry&)cppInstance {
    PKMN_CPP_TO_OBJC(
        _internal = reinterpret_cast<void*>(
                        new pkmn::database::pokemon_entry(cppInstance)
                    );
        return self;
    )
}

- (void)dealloc {
    [super dealloc];
}

@end

@implementation PKPokemonDatabaseEntryArrayFromCpp

- (PKPokemonDatabaseEntryArrayFromCpp*)initFromCpp: (pkmn::database::pokemon_entries_t&)cppInstance {
    PKMN_CPP_TO_OBJC(
        _internal = reinterpret_cast<void*>(
                        new pkmn::database::pokemon_entries_t(
                                (pkmn::database::pokemon_entries_t&&)cppInstance
                            )
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
                        new std::vector<std::string>(
                                (std::vector<std::string>&&)cppInstance
                            )
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
                        new std::map<std::string, int>(
                                (std::map<std::string, int>&&)cppInstance
                            )
                    );
        return self;
    )
}

- (void)dealloc {
    [super dealloc];
}

@end

@implementation CppToObjC

+ (std::string)getMessageFromCXXException: (CXXException*)cxxException
{
    // return (uint8_t*)(__bridge void*)object + ivar_getOffset(ivar);

    id object = (id)cxxException;
    Ivar ivar = class_getInstanceVariable(object_getClass(object), "ex");
    void* ivar_ptr = (uint8_t*)(__bridge void*)object + ivar_getOffset(ivar);

    struct _Unwind_Exception* unwind_exception_ptr = *((struct _Unwind_Exception**)ivar_ptr);
    char* ptr = (char*)unwind_exception_ptr;
    ptr -= __builtin_offsetof(struct __cxa_exception, unwindHeader);

    struct __cxa_exception* cxa_exception_ptr = (struct __cxa_exception*)ptr;
    void* thrown_object_ptr = cxa_exception_ptr + 1;

    return static_cast<const std::exception*>(thrown_object_ptr)->what();
}

+ (PKItemDatabaseEntry*)createItemDatabaseEntryFromCpp: (const pkmn::database::item_entry&)cppInstance {
    PKMN_CPP_TO_OBJC(
        return (PKItemDatabaseEntry*)[[PKItemDatabaseEntryFromCpp alloc] initFromCpp:cppInstance];
    )
}

+ (PKItemSlot*)createItemSlotFromCpp: (const pkmn::item_slot&)cppInstance {
    PKMN_CPP_TO_OBJC(
        PKItemSlot* ret = [[PKItemSlot alloc] init];
        ret->item = [NSString stringWithUTF8String:cppInstance.item.c_str()];
        ret->amount = @(cppInstance.amount);

        return ret;
    )
}

+ (PKItemSlotArray*)createItemSlotArrayFromCpp: (pkmn::item_slots_t&)cppInstance {
    PKMN_CPP_TO_OBJC(
        return (PKItemSlotArray*)[[PKItemSlotArrayFromCpp alloc] initFromCpp:cppInstance];
    )
}

+ (PKLevelupMove*)createLevelupMoveFromCpp: (const pkmn::database::levelup_move&)cppInstance {
    PKMN_CPP_TO_OBJC(
        PKLevelupMove* ret = [[PKLevelupMove alloc] init];
        ret->move = [CppToObjC createMoveDatabaseEntryFromCpp:cppInstance.move];
        ret->level = @(cppInstance.level);

        return ret;
    )
}

+ (PKLevelupMoveArray*)createLevelupMoveArrayFromCpp: (pkmn::database::levelup_moves_t&)cppInstance {
    PKMN_CPP_TO_OBJC(
        return (PKLevelupMoveArray*)[[PKLevelupMoveArrayFromCpp alloc] initFromCpp:cppInstance];
    )
}

+ (PKMoveDatabaseEntry*)createMoveDatabaseEntryFromCpp: (const pkmn::database::move_entry&)cppInstance {
    PKMN_CPP_TO_OBJC(
        return (PKMoveDatabaseEntry*)[[PKMoveDatabaseEntryFromCpp alloc] initFromCpp:cppInstance];
    )
}

+ (PKMoveDatabaseEntryArray*)createMoveDatabaseEntryArrayFromCpp: (pkmn::database::move_list_t&)cppInstance {
    PKMN_CPP_TO_OBJC(
        return (PKMoveDatabaseEntryArray*)[[PKMoveDatabaseEntryArrayFromCpp alloc] initFromCpp:cppInstance];
    )
}

+ (PKPokemonDatabaseEntry*)createPokemonDatabaseEntryFromCpp: (const pkmn::database::pokemon_entry&)cppInstance {
    PKMN_CPP_TO_OBJC(
        return (PKPokemonDatabaseEntry*)[[PKPokemonDatabaseEntryFromCpp alloc] initFromCpp:cppInstance];
    )
}

+ (PKPokemonDatabaseEntryArray*)createPokemonDatabaseEntryArrayFromCpp: (pkmn::database::pokemon_entries_t&)cppInstance {
    PKMN_CPP_TO_OBJC(
        return (PKPokemonDatabaseEntryArray*)[[PKPokemonDatabaseEntryArrayFromCpp alloc] initFromCpp:cppInstance];
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

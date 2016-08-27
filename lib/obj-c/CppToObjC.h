/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_OBJC_CPPTOOBJC_H
#define PKMN_OBJC_CPPTOOBJC_H

#import <PKMN-ObjC/Types/PKStringArray.h>

#import <Foundation/Foundation.h>

#include <stdexcept>
#include <vector>

#define PKMN_CPP_TO_OBJC(...) \
{ \
    NSString* errMsg; \
    NSString* objCErrName; \
    bool thrown = false; \
    @try { \
        try { __VA_ARGS__ } \
        catch(const std::out_of_range &e) { \
            errMsg = [NSString stringWithUTF8String:e.what()]; \
            objCErrName = @"NSRangeException"; \
            thrown = true; \
        } catch(const std::logic_error &e) { \
            errMsg = [NSString stringWithUTF8String:e.what()]; \
            objCErrName = @"NSInvalidArgumentException"; \
            thrown = true; \
        } catch(const std::runtime_error &e) { \
            errMsg = [NSString stringWithUTF8String:e.what()]; \
            objCErrName = @"NSGenericException"; \
            thrown = true; \
        } catch(const std::exception &e) { \
            errMsg = [NSString stringWithUTF8String:e.what()]; \
            objCErrName = @"NSGenericException"; \
            thrown = true; \
        } catch(...) { \
            errMsg = @"Unknown error."; \
            objCErrName = @"NSGenericException"; \
            thrown = true; \
        } \
        if(thrown) { \
            @throw [NSException \
                        exceptionWithName:objCErrName \
                        reason:errMsg \
                        userInfo:nil \
                   ]; \
        } \
    } \
    @catch(NSException* e) { \
        @throw; \
    } \
    @finally { \
    } \
}

@interface PKStringArrayFromCpp: PKStringArray

- (PKStringArray*)initFromCpp: (const std::vector<std::string>&)cppInstance;

- (void)dealloc;

@end

@interface CppToObjC: NSObject

+ (PKStringArray*)createStringArrayFromCpp: (const std::vector<std::string>&)cppInstance;

@end

#endif /* PKMN_OBJC_CPPTOOBJC_H */

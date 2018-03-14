/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMNTEST_C_CONFIG_H
#define PKMNTEST_C_CONFIG_H

/***************************************************************************
 * Define cross-platform macros
 ***************************************************************************/
#if defined(_MSC_VER)
#    define PKMNTEST_C_EXPORT         __declspec(dllexport)
#    define PKMNTEST_C_IMPORT         __declspec(dllimport)
#    define PKMNTEST_C_INLINE         __forceinline
#    define PKMNTEST_C_UNUSED(x)      x
#elif defined(__GNUC__) && __GNUC__ >= 4
#    define PKMNTEST_C_EXPORT         __attribute__((visibility("default")))
#    define PKMNTEST_C_IMPORT         __attribute__((visibility("default")))
#    define PKMNTEST_C_INLINE         inline __attribute__((always_inline))
#    define PKMNTEST_C_UNUSED(x)      x __attribute__((unused))
#else
#    define PKMNTEST_C_EXPORT
#    define PKMNTEST_C_IMPORT
#    define PKMNTEST_C_INLINE         inline
#    define PKMNTEST_C_UNUSED(x)      x
#endif

#ifdef PKMNTEST_C_DLL_EXPORTS
#    define PKMNTEST_C_API PKMNTEST_C_EXPORT
#else
#    define PKMNTEST_C_API PKMNTEST_C_IMPORT
#endif

#if defined(linux) || defined(__linux) || defined(__linux__)
#    define PKMNTEST_C_PLATFORM_LINUX
#elif defined(__MINGW32__) || defined(MINGW32)
#    define PKMNTEST_C_PLATFORM_MINGW
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#    define PKMNTEST_C_PLATFORM_WIN32
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#    define PKMNTEST_C_PLATFORM_MACOS
#endif

#endif /* PKMNTEST_C_CONFIG_H */

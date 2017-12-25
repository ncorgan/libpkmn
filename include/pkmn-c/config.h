/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_CONFIG_H
#define PKMN_C_CONFIG_H

/***************************************************************************
 * Define cross-platform macros
 ***************************************************************************/
#if defined(_MSC_VER)
#    define PKMN_C_EXPORT         __declspec(dllexport)
#    define PKMN_C_IMPORT         __declspec(dllimport)
#    define PKMN_C_INLINE         __forceinline
#    define PKMN_C_UNUSED(x)      x
#elif defined(__GNUC__) && __GNUC__ >= 4
#    define PKMN_C_EXPORT         __attribute__((visibility("default")))
#    define PKMN_C_IMPORT         __attribute__((visibility("default")))
#    define PKMN_C_INLINE         inline __attribute__((always_inline))
#    define PKMN_C_UNUSED(x)      x __attribute__((unused))
#else
#    define PKMN_C_EXPORT
#    define PKMN_C_IMPORT
#    define PKMN_C_INLINE         inline
#    define PKMN_C_UNUSED(x)      x
#endif

#ifdef PKMN_C_DLL_EXPORTS
#    define PKMN_C_API PKMN_C_EXPORT
#else
#    define PKMN_C_API PKMN_C_IMPORT
#endif

#if defined(linux) || defined(__linux) || defined(__linux__)
#    define PKMN_C_PLATFORM_LINUX
#elif defined(__MINGW32__) || defined(MINGW32)
#    define PKMN_C_PLATFORM_MINGW
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#    define PKMN_C_PLATFORM_WIN32
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#    define PKMN_C_PLATFORM_MACOS
#endif

#endif /* PKMN_C_CONFIG_H */

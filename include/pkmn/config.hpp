/*
 * Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CONFIG_HPP
#define PKMN_CONFIG_HPP

/***************************************************************************
 * Allow use of and/or/not
 ***************************************************************************/
#include <ciso646>

/***************************************************************************
 * Define cross-platform macros
 ***************************************************************************/
#if defined(_MSC_VER)
#    define PKSAV_EXPORT         __declspec(dllexport)
#    define PKSAV_IMPORT         __declspec(dllimport)
#    define PKSAV_INLINE         __forceinline
#    define PKSAV_UNUSED(x)      x
#    pragma warning(disable: 4251) // class 'A<T>' needs to have dll-interface to be used by clients of class 'B'
#    pragma warning(disable: 4800) // forcing value to bool 'true' or 'false'
#elif defined(__GNUG__) && __GNUG__ >= 4
#    define PKSAV_EXPORT         __attribute__((visibility("default")))
#    define PKSAV_IMPORT         __attribute__((visibility("default")))
#    define PKSAV_INLINE         inline __attribute__((always_inline))
#    define PKSAV_UNUSED(x)      x __attribute__((unused))
#else
#    define PKSAV_EXPORT
#    define PKSAV_IMPORT
#    define PKSAV_INLINE         inline
#    define PKSAV_UNUSED(x)      x
#endif

#ifdef PKSAV_DLL_EXPORTS
#    define PKSAV_API PKSAV_EXPORT
#else
#    define PKSAV_API PKSAV_IMPORT
#endif

#if defined(linux) || defined(__linux) || defined(__linux__)
#    define PKSAV_PLATFORM_LINUX
#elif defined(__MINGW32__) || defined(MINGW32)
#    define PKSAV_PLATFORM_MINGW
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#    define PKSAV_PLATFORM_WIN32
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#    define PKSAV_PLATFORM_MACOS
#endif

#endif /* PKMN_CONFIG_HPP */

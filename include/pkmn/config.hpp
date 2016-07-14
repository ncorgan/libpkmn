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
#    define PKMN_EXPORT         __declspec(dllexport)
#    define PKMN_IMPORT         __declspec(dllimport)
#    define PKMN_INLINE         __forceinline
#    define PKMN_UNUSED(x)      x
#    pragma warning(disable: 4251) // class 'A<T>' needs to have dll-interface to be used by clients of class 'B'
#    pragma warning(disable: 4800) // forcing value to bool 'true' or 'false'
#elif defined(__GNUG__) && __GNUG__ >= 4
#    define PKMN_EXPORT         __attribute__((visibility("default")))
#    define PKMN_IMPORT         __attribute__((visibility("default")))
#    define PKMN_INLINE         inline __attribute__((always_inline))
#    define PKMN_UNUSED(x)      x __attribute__((unused))
#else
#    define PKMN_EXPORT
#    define PKMN_IMPORT
#    define PKMN_INLINE         inline
#    define PKMN_UNUSED(x)      x
#endif

#ifdef PKMN_DLL_EXPORTS
#    define PKMN_API PKMN_EXPORT
#else
#    define PKMN_API PKMN_IMPORT
#endif

#if defined(linux) || defined(__linux) || defined(__linux__)
#    define PKMN_PLATFORM_LINUX
#elif defined(__MINGW32__) || defined(MINGW32)
#    define PKMN_PLATFORM_MINGW
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#    define PKMN_PLATFORM_WIN32
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#    define PKMN_PLATFORM_MACOS
#endif

#endif /* PKMN_CONFIG_HPP */

/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMN_ENV_HPP
#define INCLUDED_PKMN_ENV_HPP

#include <pkmn/config.hpp>
#include <cstdio>
#include <cstdlib>
#include <string>

#ifdef PKMN_PLATFORM_MINGW
static char putenv_buffer[1024];
#endif

/*
 * Thin OS-agnostic wrapper for working with environment variables
 */

inline std::string pkmn_getenv(
    const std::string& key
) {
    char* val = getenv(key.c_str());
    return (val) ? val : "";
}

inline void pkmn_setenv(
    const std::string& key,
    const std::string& val
) {
    #if defined(PKMN_PLATFORM_WIN32)
        _putenv_s(key.c_str(), val.c_str());
    #elif defined(PKMN_PLATFORM_MINGW)
        snprintf(
            putenv_buffer,
            sizeof(putenv_buffer),
            "%s=%s",
            key.c_str(),
            val.c_str()
        );
        putenv(putenv_buffer);
    #else
        setenv(key.c_str(), val.c_str(), 1);
    #endif
}

inline void pkmn_unsetenv(
    const std::string& key
) {
    #if defined(PKMN_PLATFORM_WIN32)
        _putenv_s(key.c_str(), "");
    #elif defined(PKMN_PLATFORM_MINGW)
        snprintf(
            putenv_buffer,
            sizeof(putenv_buffer),
            "%s=",
            key.c_str()
        );
        putenv(putenv_buffer);
    #else
        unsetenv(key.c_str());
    #endif
}

#endif /* INCLUDED_PKMN_ENV_HPP */

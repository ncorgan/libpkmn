/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifdef _MSC_VER
#    pragma warning(disable: 4273) // Inconsistent DLL linkage
#endif

#include "error_internal.hpp"

#include <pkmn-c/error.h>

#include <boost/thread/mutex.hpp>

#include <string>

struct error_struct {
    std::string error;
    boost::mutex error_mutex;
};

static struct error_struct global_error;

void pkmn_set_error(
    const std::string &error
) {
    boost::mutex::scoped_lock lock(global_error.error_mutex);
    global_error.error = error;
}

const char* pkmn_strerror() {
    boost::mutex::scoped_lock lock(global_error.error_mutex);
    return global_error.error.c_str();
}

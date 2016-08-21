/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "error_internal.hpp"

#include <pkmn-c/error.h>

#include <boost/thread/mutex.hpp>

#include <string>

struct error_struct {
    std::string error;
};

static struct error_struct pkmn_c_error;

static boost::mutex pkmn_c_error_mutex;

void pkmn_set_error(
    const std::string &error
) {
    boost::mutex::scoped_lock lock(pkmn_c_error_mutex);
    pkmn_c_error.error = error;
}

const char* pkmn_strerror() {
    boost::mutex::scoped_lock lock(pkmn_c_error_mutex);
    return pkmn_c_error.error.c_str();
}

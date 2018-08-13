/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
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

namespace pkmn { namespace c {

struct error_struct
{
    std::string error;
    boost::mutex error_mutex;

    error_struct():
        error("None")
    {}
};

static struct error_struct global_error;

void pkmn_set_error(const std::string& error)
{
    boost::mutex::scoped_lock lock(global_error.error_mutex);

    global_error.error = error;
}

enum pkmn_error check_for_null_param(
    const void* p_param,
    const std::string& param_name
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    if(p_param == nullptr)
    {
        std::string error_message("Null pointer passed into parameter \"");
        error_message.append(param_name);
        error_message.append("\".");

        pkmn_set_error(error_message);

        error = PKMN_ERROR_NULL_POINTER;
    }

    return error;
}

}}

const char* pkmn_strerror()
{
    boost::mutex::scoped_lock lock(pkmn::c::global_error.error_mutex);

    return pkmn::c::global_error.error.c_str();
}

/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_C_ERROR_INTERNAL_HPP
#define PKMN_C_ERROR_INTERNAL_HPP

#include "cpp_to_c.hpp"

#include <pkmn-c/error.h>

#include <pkmn/exception.hpp>

#include <boost/assert.hpp>
#include <boost/format.hpp>

#include <boost/thread/mutex.hpp>

#include <cstring>
#include <stdexcept>
#include <string>

namespace pkmn { namespace c {

void pkmn_set_error(const std::string& error);

template <typename functor>
static enum pkmn_error handle_exceptions(const functor& func)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    try
    {
        func();
        pkmn_set_error("None");
    }
    catch(const pkmn::unimplemented_error& e)
    {
        pkmn_set_error(e.what());
        error = PKMN_ERROR_UNIMPLEMENTED_ERROR;
    }
    catch(const pkmn::feature_not_in_game_error& e)
    {
        pkmn_set_error(e.what());
        error = PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR;
    }
    catch(const pkmn::feature_not_in_build_error& e)
    {
        pkmn_set_error(e.what());
        error = PKMN_ERROR_FEATURE_NOT_IN_BUILD_ERROR;
    }
    catch(const std::invalid_argument& e)
    {
        pkmn_set_error(e.what());
        error = PKMN_ERROR_INVALID_ARGUMENT;
    }
    catch(const std::domain_error& e)
    {
        pkmn_set_error(e.what());
        error = PKMN_ERROR_DOMAIN_ERROR;
    }
    catch(const std::length_error& e)
    {
        pkmn_set_error(e.what());
        error = PKMN_ERROR_LENGTH_ERROR;
    }
    catch(const std::out_of_range& e)
    {
        pkmn_set_error(e.what());
        error = PKMN_ERROR_OUT_OF_RANGE;
    }
    catch(const std::logic_error& e)
    {
        pkmn_set_error(e.what());
        error = PKMN_ERROR_LOGIC_ERROR;
    }
    catch(const std::range_error& e)
    {
        pkmn_set_error(e.what());
        error = PKMN_ERROR_RANGE_ERROR;
    }
    catch(const std::overflow_error& e)
    {
        pkmn_set_error(e.what());
        error = PKMN_ERROR_OVERFLOW_ERROR;
    }
    catch(const std::underflow_error& e)
    {
        pkmn_set_error(e.what());
        error = PKMN_ERROR_UNDERFLOW_ERROR;
    }
    catch(const std::runtime_error& e)
    {
        pkmn_set_error(e.what());
        if(strstr(e.what(), "Internal error") != nullptr)
        {
            error = PKMN_ERROR_INTERNAL_ERROR;
        }
        else
        {
            error = PKMN_ERROR_RUNTIME_ERROR;
        }
    }
    catch(const std::exception& e)
    {
        pkmn_set_error(e.what());
        error = PKMN_ERROR_STD_EXCEPTION;
    }
    catch(...)
    {
        pkmn_set_error("Unknown error");
        error = PKMN_ERROR_UNKNOWN_ERROR;
    }

    return error;
}

template <typename functor, typename libpkmn_type>
static enum pkmn_error handle_exceptions(
    const functor& func,
    internal_t<libpkmn_type>* p_libpkmn_c_struct
)
{
    BOOST_ASSERT(p_libpkmn_c_struct != nullptr);

    boost::mutex::scoped_lock lock(p_libpkmn_c_struct->error_mutex);

    enum pkmn_error error = handle_exceptions(func);
    p_libpkmn_c_struct->last_error.assign(pkmn_strerror());

    return error;
}

enum pkmn_error check_for_null_param(
    const void* p_param,
    const std::string& param_name
);

template <typename libpkmn_type>
static enum pkmn_error check_for_null_param(
    const void* p_param,
    const std::string& param_name,
    internal_t<libpkmn_type>* p_libpkmn_c_struct
)
{
    BOOST_ASSERT(p_libpkmn_c_struct != nullptr);

    boost::mutex::scoped_lock lock(p_libpkmn_c_struct->error_mutex);

    enum pkmn_error error = check_for_null_param(p_param, param_name);
    p_libpkmn_c_struct->last_error.assign(pkmn_strerror());

    return error;
}

template <typename libpkmn_param_type>
static enum pkmn_error check_for_null_wrapper_param(
    const libpkmn_param_type* p_param,
    const std::string& param_name
)
{
    enum pkmn_error error = check_for_null_param(p_param, param_name);
    if(!error)
    {
        if(!p_param->p_internal)
        {
            std::string error_message("The parameter \"");
            error_message.append(param_name);
            error_message.append("\" has not been initialized.");

            pkmn_set_error(error_message);
            error = PKMN_ERROR_NULL_POINTER;
        }
    }

    return error;
}

template <typename libpkmn_param_type, typename libpkmn_output_type>
static enum pkmn_error check_for_null_wrapper_param(
    const libpkmn_param_type* p_param,
    const std::string& param_name,
    internal_t<libpkmn_output_type>* p_output_struct
)
{
    BOOST_ASSERT(p_output_struct != nullptr);

    boost::mutex::scoped_lock lock(p_output_struct->error_mutex);

    enum pkmn_error error = check_for_null_wrapper_param(p_param, param_name);
    p_output_struct->last_error.assign(pkmn_strerror());

    return error;
}

}}

#endif /* PKMN_C_ERROR_INTERNAL_HPP */

/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_C_ERROR_INTERNAL_HPP
#define PKMN_C_ERROR_INTERNAL_HPP

#include <pkmn-c/error.h>

#include <pkmn/exception.hpp>

#include <boost/format.hpp>

#include <cstring>
#include <stdexcept>
#include <string>

void pkmn_set_error(
    const std::string &error
);

#define PKMN_CHECK_NULL_WRAPPER_PARAM(param) \
{ \
    if(param) \
    { \
        if(!param->p_internal) \
        { \
            pkmn_set_error("The parameter \"" #param "\" has not been initialized."); \
            return PKMN_ERROR_NULL_POINTER; \
        } \
    } \
    else \
    { \
        pkmn_set_error("Null pointer passed into parameter \"" #param "\""); \
        return PKMN_ERROR_NULL_POINTER; \
    } \
}

#define PKMN_CHECK_NULL_PARAM(param) \
{ \
    if(!param) { \
        pkmn_set_error("Null pointer passed into parameter \"" #param "\""); \
        return PKMN_ERROR_NULL_POINTER; \
    } \
}

#define PKMN_CHECK_NULL_WRAPPER_PARAM_WITH_HANDLE(param, handle) \
{ \
    if(param) \
    { \
        if(!param->p_internal) \
        { \
            boost::mutex::scoped_lock lock(handle->error_mutex); \
            pkmn_set_error("The parameter \"" #param "\" has not been initialized."); \
            handle->last_error = "The parameter \"" #param "\" has not been initialized."; \
            return PKMN_ERROR_NULL_POINTER; \
        } \
    } \
    else \
    { \
        boost::mutex::scoped_lock lock(handle->error_mutex); \
        pkmn_set_error("Null pointer passed into parameter \"" #param "\""); \
        handle->last_error = "Null pointer passed into parameter \"" #param "\""; \
        return PKMN_ERROR_NULL_POINTER; \
    } \
}

#define PKMN_CHECK_NULL_PARAM_WITH_HANDLE(param, handle) \
{ \
    if(!param) { \
        boost::mutex::scoped_lock lock(handle->error_mutex); \
        pkmn_set_error("Null pointer passed into parameter \"" #param "\""); \
        handle->last_error = "Null pointer passed into parameter \"" #param "\""; \
        return PKMN_ERROR_NULL_POINTER; \
    } \
}

#define PKMN_CPP_TO_C(...) \
{ \
    enum pkmn_error error = PKMN_ERROR_NONE; \
    try \
    { \
        __VA_ARGS__ ; \
        pkmn_set_error("None"); \
        error = PKMN_ERROR_NONE; \
    } \
    catch(const pkmn::unimplemented_error &e) \
    { \
        pkmn_set_error(e.what()); \
        error = PKMN_ERROR_UNIMPLEMENTED_ERROR; \
    } \
    catch(const pkmn::feature_not_in_game_error &e) \
    { \
        pkmn_set_error(e.what()); \
        error = PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR; \
    } \
    catch(const pkmn::feature_not_in_build_error &e) \
    { \
        pkmn_set_error(e.what()); \
        error = PKMN_ERROR_FEATURE_NOT_IN_BUILD_ERROR; \
    } \
    catch(const std::invalid_argument &e) \
    { \
        pkmn_set_error(e.what()); \
        error = PKMN_ERROR_INVALID_ARGUMENT; \
    } \
    catch(const std::domain_error &e) \
    { \
        pkmn_set_error(e.what()); \
        error = PKMN_ERROR_DOMAIN_ERROR; \
    } \
    catch(const std::length_error &e) \
    { \
        pkmn_set_error(e.what()); \
        error = PKMN_ERROR_LENGTH_ERROR; \
    } catch(const std::out_of_range &e) \
    { \
        pkmn_set_error(e.what()); \
        error = PKMN_ERROR_OUT_OF_RANGE; \
    } \
    catch(const std::logic_error &e) \
    { \
        pkmn_set_error(e.what()); \
        error = PKMN_ERROR_LOGIC_ERROR; \
    } \
    catch(const std::range_error &e) \
    { \
        pkmn_set_error(e.what()); \
        error = PKMN_ERROR_RANGE_ERROR; \
    } \
    catch(const std::overflow_error &e) \
    { \
        pkmn_set_error(e.what()); \
        error = PKMN_ERROR_OVERFLOW_ERROR; \
    } \
    catch(const std::underflow_error &e) \
    { \
        pkmn_set_error(e.what()); \
        error = PKMN_ERROR_UNDERFLOW_ERROR; \
    } \
    catch(const std::runtime_error &e) \
    { \
        pkmn_set_error(e.what()); \
        if(strstr(e.what(), "Internal error") != nullptr) \
        { \
            error = PKMN_ERROR_INTERNAL_ERROR; \
        } \
        else \
        { \
            error = PKMN_ERROR_RUNTIME_ERROR; \
        } \
    } \
    catch(const std::exception &e) \
    { \
        pkmn_set_error(e.what()); \
        error = PKMN_ERROR_STD_EXCEPTION; \
    } \
    catch(...) \
    { \
        pkmn_set_error("Unknown error"); \
        error = PKMN_ERROR_UNKNOWN_ERROR; \
    } \
    return error; \
}

#define PKMN_CPP_TO_C_WITH_HANDLE(h,...) \
{ \
    enum pkmn_error error = PKMN_ERROR_NONE; \
    boost::mutex::scoped_lock lock(h->error_mutex); \
    try \
    { \
        __VA_ARGS__ ; \
        pkmn_set_error("None"); \
        h->last_error = "None"; \
        error = PKMN_ERROR_NONE; \
    } \
    catch(const pkmn::unimplemented_error &e) \
    { \
        pkmn_set_error(e.what()); \
        h->last_error = e.what(); \
        error = PKMN_ERROR_UNIMPLEMENTED_ERROR; \
    } \
    catch(const pkmn::feature_not_in_game_error &e) \
    { \
        pkmn_set_error(e.what()); \
        h->last_error = e.what(); \
        error = PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR; \
    } \
    catch(const pkmn::feature_not_in_build_error &e) \
    { \
        pkmn_set_error(e.what()); \
        h->last_error = e.what(); \
        error = PKMN_ERROR_FEATURE_NOT_IN_BUILD_ERROR; \
    } \
    catch(const std::invalid_argument &e) \
    { \
        pkmn_set_error(e.what()); \
        h->last_error = e.what(); \
        error = PKMN_ERROR_INVALID_ARGUMENT; \
    } \
    catch(const std::domain_error &e) \
    { \
        pkmn_set_error(e.what()); \
        h->last_error = e.what(); \
        error = PKMN_ERROR_DOMAIN_ERROR; \
    } \
    catch(const std::length_error &e) \
    { \
        pkmn_set_error(e.what()); \
        h->last_error = e.what(); \
        error = PKMN_ERROR_LENGTH_ERROR; \
    } \
    catch(const std::out_of_range &e) \
    { \
        pkmn_set_error(e.what()); \
        h->last_error = e.what(); \
        error = PKMN_ERROR_OUT_OF_RANGE; \
    } \
    catch(const std::logic_error &e) \
    { \
        pkmn_set_error(e.what()); \
        h->last_error = e.what(); \
        error = PKMN_ERROR_LOGIC_ERROR; \
    } \
    catch(const std::range_error &e) \
    { \
        pkmn_set_error(e.what()); \
        h->last_error = e.what(); \
        error = PKMN_ERROR_RANGE_ERROR; \
    } \
    catch(const std::overflow_error &e) \
    { \
        pkmn_set_error(e.what()); \
        h->last_error = e.what(); \
        error = PKMN_ERROR_OVERFLOW_ERROR; \
    } \
    catch(const std::underflow_error &e) \
    { \
        pkmn_set_error(e.what()); \
        h->last_error = e.what(); \
        error = PKMN_ERROR_UNDERFLOW_ERROR; \
    } \
    catch(const std::runtime_error &e) \
    { \
        pkmn_set_error(e.what()); \
        h->last_error = e.what(); \
        if(strstr(e.what(), "Internal error") != nullptr) \
        { \
            error = PKMN_ERROR_INTERNAL_ERROR; \
        } \
        else \
        { \
            error = PKMN_ERROR_RUNTIME_ERROR; \
        } \
    } \
    catch(const std::exception &e) \
    { \
        pkmn_set_error(e.what()); \
        h->last_error = e.what(); \
        error = PKMN_ERROR_STD_EXCEPTION; \
    } \
    catch(...) \
    { \
        pkmn_set_error("Unknown error"); \
        h->last_error = "Unknown error"; \
        error = PKMN_ERROR_UNKNOWN_ERROR; \
    } \
    return error; \
}

#endif /* PKMN_C_ERROR_INTERNAL_HPP */

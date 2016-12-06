/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_C_ERROR_INTERNAL_HPP
#define PKMN_C_ERROR_INTERNAL_HPP

#include <pkmn-c/error.h>

#include <pkmn/exception.hpp>

#include <stdexcept>
#include <string>

void pkmn_set_error(
    const std::string &error
);

#define PKMN_CPP_TO_C(...) \
{ \
    try { \
        __VA_ARGS__ ; \
        pkmn_set_error("None"); \
        return PKMN_ERROR_NONE; \
    } catch(const pkmn::pksav_error &e) { \
        pkmn_set_error(e.what()); \
        return PKMN_ERROR_PKSAV_ERROR; \
    } catch(const pkmn::sqlite_error &e) { \
        pkmn_set_error(e.what()); \
        return PKMN_ERROR_SQLITE_ERROR; \
    } catch(const pkmn::range_error &e) { \
        pkmn_set_error(e.what()); \
        return PKMN_ERROR_RANGE_ERROR; \
    } catch(const pkmn::unimplemented_error &e) { \
        pkmn_set_error(e.what()); \
        return PKMN_ERROR_UNIMPLEMENTED_ERROR; \
    } catch(const std::invalid_argument &e) { \
        pkmn_set_error(e.what()); \
        return PKMN_ERROR_INVALID_ARGUMENT; \
    } catch(const std::domain_error &e) { \
        pkmn_set_error(e.what()); \
        return PKMN_ERROR_DOMAIN_ERROR; \
    } catch(const std::length_error &e) { \
        pkmn_set_error(e.what()); \
        return PKMN_ERROR_LENGTH_ERROR; \
    } catch(const std::out_of_range &e) { \
        pkmn_set_error(e.what()); \
        return PKMN_ERROR_OUT_OF_RANGE; \
    } catch(const std::logic_error &e) { \
        pkmn_set_error(e.what()); \
        return PKMN_ERROR_LOGIC_ERROR; \
    } catch(const std::range_error &e) { \
        pkmn_set_error(e.what()); \
        return PKMN_ERROR_STD_RANGE_ERROR; \
    } catch(const std::overflow_error &e) { \
        pkmn_set_error(e.what()); \
        return PKMN_ERROR_OVERFLOW_ERROR; \
    } catch(const std::underflow_error &e) { \
        pkmn_set_error(e.what()); \
        return PKMN_ERROR_UNDERFLOW_ERROR; \
    } catch(const std::runtime_error &e) { \
        pkmn_set_error(e.what()); \
        return PKMN_ERROR_RUNTIME_ERROR; \
    } catch(const std::exception &e) { \
        pkmn_set_error(e.what()); \
        return PKMN_ERROR_STD_EXCEPTION; \
    } catch(...) { \
        pkmn_set_error("Unknown error"); \
        return PKMN_ERROR_UNKNOWN_ERROR; \
    } \
}

#define PKMN_CPP_TO_C_WITH_HANDLE(h,...) \
{ \
    try { \
        __VA_ARGS__ ; \
        pkmn_set_error("None"); \
        h->last_error = "None"; \
        return PKMN_ERROR_NONE; \
    } catch(const pkmn::pksav_error &e) { \
        pkmn_set_error(e.what()); \
        h->last_error = e.what(); \
        return PKMN_ERROR_PKSAV_ERROR; \
    } catch(const pkmn::sqlite_error &e) { \
        pkmn_set_error(e.what()); \
        h->last_error = e.what(); \
        return PKMN_ERROR_SQLITE_ERROR; \
    } catch(const pkmn::range_error &e) { \
        pkmn_set_error(e.what()); \
        h->last_error = e.what(); \
        return PKMN_ERROR_RANGE_ERROR; \
    } catch(const pkmn::unimplemented_error &e) { \
        pkmn_set_error(e.what()); \
        h->last_error = e.what(); \
        return PKMN_ERROR_UNIMPLEMENTED_ERROR; \
    } catch(const std::invalid_argument &e) { \
        pkmn_set_error(e.what()); \
        h->last_error = e.what(); \
        return PKMN_ERROR_INVALID_ARGUMENT; \
    } catch(const std::domain_error &e) { \
        pkmn_set_error(e.what()); \
        h->last_error = e.what(); \
        return PKMN_ERROR_DOMAIN_ERROR; \
    } catch(const std::length_error &e) { \
        pkmn_set_error(e.what()); \
        h->last_error = e.what(); \
        return PKMN_ERROR_LENGTH_ERROR; \
    } catch(const std::out_of_range &e) { \
        pkmn_set_error(e.what()); \
        h->last_error = e.what(); \
        return PKMN_ERROR_OUT_OF_RANGE; \
    } catch(const std::logic_error &e) { \
        pkmn_set_error(e.what()); \
        h->last_error = e.what(); \
        return PKMN_ERROR_LOGIC_ERROR; \
    } catch(const std::range_error &e) { \
        pkmn_set_error(e.what()); \
        h->last_error = e.what(); \
        return PKMN_ERROR_RANGE_ERROR; \
    } catch(const std::overflow_error &e) { \
        pkmn_set_error(e.what()); \
        h->last_error = e.what(); \
        return PKMN_ERROR_OVERFLOW_ERROR; \
    } catch(const std::underflow_error &e) { \
        pkmn_set_error(e.what()); \
        h->last_error = e.what(); \
        return PKMN_ERROR_UNDERFLOW_ERROR; \
    } catch(const std::runtime_error &e) { \
        pkmn_set_error(e.what()); \
        h->last_error = e.what(); \
        return PKMN_ERROR_RUNTIME_ERROR; \
    } catch(const std::exception &e) { \
        pkmn_set_error(e.what()); \
        h->last_error = e.what(); \
        return PKMN_ERROR_STD_EXCEPTION; \
    } catch(...) { \
        pkmn_set_error("Unknown error"); \
        h->last_error = "Unknown error"; \
        return PKMN_ERROR_UNKNOWN_ERROR; \
    } \
}

#endif /* PKMN_C_ERROR_INTERNAL_HPP */

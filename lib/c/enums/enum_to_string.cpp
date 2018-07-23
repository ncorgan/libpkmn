/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn/enums/enum_to_string.hpp>
#include <pkmn-c/enums/enum_to_string.h>

#define PKMN_C_ENUM_TO_STRING_FCNS(enum_name) \
\
enum pkmn_error pkmn_ ## enum_name ## _to_string( \
    enum pkmn_ ## enum_name enum_name , \
    char* p_ ## enum_name ## _name_out, \
    size_t enum_name ## _name_buffer_len, \
    size_t* p_ ## enum_name ## _name_len_out \
) \
{ \
    PKMN_CHECK_NULL_PARAM(p_ ## enum_name ## _name_out); \
\
    PKMN_CPP_TO_C( \
        pkmn::c::string_cpp_to_c( \
            pkmn:: enum_name ## _to_string(static_cast<pkmn::e_ ## enum_name >( enum_name )), \
            p_ ## enum_name ## _name_out, \
            enum_name ## _name_buffer_len, \
            p_ ## enum_name ## _name_len_out \
        ); \
    ) \
} \
\
enum pkmn_error pkmn_string_to_ ## enum_name ( \
    const char* p_ ## enum_name ## _name, \
    enum pkmn_ ## enum_name * p_ ## enum_name ## _out \
) \
{ \
    PKMN_CHECK_NULL_PARAM(p_ ## enum_name ## _name); \
    PKMN_CHECK_NULL_PARAM(p_ ## enum_name ## _out); \
\
    PKMN_CPP_TO_C( \
        *p_ ## enum_name ## _out = static_cast<enum pkmn_ ## enum_name >( \
                             pkmn::string_to_ ## enum_name (p_ ## enum_name ## _name) \
                         ); \
    ) \
}

PKMN_C_ENUM_TO_STRING_FCNS(ability)
PKMN_C_ENUM_TO_STRING_FCNS(ball)
PKMN_C_ENUM_TO_STRING_FCNS(condition)
PKMN_C_ENUM_TO_STRING_FCNS(egg_group)
PKMN_C_ENUM_TO_STRING_FCNS(game)
PKMN_C_ENUM_TO_STRING_FCNS(gender)
PKMN_C_ENUM_TO_STRING_FCNS(item)
PKMN_C_ENUM_TO_STRING_FCNS(language)
PKMN_C_ENUM_TO_STRING_FCNS(move_damage_class)
PKMN_C_ENUM_TO_STRING_FCNS(move_target)
PKMN_C_ENUM_TO_STRING_FCNS(move)
PKMN_C_ENUM_TO_STRING_FCNS(nature)
PKMN_C_ENUM_TO_STRING_FCNS(species)
PKMN_C_ENUM_TO_STRING_FCNS(stat)
PKMN_C_ENUM_TO_STRING_FCNS(type)

/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/calculations/form.h>

#include <pkmn/calculations/form.hpp>

enum pkmn_error pkmn_calculations_gen2_unown_form(
    int IV_attack,
    int IV_defense,
    int IV_speed,
    int IV_special,
    char* p_form_out,
    size_t buffer_len,
    size_t* p_form_length_out
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_form_out,
                "p_form_out"
            );
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::c::string_cpp_to_c(
                pkmn::calculations::gen2_unown_form(
                    IV_attack, IV_defense,
                    IV_speed, IV_special
                ),
                p_form_out,
                buffer_len,
                p_form_length_out
            );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

enum pkmn_error pkmn_calculations_gen3_unown_form(
    uint32_t personality,
    char* p_form_out,
    size_t buffer_len,
    size_t* p_form_length_out
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_form_out,
                "p_form_out"
            );
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::c::string_cpp_to_c(
                pkmn::calculations::gen3_unown_form(personality),
                p_form_out,
                buffer_len,
                p_form_length_out
            );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

enum pkmn_error pkmn_calculations_wurmple_becomes_silcoon(
    uint32_t personality,
    bool before_gen5,
    bool* p_evolves_out
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_evolves_out,
                "p_evolves_out"
            );
    if(!error)
    {
        auto impl = [&]()
        {
            *p_evolves_out = pkmn::calculations::wurmple_becomes_silcoon(
                               personality, before_gen5
                           );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

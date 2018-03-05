/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_enum_maps.hpp"
#include "cpp_to_c.hpp"
#include "error_internal.hpp"
#include "exception_internal.hpp"

#include "private_exports.hpp"

#include <pkmn-c/pokemon.h>

#include <pkmn/exception.hpp>

#include <boost/assert.hpp>
#include <boost/thread/mutex.hpp>

#include <cstdio>
#include <type_traits>

// The caller is expected to be exception-safe.
template <typename enum_type, typename buffer_type>
static void copy_map_to_buffer(
    const std::map<std::string, buffer_type>& value_map,
    const boost::bimap<std::string, enum_type>& value_enum_bimap,
    buffer_type* values_buffer_out,
    size_t value_buffer_size,
    size_t actual_num_values,
    size_t* actual_num_values_out
)
{
    BOOST_ASSERT(values_buffer_out);

    std::memset(
        values_buffer_out,
        0,
        value_buffer_size * sizeof(buffer_type)
    );

    size_t internal_num_values = std::min<size_t>(value_buffer_size, actual_num_values);
    for(size_t value = 0; value < internal_num_values; ++value)
    {
        enum_type value_enum = enum_type(value);
        BOOST_ASSERT(value_enum_bimap.right.count(value_enum) > 0);

        const std::string& cpp_key = value_enum_bimap.right.at(value_enum);
        if(value_map.count(cpp_key) > 0)
        {
            values_buffer_out[value] = value_map.at(value_enum_bimap.right.at(value_enum));
        }
        else
        {
            if(std::is_same<bool, buffer_type>::value)
            {
                values_buffer_out[value] = false;
            }
            else
            {
                values_buffer_out[value] = buffer_type(-1);
            }
        }
    }

    // Optional parameter
    if(actual_num_values_out)
    {
        *actual_num_values_out = actual_num_values;
    }
}

pkmn_trainer_id_t pkmn_pokemon_default_trainer_id()
{
    return {pkmn::pokemon::DEFAULT_TRAINER_ID};
}

const char* pkmn_pokemon_default_trainer_name()
{
    static const std::string DEFAULT_TRAINER_NAME = pkmn::pokemon::DEFAULT_TRAINER_NAME;
    return DEFAULT_TRAINER_NAME.c_str();
}

pkmn_error_t pkmn_pokemon_init(
    const char* species,
    const char* game,
    const char* form,
    int level,
    pkmn_pokemon_t* pokemon_out
)
{
    PKMN_CHECK_NULL_PARAM(species);
    PKMN_CHECK_NULL_PARAM(game);
    PKMN_CHECK_NULL_PARAM(form);
    PKMN_CHECK_NULL_PARAM(pokemon_out);

    PKMN_CPP_TO_C(
        pkmn::pokemon::sptr cpp = pkmn::pokemon::make(
                                      species,
                                      game,
                                      form,
                                      level
                                  );

        pkmn::c::init_pokemon(
            cpp,
            pokemon_out
        );
    )
}

pkmn_error_t pkmn_pokemon_init_from_file(
    const char* filepath,
    pkmn_pokemon_t* pokemon_out
)
{
    PKMN_CHECK_NULL_PARAM(filepath);
    PKMN_CHECK_NULL_PARAM(pokemon_out);

    PKMN_CPP_TO_C(
        pkmn::pokemon::sptr cpp = pkmn::pokemon::from_file(filepath);

        pkmn::c::init_pokemon(
            cpp,
            pokemon_out
        );
    )
}

pkmn_error_t pkmn_pokemon_free(
    pkmn_pokemon_t* pokemon_ptr
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);

    pkmn::c::free_pointer_and_set_to_null(&pokemon_ptr->species);
    pkmn::c::free_pointer_and_set_to_null(&pokemon_ptr->game);

    PKMN_CPP_TO_C(
        pkmn::c::delete_pointer_and_set_to_null(
            reinterpret_cast<pkmn_pokemon_internal_t**>(&pokemon_ptr->_internal)
        );
    )
}

const char* pkmn_pokemon_strerror(
    pkmn_pokemon_t* pokemon_ptr
)
{
    if(!pokemon_ptr)
    {
        return nullptr;
    }

    try
    {
        pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
        if(!internal_ptr)
        {
            return nullptr;
        }

        boost::mutex::scoped_lock lock(internal_ptr->error_mutex);
        return internal_ptr->last_error.c_str();
    }
    catch(...)
    {
        return nullptr;
    }
}

pkmn_error_t pkmn_pokemon_to_game(
    pkmn_pokemon_t* pokemon_ptr,
    const char* game,
    pkmn_pokemon_t* new_pokemon_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(game, internal_ptr);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(new_pokemon_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::pokemon::sptr new_pokemon_cpp = internal_ptr->cpp->to_game(game);

        pkmn::c::init_pokemon(
            new_pokemon_cpp,
            new_pokemon_out
        );
    )
}

pkmn_error_t pkmn_pokemon_export_to_file(
    pkmn_pokemon_t* pokemon_ptr,
    const char* filepath
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(filepath, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->export_to_file(filepath);
    )
}

pkmn_error_t pkmn_pokemon_get_form(
    pkmn_pokemon_t* pokemon_ptr,
    char* form_buffer,
    size_t form_buffer_len,
    size_t* form_length_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(form_buffer, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::string_cpp_to_c(
            internal_ptr->cpp->get_form(),
            form_buffer,
            form_buffer_len,
            form_length_out
        );
    )
}

pkmn_error_t pkmn_pokemon_set_form(
    pkmn_pokemon_t* pokemon_ptr,
    const char* form
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(form, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_form(form);
    )
}

pkmn_error_t pkmn_pokemon_is_egg(
    pkmn_pokemon_t* pokemon_ptr,
    bool* is_egg_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(is_egg_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *is_egg_out = internal_ptr->cpp->is_egg();
    )
}

pkmn_error_t pkmn_pokemon_set_is_egg(
    pkmn_pokemon_t* pokemon_ptr,
    bool is_egg
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_is_egg(is_egg);
    )
}

pkmn_error_t pkmn_pokemon_get_database_entry(
    pkmn_pokemon_t* pokemon_ptr,
    pkmn_database_pokemon_entry_t* database_entry_ptr
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(database_entry_ptr, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::pokemon_entry_cpp_to_c(
            internal_ptr->cpp->get_database_entry(),
            database_entry_ptr
        );
    )
}

pkmn_error_t pkmn_pokemon_get_condition(
    pkmn_pokemon_t* pokemon_ptr,
    pkmn_condition_t* condition_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(condition_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        std::string condition = internal_ptr->cpp->get_condition();
        BOOST_ASSERT(pkmn::c::CONDITION_BIMAP.left.count(condition) > 0);

        *condition_out = pkmn::c::CONDITION_BIMAP.left.at(condition);
    )
}

pkmn_error_t pkmn_pokemon_set_condition(
    pkmn_pokemon_t* pokemon_ptr,
    pkmn_condition_t condition
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::enforce_value_in_map_keys(
            "Condition",
            condition,
            pkmn::c::CONDITION_BIMAP.right
        );

        internal_ptr->cpp->set_condition(
            pkmn::c::CONDITION_BIMAP.right.at(condition)
        );
    )
}

pkmn_error_t pkmn_pokemon_get_nickname(
    pkmn_pokemon_t* pokemon_ptr,
    char* nickname_out,
    size_t nickname_buffer_len,
    size_t* actual_nickname_len_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(nickname_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::string_cpp_to_c(
            internal_ptr->cpp->get_nickname(),
            nickname_out,
            nickname_buffer_len,
            actual_nickname_len_out
        );
    )
}

pkmn_error_t pkmn_pokemon_set_nickname(
    pkmn_pokemon_t* pokemon_ptr,
    const char* nickname
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(nickname, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_nickname(nickname);
    )
}

pkmn_error_t pkmn_pokemon_get_gender(
    pkmn_pokemon_t* pokemon_ptr,
    pkmn_gender_t* gender_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(gender_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        std::string gender = internal_ptr->cpp->get_gender();
        BOOST_ASSERT(pkmn::c::GENDER_BIMAP.left.count(gender) > 0);

        *gender_out = pkmn::c::GENDER_BIMAP.left.at(gender);
    )
}

pkmn_error_t pkmn_pokemon_set_gender(
    pkmn_pokemon_t* pokemon_ptr,
    pkmn_gender_t gender
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::enforce_value_in_map_keys(
            "Gender",
            gender,
            pkmn::c::GENDER_BIMAP.right
        );

        internal_ptr->cpp->set_gender(
            pkmn::c::GENDER_BIMAP.right.at(gender)
        );
    )
}

pkmn_error_t pkmn_pokemon_is_shiny(
    pkmn_pokemon_t* pokemon_ptr,
    bool* is_shiny_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(is_shiny_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *is_shiny_out = internal_ptr->cpp->is_shiny();
    )
}

pkmn_error_t pkmn_pokemon_set_is_shiny(
    pkmn_pokemon_t* pokemon_ptr,
    bool is_shiny
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_shininess(is_shiny);
    )
}

pkmn_error_t pkmn_pokemon_get_held_item(
    pkmn_pokemon_t* pokemon_ptr,
    char* held_item_out,
    size_t held_item_buffer_len,
    size_t* actual_held_item_len_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(held_item_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::string_cpp_to_c(
            internal_ptr->cpp->get_held_item(),
            held_item_out,
            held_item_buffer_len,
            actual_held_item_len_out
        );
    )
}

pkmn_error_t pkmn_pokemon_set_held_item(
    pkmn_pokemon_t* pokemon_ptr,
    const char* held_item
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(held_item, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_held_item(held_item);
    )
}

pkmn_error_t pkmn_pokemon_get_pokerus_duration(
    pkmn_pokemon_t* pokemon_ptr,
    int* pokerus_duration_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(pokerus_duration_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *pokerus_duration_out = internal_ptr->cpp->get_pokerus_duration();
    )
}

pkmn_error_t pkmn_pokemon_set_pokerus_duration(
    pkmn_pokemon_t* pokemon_ptr,
    int pokerus_duration
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_pokerus_duration(pokerus_duration);
    )
}

pkmn_error_t pkmn_pokemon_get_original_trainer_info(
    pkmn_pokemon_t* pokemon_ptr,
    pkmn_trainer_info_t* original_trainer_info_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(original_trainer_info_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        // Use a separate struct so as to not have any side effects if there
        // are errors.
        pkmn_trainer_info_t trainer_info = {nullptr, {0}, PKMN_GENDER_MALE};

        pkmn::c::string_cpp_to_c_alloc(
            internal_ptr->cpp->get_original_trainer_name(),
            &trainer_info.name
        );
        trainer_info.id.id = internal_ptr->cpp->get_original_trainer_id();

        if(internal_ptr->generation >= 2)
        {
            std::string original_trainer_gender = internal_ptr->cpp->get_original_trainer_gender();
            BOOST_ASSERT(pkmn::c::GENDER_BIMAP.left.count(original_trainer_gender) > 0);

            trainer_info.gender = pkmn::c::GENDER_BIMAP.left.at(original_trainer_gender);
        }

        *original_trainer_info_out = std::move(trainer_info);
    )
}

pkmn_error_t pkmn_pokemon_set_original_trainer_name(
    pkmn_pokemon_t* pokemon_ptr,
    const char* original_trainer_name
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(original_trainer_name, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_original_trainer_name(original_trainer_name);
    )
}

pkmn_error_t pkmn_pokemon_set_original_trainer_public_id(
    pkmn_pokemon_t* pokemon_ptr,
    uint16_t original_trainer_public_id
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_original_trainer_public_id(
            original_trainer_public_id
        );
    )
}

pkmn_error_t pkmn_pokemon_set_original_trainer_secret_id(
    pkmn_pokemon_t* pokemon_ptr,
    uint16_t original_trainer_secret_id
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_original_trainer_secret_id(
            original_trainer_secret_id
        );
    )
}

pkmn_error_t pkmn_pokemon_set_original_trainer_id(
    pkmn_pokemon_t* pokemon_ptr,
    uint32_t original_trainer_id
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_original_trainer_id(original_trainer_id);
    )
}

pkmn_error_t pkmn_pokemon_set_original_trainer_gender(
    pkmn_pokemon_t* pokemon_ptr,
    pkmn_gender_t original_trainer_gender
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::enforce_value_in_map_keys(
            "Original trainer gender",
            original_trainer_gender,
            pkmn::c::GENDER_BIMAP.right
        );

        internal_ptr->cpp->set_original_trainer_gender(
            pkmn::c::GENDER_BIMAP.right.at(original_trainer_gender)
        );
    )
}

pkmn_error_t pkmn_pokemon_get_current_trainer_friendship(
    pkmn_pokemon_t* pokemon_ptr,
    int* current_trainer_friendship_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(current_trainer_friendship_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *current_trainer_friendship_out = internal_ptr->cpp->get_current_trainer_friendship();
    )
}

pkmn_error_t pkmn_pokemon_set_current_trainer_friendship(
    pkmn_pokemon_t* pokemon_ptr,
    int current_trainer_friendship
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_current_trainer_friendship(current_trainer_friendship);
    )
}

pkmn_error_t pkmn_pokemon_get_ability(
    pkmn_pokemon_t* pokemon_ptr,
    char* ability_out,
    size_t ability_buffer_len,
    size_t* actual_ability_len_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(ability_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::string_cpp_to_c(
            internal_ptr->cpp->get_ability(),
            ability_out,
            ability_buffer_len,
            actual_ability_len_out
        );
    )
}

pkmn_error_t pkmn_pokemon_set_ability(
    pkmn_pokemon_t* pokemon_ptr,
    const char* ability
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(ability, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_ability(ability);
    )
}

pkmn_error_t pkmn_pokemon_get_ball(
    pkmn_pokemon_t* pokemon_ptr,
    char* ball_out,
    size_t ball_buffer_len,
    size_t* actual_ball_len_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(ball_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::string_cpp_to_c(
            internal_ptr->cpp->get_ball(),
            ball_out,
            ball_buffer_len,
            actual_ball_len_out
        );
    )
}

pkmn_error_t pkmn_pokemon_set_ball(
    pkmn_pokemon_t* pokemon_ptr,
    const char* ball
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(ball, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_ball(ball);
    )
}

pkmn_error_t pkmn_pokemon_get_level_met(
    pkmn_pokemon_t* pokemon_ptr,
    int* level_met_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(level_met_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *level_met_out = internal_ptr->cpp->get_level_met();
    )
}

pkmn_error_t pkmn_pokemon_set_level_met(
    pkmn_pokemon_t* pokemon_ptr,
    int level_met
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_level_met(level_met);
    )
}

pkmn_error_t pkmn_pokemon_get_location_met(
    pkmn_pokemon_t* pokemon_ptr,
    bool as_egg,
    char* location_met_out,
    size_t location_met_buffer_len,
    size_t* actual_location_met_len_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(location_met_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::string_cpp_to_c(
            internal_ptr->cpp->get_location_met(as_egg),
            location_met_out,
            location_met_buffer_len,
            actual_location_met_len_out
        );
    )
}

pkmn_error_t pkmn_pokemon_set_location_met(
    pkmn_pokemon_t* pokemon_ptr,
    const char* location_met,
    bool as_egg
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(location_met, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_location_met(location_met, as_egg);
    )
}

pkmn_error_t pkmn_pokemon_get_original_game(
    pkmn_pokemon_t* pokemon_ptr,
    char* original_game_out,
    size_t original_game_buffer_len,
    size_t* actual_original_game_len_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(original_game_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::string_cpp_to_c(
            internal_ptr->cpp->get_original_game(),
            original_game_out,
            original_game_buffer_len,
            actual_original_game_len_out
        );
    )
}

pkmn_error_t pkmn_pokemon_set_original_game(
    pkmn_pokemon_t* pokemon_ptr,
    const char* game
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(game, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_original_game(game);
    )
}

pkmn_error_t pkmn_pokemon_get_personality(
    pkmn_pokemon_t* pokemon_ptr,
    uint32_t* personality_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(personality_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *personality_out = internal_ptr->cpp->get_personality();
    )
}

pkmn_error_t pkmn_pokemon_set_personality(
    pkmn_pokemon_t* pokemon_ptr,
    uint32_t personality
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_personality(personality);
    )
}

pkmn_error_t pkmn_pokemon_get_experience(
    pkmn_pokemon_t* pokemon_ptr,
    int* experience_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(experience_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *experience_out = internal_ptr->cpp->get_experience();
    )
}

pkmn_error_t pkmn_pokemon_set_experience(
    pkmn_pokemon_t* pokemon_ptr,
    int experience
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_experience(experience);
    )
}

pkmn_error_t pkmn_pokemon_get_level(
    pkmn_pokemon_t* pokemon_ptr,
    int* level_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(level_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *level_out = internal_ptr->cpp->get_level();
    )
}

pkmn_error_t pkmn_pokemon_set_level(
    pkmn_pokemon_t* pokemon_ptr,
    int level
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_level(level);
    )
}

// Misc

pkmn_error_t pkmn_pokemon_get_markings(
    pkmn_pokemon_t* pokemon_ptr,
    bool* has_markings_buffer_out,
    size_t has_marking_buffer_size,
    size_t* actual_num_markings_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(has_markings_buffer_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        const pkmn::c::marking_bimap_t& marking_bimap = (internal_ptr->generation == 3) ?
            pkmn::c::GEN3_MARKING_BIMAP : pkmn::c::MARKING_BIMAP;

        copy_map_to_buffer(
            internal_ptr->cpp->get_markings(),
            marking_bimap,
            has_markings_buffer_out,
            has_marking_buffer_size,
            marking_bimap.size(),
            actual_num_markings_out
        );
    )
}

pkmn_error_t pkmn_pokemon_set_has_marking(
    pkmn_pokemon_t* pokemon_ptr,
    pkmn_marking_t marking,
    bool has_marking
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        const pkmn::c::marking_bimap_t& marking_bimap = (internal_ptr->generation == 3) ?
            pkmn::c::GEN3_MARKING_BIMAP : pkmn::c::MARKING_BIMAP;

        pkmn::enforce_value_in_map_keys(
            "Marking",
            marking,
            marking_bimap.right
        );

        internal_ptr->cpp->set_marking(
            marking_bimap.right.at(marking),
            has_marking
        );
    )
}

pkmn_error_t pkmn_pokemon_has_ribbon(
    pkmn_pokemon_t* pokemon_ptr,
    const char* ribbon_name,
    bool* has_ribbon_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(ribbon_name, internal_ptr);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(has_ribbon_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        const std::map<std::string, bool>& cpp_ribbons = internal_ptr->cpp->get_ribbons();

        pkmn::enforce_value_in_map_keys(
            "Ribbon name",
            ribbon_name,
            cpp_ribbons
        );

        *has_ribbon_out = cpp_ribbons.at(ribbon_name);
    )
}

pkmn_error_t pkmn_pokemon_set_has_ribbon(
    pkmn_pokemon_t* pokemon_ptr,
    const char* ribbon_name,
    bool has_ribbon
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(ribbon_name, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        const std::map<std::string, bool>& cpp_ribbons = internal_ptr->cpp->get_ribbons();

        pkmn::enforce_value_in_map_keys(
            "Ribbon name",
            ribbon_name,
            cpp_ribbons
        );

        internal_ptr->cpp->set_ribbon(ribbon_name, has_ribbon);
    )
}

pkmn_error_t pkmn_pokemon_get_ribbon_names(
    pkmn_pokemon_t* pokemon_ptr,
    pkmn_string_list_t* ribbon_names_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(ribbon_names_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::string_map_keys_to_string_list(
            internal_ptr->cpp->get_ribbons(),
            ribbon_names_out
        );
    )
}

pkmn_error_t pkmn_pokemon_get_contest_stats(
    pkmn_pokemon_t* pokemon_ptr,
    int* contest_stats_buffer_out,
    size_t contest_stat_buffer_size,
    size_t* actual_num_contest_stats_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(contest_stats_buffer_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        copy_map_to_buffer(
            internal_ptr->cpp->get_contest_stats(),
            pkmn::c::CONTEST_STAT_BIMAP,
            contest_stats_buffer_out,
            contest_stat_buffer_size,
            PKMN_NUM_CONTEST_STATS,
            actual_num_contest_stats_out
        );
    )
}

pkmn_error_t pkmn_pokemon_set_contest_stat(
    pkmn_pokemon_t* pokemon_ptr,
    pkmn_contest_stat_t contest_stat,
    int value
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::enforce_value_in_map_keys(
            "Contest stat",
            contest_stat,
            pkmn::c::CONTEST_STAT_BIMAP.right
        );

        internal_ptr->cpp->set_contest_stat(
            pkmn::c::CONTEST_STAT_BIMAP.right.at(contest_stat),
            value
        );
    )
}

// Moves

pkmn_error_t pkmn_pokemon_get_moves(
    pkmn_pokemon_t* pokemon_ptr,
    pkmn_move_slots_t* move_slots_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(move_slots_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::move_slots_cpp_to_c(
            internal_ptr->cpp->get_moves(),
            move_slots_out
        );
    )
}

pkmn_error_t pkmn_pokemon_set_move(
    pkmn_pokemon_t* pokemon_ptr,
    size_t index,
    const char* move
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(move, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_move(move, int(index));
    )
}

pkmn_error_t pkmn_pokemon_set_move_pp(
    pkmn_pokemon_t* pokemon_ptr,
    size_t index,
    int pp
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_move_pp(int(index), pp);
    )
}

// Stats

pkmn_error_t pkmn_pokemon_get_EVs(
    pkmn_pokemon_t* pokemon_ptr,
    int* EVs_buffer_out,
    size_t stat_buffer_size,
    size_t* actual_num_EVs_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(EVs_buffer_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        copy_map_to_buffer(
            internal_ptr->cpp->get_EVs(),
            pkmn::c::STAT_BIMAP,
            EVs_buffer_out,
            stat_buffer_size,
            PKMN_NUM_STATS,
            actual_num_EVs_out
        );
    )
}

pkmn_error_t pkmn_pokemon_set_EV(
    pkmn_pokemon_t* pokemon_ptr,
    pkmn_stat_t stat,
    int value
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::enforce_value_in_map_keys(
            "Stat",
            stat,
            pkmn::c::STAT_BIMAP.right
        );

        internal_ptr->cpp->set_EV(
            pkmn::c::STAT_BIMAP.right.at(stat),
            value
        );
    )
}

pkmn_error_t pkmn_pokemon_get_IVs(
    pkmn_pokemon_t* pokemon_ptr,
    int* IVs_buffer_out,
    size_t stat_buffer_size,
    size_t* actual_num_IVs_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(IVs_buffer_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        copy_map_to_buffer(
            internal_ptr->cpp->get_IVs(),
            pkmn::c::STAT_BIMAP,
            IVs_buffer_out,
            stat_buffer_size,
            PKMN_NUM_STATS,
            actual_num_IVs_out
        );
    )
}

pkmn_error_t pkmn_pokemon_set_IV(
    pkmn_pokemon_t* pokemon_ptr,
    pkmn_stat_t stat,
    int value
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::enforce_value_in_map_keys(
            "Stat",
            stat,
            pkmn::c::STAT_BIMAP.right
        );

        internal_ptr->cpp->set_IV(
            pkmn::c::STAT_BIMAP.right.at(stat),
            value
        );
    )
}

pkmn_error_t pkmn_pokemon_get_stats(
    pkmn_pokemon_t* pokemon_ptr,
    int* stats_buffer_out,
    size_t stat_buffer_size,
    size_t* actual_num_stats_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(stats_buffer_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        copy_map_to_buffer(
            internal_ptr->cpp->get_stats(),
            pkmn::c::STAT_BIMAP,
            stats_buffer_out,
            stat_buffer_size,
            PKMN_NUM_STATS,
            actual_num_stats_out
        );
    )
}

pkmn_error_t pkmn_pokemon_get_current_hp(
    pkmn_pokemon_t* pokemon_ptr,
    int* current_hp_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(current_hp_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *current_hp_out = internal_ptr->cpp->get_current_hp();
    )
}

pkmn_error_t pkmn_pokemon_set_current_hp(
    pkmn_pokemon_t* pokemon_ptr,
    int current_hp
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_current_hp(current_hp);
    )
}

pkmn_error_t pkmn_pokemon_get_icon_filepath(
    pkmn_pokemon_t* pokemon_ptr,
    char* icon_filepath_out,
    size_t icon_filepath_buffer_len,
    size_t* actual_icon_filepath_len_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(icon_filepath_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::string_cpp_to_c(
            internal_ptr->cpp->get_icon_filepath(),
            icon_filepath_out,
            icon_filepath_buffer_len,
            actual_icon_filepath_len_out
        );
    )
}

pkmn_error_t pkmn_pokemon_get_sprite_filepath(
    pkmn_pokemon_t* pokemon_ptr,
    char* sprite_filepath_out,
    size_t sprite_filepath_buffer_len,
    size_t* actual_sprite_filepath_len_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(sprite_filepath_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::string_cpp_to_c(
            internal_ptr->cpp->get_sprite_filepath(),
            sprite_filepath_out,
            sprite_filepath_buffer_len,
            actual_sprite_filepath_len_out
        );
    )
}

// Attributes

pkmn_error_t pkmn_pokemon_get_numeric_attribute(
    pkmn_pokemon_t* pokemon_ptr,
    const char* attribute_name,
    int* value_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(attribute_name, internal_ptr);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(value_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *value_out = internal_ptr->cpp->get_numeric_attribute(attribute_name);
    )
}

pkmn_error_t pkmn_pokemon_set_numeric_attribute(
    pkmn_pokemon_t* pokemon_ptr,
    const char* attribute_name,
    int value
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(attribute_name, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_numeric_attribute(attribute_name, value);
    )
}

pkmn_error_t pkmn_pokemon_get_string_attribute(
    pkmn_pokemon_t* pokemon_ptr,
    const char* attribute_name,
    char* value_out,
    size_t value_buffer_len,
    size_t* actual_value_len_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(attribute_name, internal_ptr);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(value_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::string_cpp_to_c(
            internal_ptr->cpp->get_string_attribute(attribute_name),
            value_out,
            value_buffer_len,
            actual_value_len_out
        );
    )
}

pkmn_error_t pkmn_pokemon_set_string_attribute(
    pkmn_pokemon_t* pokemon_ptr,
    const char* attribute_name,
    const char* value
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(attribute_name, internal_ptr);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(value, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_string_attribute(attribute_name, value);
    )
}

pkmn_error_t pkmn_pokemon_get_boolean_attribute(
    pkmn_pokemon_t* pokemon_ptr,
    const char* attribute_name,
    bool* value_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(attribute_name, internal_ptr);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(value_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *value_out = internal_ptr->cpp->get_boolean_attribute(attribute_name);
    )
}

pkmn_error_t pkmn_pokemon_set_boolean_attribute(
    pkmn_pokemon_t* pokemon_ptr,
    const char* attribute_name,
    bool value
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(attribute_name, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_boolean_attribute(attribute_name, value);
    )
}

pkmn_error_t pkmn_pokemon_get_attribute_names(
    pkmn_pokemon_t* pokemon_ptr,
    pkmn_attribute_names_t* attribute_names_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(attribute_names_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::string_list_cpp_to_c(
            internal_ptr->cpp->get_numeric_attribute_names(),
            &attribute_names_out->numeric_attribute_names
        );
        pkmn::c::string_list_cpp_to_c(
            internal_ptr->cpp->get_string_attribute_names(),
            &attribute_names_out->string_attribute_names
        );
        pkmn::c::string_list_cpp_to_c(
            internal_ptr->cpp->get_boolean_attribute_names(),
            &attribute_names_out->boolean_attribute_names
        );
    )
}

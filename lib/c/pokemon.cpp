/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "enum_maps.hpp"
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
    buffer_type* p_values_buffer_out,
    size_t value_buffer_size,
    size_t p_actual_num_values,
    size_t* p_p_actual_num_values_out
)
{
    BOOST_ASSERT(p_values_buffer_out != nullptr);

    std::memset(
        p_values_buffer_out,
        0,
        value_buffer_size * sizeof(buffer_type)
    );

    size_t internal_num_values = std::min<size_t>(value_buffer_size, p_actual_num_values);
    for(size_t value = 0; value < internal_num_values; ++value)
    {
        enum_type value_enum = enum_type(value);
        BOOST_ASSERT(value_enum_bimap.right.count(value_enum) > 0);

        const std::string& cpp_key = value_enum_bimap.right.at(value_enum);
        if(value_map.count(cpp_key) > 0)
        {
            p_values_buffer_out[value] = value_map.at(value_enum_bimap.right.at(value_enum));
        }
        else
        {
            if(std::is_same<bool, buffer_type>::value)
            {
                p_values_buffer_out[value] = false;
            }
            else
            {
                p_values_buffer_out[value] = buffer_type(-1);
            }
        }
    }

    // Optional parameter
    if(p_p_actual_num_values_out)
    {
        *p_p_actual_num_values_out = p_actual_num_values;
    }
}

union pkmn_trainer_id pkmn_pokemon_default_trainer_id()
{
    return {pkmn::pokemon::DEFAULT_TRAINER_ID};
}

const char* pkmn_pokemon_default_trainer_name()
{
    return pkmn::pokemon::DEFAULT_TRAINER_NAME.c_str();
}

enum pkmn_error pkmn_pokemon_init(
    const char* p_species,
    const char* p_game,
    const char* p_form,
    int level,
    struct pkmn_pokemon* p_pokemon_out
)
{
    PKMN_CHECK_NULL_PARAM(p_species);
    PKMN_CHECK_NULL_PARAM(p_game);
    PKMN_CHECK_NULL_PARAM(p_form);
    PKMN_CHECK_NULL_PARAM(p_pokemon_out);

    PKMN_CPP_TO_C(
        pkmn::pokemon::sptr cpp = pkmn::pokemon::make(
                                      p_species,
                                      p_game,
                                      p_form,
                                      level
                                  );

        pkmn::c::init_pokemon(
            cpp,
            p_pokemon_out
        );
    )
}

enum pkmn_error pkmn_pokemon_init_from_file(
    const char* p_filepath,
    struct pkmn_pokemon* p_pokemon_out
)
{
    PKMN_CHECK_NULL_PARAM(p_filepath);
    PKMN_CHECK_NULL_PARAM(p_pokemon_out);

    PKMN_CPP_TO_C(
        pkmn::pokemon::sptr cpp = pkmn::pokemon::from_file(p_filepath);

        pkmn::c::init_pokemon(
            cpp,
            p_pokemon_out
        );
    )
}

enum pkmn_error pkmn_pokemon_free(
    struct pkmn_pokemon* p_pokemon
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);

    pkmn::c::free_pointer_and_set_to_null(&p_pokemon->p_species);
    pkmn::c::free_pointer_and_set_to_null(&p_pokemon->p_game);

    PKMN_CPP_TO_C(
        pkmn::c::delete_pointer_and_set_to_null(
            reinterpret_cast<pkmn_pokemon_internal_t**>(&p_pokemon->p_internal)
        );
    )
}

const char* pkmn_pokemon_strerror(
    struct pkmn_pokemon* p_pokemon
)
{
    if(!p_pokemon)
    {
        return nullptr;
    }

    try
    {
        pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
        if(!p_internal)
        {
            return nullptr;
        }

        boost::mutex::scoped_lock lock(p_internal->error_mutex);
        return p_internal->last_error.c_str();
    }
    catch(...)
    {
        return nullptr;
    }
}

enum pkmn_error pkmn_pokemon_to_game(
    struct pkmn_pokemon* p_pokemon,
    const char* p_game,
    struct pkmn_pokemon* p_new_pokemon_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_game, p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_new_pokemon_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::pokemon::sptr new_pokemon_cpp = p_internal->cpp->to_game(p_game);

        pkmn::c::init_pokemon(
            new_pokemon_cpp,
            p_new_pokemon_out
        );
    )
}

enum pkmn_error pkmn_pokemon_export_to_file(
    struct pkmn_pokemon* p_pokemon,
    const char* p_filepath
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_filepath, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->export_to_file(p_filepath);
    )
}

enum pkmn_error pkmn_pokemon_get_form(
    struct pkmn_pokemon* p_pokemon,
    char* form_buffer,
    size_t form_buffer_len,
    size_t* form_length_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(form_buffer, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::string_cpp_to_c(
            p_internal->cpp->get_form(),
            form_buffer,
            form_buffer_len,
            form_length_out
        );
    )
}

enum pkmn_error pkmn_pokemon_set_form(
    struct pkmn_pokemon* p_pokemon,
    const char* p_form
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_form, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_form(p_form);
    )
}

enum pkmn_error pkmn_pokemon_is_egg(
    struct pkmn_pokemon* p_pokemon,
    bool* is_egg_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(is_egg_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        *is_egg_out = p_internal->cpp->is_egg();
    )
}

enum pkmn_error pkmn_pokemon_set_is_egg(
    struct pkmn_pokemon* p_pokemon,
    bool is_egg
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_is_egg(is_egg);
    )
}

enum pkmn_error pkmn_pokemon_get_database_entry(
    struct pkmn_pokemon* p_pokemon,
    struct pkmn_database_pokemon_entry* p_database_entry_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_database_entry_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::pokemon_entry_cpp_to_c(
            p_internal->cpp->get_database_entry(),
            p_database_entry_out
        );
    )
}

enum pkmn_error pkmn_pokemon_get_condition(
    struct pkmn_pokemon* p_pokemon,
    enum pkmn_condition* p_condition_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_condition_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        std::string condition = p_internal->cpp->get_condition();

        const pkmn::c::condition_bimap_t& condition_bimap = pkmn::c::get_condition_bimap();
        BOOST_ASSERT(condition_bimap.left.count(condition) > 0);
        *p_condition_out = condition_bimap.left.at(condition);
    )
}

enum pkmn_error pkmn_pokemon_set_condition(
    struct pkmn_pokemon* p_pokemon,
    enum pkmn_condition condition
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        const pkmn::c::condition_bimap_t& condition_bimap = pkmn::c::get_condition_bimap();

        pkmn::enforce_value_in_map_keys(
            "Condition",
            condition,
            condition_bimap.right
        );

        p_internal->cpp->set_condition(
            condition_bimap.right.at(condition)
        );
    )
}

enum pkmn_error pkmn_pokemon_get_nickname(
    struct pkmn_pokemon* p_pokemon,
    char* p_nickname_out,
    size_t nickname_buffer_len,
    size_t* p_actual_nickname_len_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_nickname_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::string_cpp_to_c(
            p_internal->cpp->get_nickname(),
            p_nickname_out,
            nickname_buffer_len,
            p_actual_nickname_len_out
        );
    )
}

enum pkmn_error pkmn_pokemon_set_nickname(
    struct pkmn_pokemon* p_pokemon,
    const char* p_nickname
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_nickname, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_nickname(p_nickname);
    )
}

enum pkmn_error pkmn_pokemon_get_gender(
    struct pkmn_pokemon* p_pokemon,
    enum pkmn_gender* p_gender_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_gender_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        std::string gender = p_internal->cpp->get_gender();

        const pkmn::c::gender_bimap_t& gender_bimap = pkmn::c::get_gender_bimap();
        BOOST_ASSERT(gender_bimap.left.count(gender) > 0);
        *p_gender_out = gender_bimap.left.at(gender);
    )
}

enum pkmn_error pkmn_pokemon_set_gender(
    struct pkmn_pokemon* p_pokemon,
    enum pkmn_gender gender
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        const pkmn::c::gender_bimap_t& gender_bimap = pkmn::c::get_gender_bimap();

        pkmn::enforce_value_in_map_keys(
            "Gender",
            gender,
            gender_bimap.right
        );

        p_internal->cpp->set_gender(
            gender_bimap.right.at(gender)
        );
    )
}

enum pkmn_error pkmn_pokemon_is_shiny(
    struct pkmn_pokemon* p_pokemon,
    bool* p_is_shiny_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_is_shiny_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        *p_is_shiny_out = p_internal->cpp->is_shiny();
    )
}

enum pkmn_error pkmn_pokemon_set_is_shiny(
    struct pkmn_pokemon* p_pokemon,
    bool is_shiny
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_shininess(is_shiny);
    )
}

enum pkmn_error pkmn_pokemon_get_held_item(
    struct pkmn_pokemon* p_pokemon,
    char* p_held_item_out,
    size_t held_item_buffer_len,
    size_t* p_actual_held_item_len_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_held_item_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::string_cpp_to_c(
            p_internal->cpp->get_held_item(),
            p_held_item_out,
            held_item_buffer_len,
            p_actual_held_item_len_out
        );
    )
}

enum pkmn_error pkmn_pokemon_set_held_item(
    struct pkmn_pokemon* p_pokemon,
    const char* p_held_item
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_held_item, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_held_item(p_held_item);
    )
}

enum pkmn_error pkmn_pokemon_get_pokerus_duration(
    struct pkmn_pokemon* p_pokemon,
    int* p_pokerus_duration_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_pokerus_duration_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        *p_pokerus_duration_out = p_internal->cpp->get_pokerus_duration();
    )
}

enum pkmn_error pkmn_pokemon_set_pokerus_duration(
    struct pkmn_pokemon* p_pokemon,
    int pokerus_duration
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_pokerus_duration(pokerus_duration);
    )
}

enum pkmn_error pkmn_pokemon_get_original_trainer_info(
    struct pkmn_pokemon* p_pokemon,
    struct pkmn_trainer_info* p_original_trainer_info_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_original_trainer_info_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        // Use a separate struct so as to not have any side effects if there
        // are errors.
        struct pkmn_trainer_info trainer_info = {nullptr, {0}, PKMN_GENDER_MALE};

        pkmn::c::string_cpp_to_c_alloc(
            p_internal->cpp->get_original_trainer_name(),
            &trainer_info.p_name
        );
        trainer_info.id.id = p_internal->cpp->get_original_trainer_id();

        if(p_internal->generation >= 2)
        {
            std::string original_trainer_gender = p_internal->cpp->get_original_trainer_gender();

            const pkmn::c::gender_bimap_t& gender_bimap = pkmn::c::get_gender_bimap();
            BOOST_ASSERT(gender_bimap.left.count(original_trainer_gender) > 0);
            trainer_info.gender = gender_bimap.left.at(original_trainer_gender);
        }

        *p_original_trainer_info_out = std::move(trainer_info);
    )
}

enum pkmn_error pkmn_pokemon_set_original_trainer_name(
    struct pkmn_pokemon* p_pokemon,
    const char* p_original_trainer_name
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_original_trainer_name, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_original_trainer_name(p_original_trainer_name);
    )
}

enum pkmn_error pkmn_pokemon_set_original_trainer_public_id(
    struct pkmn_pokemon* p_pokemon,
    uint16_t original_trainer_public_id
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_original_trainer_public_id(
            original_trainer_public_id
        );
    )
}

enum pkmn_error pkmn_pokemon_set_original_trainer_secret_id(
    struct pkmn_pokemon* p_pokemon,
    uint16_t original_trainer_secret_id
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_original_trainer_secret_id(
            original_trainer_secret_id
        );
    )
}

enum pkmn_error pkmn_pokemon_set_original_trainer_id(
    struct pkmn_pokemon* p_pokemon,
    uint32_t original_trainer_id
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_original_trainer_id(original_trainer_id);
    )
}

enum pkmn_error pkmn_pokemon_set_original_trainer_gender(
    struct pkmn_pokemon* p_pokemon,
    enum pkmn_gender original_trainer_gender
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        const pkmn::c::gender_bimap_t& gender_bimap = pkmn::c::get_gender_bimap();

        pkmn::enforce_value_in_map_keys(
            "Original trainer gender",
            original_trainer_gender,
            gender_bimap.right
        );

        p_internal->cpp->set_original_trainer_gender(
            gender_bimap.right.at(original_trainer_gender)
        );
    )
}

enum pkmn_error pkmn_pokemon_get_current_trainer_friendship(
    struct pkmn_pokemon* p_pokemon,
    int* p_current_trainer_friendship_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_current_trainer_friendship_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        *p_current_trainer_friendship_out = p_internal->cpp->get_current_trainer_friendship();
    )
}

enum pkmn_error pkmn_pokemon_set_current_trainer_friendship(
    struct pkmn_pokemon* p_pokemon,
    int current_trainer_friendship
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_current_trainer_friendship(current_trainer_friendship);
    )
}

enum pkmn_error pkmn_pokemon_get_ability(
    struct pkmn_pokemon* p_pokemon,
    char* p_ability_out,
    size_t ability_buffer_len,
    size_t* p_actual_ability_len_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_ability_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::string_cpp_to_c(
            p_internal->cpp->get_ability(),
            p_ability_out,
            ability_buffer_len,
            p_actual_ability_len_out
        );
    )
}

enum pkmn_error pkmn_pokemon_set_ability(
    struct pkmn_pokemon* p_pokemon,
    const char* p_ability
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_ability, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_ability(p_ability);
    )
}

enum pkmn_error pkmn_pokemon_get_ball(
    struct pkmn_pokemon* p_pokemon,
    char* p_ball_out,
    size_t ball_buffer_len,
    size_t* p_actual_ball_len_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_ball_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::string_cpp_to_c(
            p_internal->cpp->get_ball(),
            p_ball_out,
            ball_buffer_len,
            p_actual_ball_len_out
        );
    )
}

enum pkmn_error pkmn_pokemon_set_ball(
    struct pkmn_pokemon* p_pokemon,
    const char* p_ball
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_ball, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_ball(p_ball);
    )
}

enum pkmn_error pkmn_pokemon_get_level_met(
    struct pkmn_pokemon* p_pokemon,
    int* p_level_met_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_level_met_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        *p_level_met_out = p_internal->cpp->get_level_met();
    )
}

enum pkmn_error pkmn_pokemon_set_level_met(
    struct pkmn_pokemon* p_pokemon,
    int level_met
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_level_met(level_met);
    )
}

enum pkmn_error pkmn_pokemon_get_location_met(
    struct pkmn_pokemon* p_pokemon,
    bool as_egg,
    char* p_location_met_out,
    size_t location_met_buffer_len,
    size_t* p_actual_location_met_len_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_location_met_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::string_cpp_to_c(
            p_internal->cpp->get_location_met(as_egg),
            p_location_met_out,
            location_met_buffer_len,
            p_actual_location_met_len_out
        );
    )
}

enum pkmn_error pkmn_pokemon_set_location_met(
    struct pkmn_pokemon* p_pokemon,
    const char* p_location_met,
    bool as_egg
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_location_met, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_location_met(p_location_met, as_egg);
    )
}

enum pkmn_error pkmn_pokemon_get_original_game(
    struct pkmn_pokemon* p_pokemon,
    char* p_original_game_out,
    size_t original_game_buffer_len,
    size_t* p_actual_original_game_len_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_original_game_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::string_cpp_to_c(
            p_internal->cpp->get_original_game(),
            p_original_game_out,
            original_game_buffer_len,
            p_actual_original_game_len_out
        );
    )
}

enum pkmn_error pkmn_pokemon_set_original_game(
    struct pkmn_pokemon* p_pokemon,
    const char* p_game
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_game, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_original_game(p_game);
    )
}

enum pkmn_error pkmn_pokemon_get_personality(
    struct pkmn_pokemon* p_pokemon,
    uint32_t* p_personality_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_personality_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        *p_personality_out = p_internal->cpp->get_personality();
    )
}

enum pkmn_error pkmn_pokemon_set_personality(
    struct pkmn_pokemon* p_pokemon,
    uint32_t personality
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_personality(personality);
    )
}

enum pkmn_error pkmn_pokemon_get_experience(
    struct pkmn_pokemon* p_pokemon,
    int* p_experience_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_experience_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        *p_experience_out = p_internal->cpp->get_experience();
    )
}

enum pkmn_error pkmn_pokemon_set_experience(
    struct pkmn_pokemon* p_pokemon,
    int experience
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_experience(experience);
    )
}

enum pkmn_error pkmn_pokemon_get_level(
    struct pkmn_pokemon* p_pokemon,
    int* p_level_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_level_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        *p_level_out = p_internal->cpp->get_level();
    )
}

enum pkmn_error pkmn_pokemon_set_level(
    struct pkmn_pokemon* p_pokemon,
    int level
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_level(level);
    )
}

// Misc

enum pkmn_error pkmn_pokemon_get_markings(
    struct pkmn_pokemon* p_pokemon,
    bool* p_has_markings_buffer_out,
    size_t has_marking_buffer_size,
    size_t* p_actual_num_markings_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_has_markings_buffer_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        const pkmn::c::marking_bimap_t& marking_bimap = (p_internal->generation == 3) ?
            pkmn::c::get_gen3_marking_bimap() : pkmn::c::get_marking_bimap();

        copy_map_to_buffer(
            p_internal->cpp->get_markings(),
            marking_bimap,
            p_has_markings_buffer_out,
            has_marking_buffer_size,
            marking_bimap.size(),
            p_actual_num_markings_out
        );
    )
}

enum pkmn_error pkmn_pokemon_set_has_marking(
    struct pkmn_pokemon* p_pokemon,
    enum pkmn_marking marking,
    bool has_marking
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        const pkmn::c::marking_bimap_t& marking_bimap = (p_internal->generation == 3) ?
            pkmn::c::get_gen3_marking_bimap() : pkmn::c::get_marking_bimap();

        pkmn::enforce_value_in_map_keys(
            "Marking",
            marking,
            marking_bimap.right
        );

        p_internal->cpp->set_marking(
            marking_bimap.right.at(marking),
            has_marking
        );
    )
}

enum pkmn_error pkmn_pokemon_has_ribbon(
    struct pkmn_pokemon* p_pokemon,
    const char* p_ribbon_name,
    bool* p_has_ribbon_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_ribbon_name, p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_has_ribbon_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        const std::map<std::string, bool>& cpp_ribbons = p_internal->cpp->get_ribbons();

        pkmn::enforce_value_in_map_keys(
            "Ribbon name",
            p_ribbon_name,
            cpp_ribbons
        );

        *p_has_ribbon_out = cpp_ribbons.at(p_ribbon_name);
    )
}

enum pkmn_error pkmn_pokemon_set_has_ribbon(
    struct pkmn_pokemon* p_pokemon,
    const char* p_ribbon_name,
    bool has_ribbon
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_ribbon_name, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        const std::map<std::string, bool>& cpp_ribbons = p_internal->cpp->get_ribbons();

        pkmn::enforce_value_in_map_keys(
            "Ribbon name",
            p_ribbon_name,
            cpp_ribbons
        );

        p_internal->cpp->set_ribbon(p_ribbon_name, has_ribbon);
    )
}

enum pkmn_error pkmn_pokemon_get_ribbon_names(
    struct pkmn_pokemon* p_pokemon,
    struct pkmn_string_list* p_ribbon_names_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_ribbon_names_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::string_map_keys_to_string_list(
            p_internal->cpp->get_ribbons(),
            p_ribbon_names_out
        );
    )
}

enum pkmn_error pkmn_pokemon_get_contest_stats(
    struct pkmn_pokemon* p_pokemon,
    int* p_contest_stats_buffer_out,
    size_t contest_stat_buffer_size,
    size_t* p_actual_num_contest_stats_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_contest_stats_buffer_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        copy_map_to_buffer(
            p_internal->cpp->get_contest_stats(),
            pkmn::c::get_contest_stat_bimap(),
            p_contest_stats_buffer_out,
            contest_stat_buffer_size,
            PKMN_NUM_CONTEST_STATS,
            p_actual_num_contest_stats_out
        );
    )
}

enum pkmn_error pkmn_pokemon_set_contest_stat(
    struct pkmn_pokemon* p_pokemon,
    enum pkmn_contest_stat contest_stat,
    int value
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        const pkmn::c::contest_stat_bimap_t& contest_stat_bimap = pkmn::c::get_contest_stat_bimap();

        pkmn::enforce_value_in_map_keys(
            "Contest stat",
            contest_stat,
            contest_stat_bimap.right
        );

        p_internal->cpp->set_contest_stat(
            contest_stat_bimap.right.at(contest_stat),
            value
        );
    )
}

// Moves

enum pkmn_error pkmn_pokemon_get_moves(
    struct pkmn_pokemon* p_pokemon,
    struct pkmn_move_slots* p_move_slots_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_move_slots_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::move_slots_cpp_to_c(
            p_internal->cpp->get_moves(),
            p_move_slots_out
        );
    )
}

enum pkmn_error pkmn_pokemon_set_move(
    struct pkmn_pokemon* p_pokemon,
    size_t index,
    const char* p_move
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_move, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_move(p_move, int(index));
    )
}

enum pkmn_error pkmn_pokemon_set_move_pp(
    struct pkmn_pokemon* p_pokemon,
    size_t index,
    int pp
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_move_pp(int(index), pp);
    )
}

// Stats

enum pkmn_error pkmn_pokemon_get_EVs(
    struct pkmn_pokemon* p_pokemon,
    int* p_EVs_buffer_out,
    size_t stat_buffer_size,
    size_t* p_actual_num_EVs_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_EVs_buffer_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        copy_map_to_buffer(
            p_internal->cpp->get_EVs(),
            pkmn::c::get_stat_bimap(),
            p_EVs_buffer_out,
            stat_buffer_size,
            PKMN_NUM_STATS,
            p_actual_num_EVs_out
        );
    )
}

enum pkmn_error pkmn_pokemon_set_EV(
    struct pkmn_pokemon* p_pokemon,
    enum pkmn_stat stat,
    int value
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        const pkmn::c::stat_bimap_t& stat_bimap = pkmn::c::get_stat_bimap();

        pkmn::enforce_value_in_map_keys(
            "Stat",
            stat,
            stat_bimap.right
        );

        p_internal->cpp->set_EV(
            stat_bimap.right.at(stat),
            value
        );
    )
}

enum pkmn_error pkmn_pokemon_get_IVs(
    struct pkmn_pokemon* p_pokemon,
    int* p_IVs_buffer_out,
    size_t stat_buffer_size,
    size_t* p_actual_num_IVs_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_IVs_buffer_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        copy_map_to_buffer(
            p_internal->cpp->get_IVs(),
            pkmn::c::get_stat_bimap(),
            p_IVs_buffer_out,
            stat_buffer_size,
            PKMN_NUM_STATS,
            p_actual_num_IVs_out
        );
    )
}

enum pkmn_error pkmn_pokemon_set_IV(
    struct pkmn_pokemon* p_pokemon,
    enum pkmn_stat stat,
    int value
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        const pkmn::c::stat_bimap_t& stat_bimap = pkmn::c::get_stat_bimap();

        pkmn::enforce_value_in_map_keys(
            "Stat",
            stat,
            stat_bimap.right
        );

        p_internal->cpp->set_IV(
            stat_bimap.right.at(stat),
            value
        );
    )
}

enum pkmn_error pkmn_pokemon_get_stats(
    struct pkmn_pokemon* p_pokemon,
    int* p_stats_buffer_out,
    size_t stat_buffer_size,
    size_t* p_actual_num_stats_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_stats_buffer_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        copy_map_to_buffer(
            p_internal->cpp->get_stats(),
            pkmn::c::get_stat_bimap(),
            p_stats_buffer_out,
            stat_buffer_size,
            PKMN_NUM_STATS,
            p_actual_num_stats_out
        );
    )
}

enum pkmn_error pkmn_pokemon_get_current_hp(
    struct pkmn_pokemon* p_pokemon,
    int* p_current_hp_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_current_hp_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        *p_current_hp_out = p_internal->cpp->get_current_hp();
    )
}

enum pkmn_error pkmn_pokemon_set_current_hp(
    struct pkmn_pokemon* p_pokemon,
    int current_hp
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_current_hp(current_hp);
    )
}

enum pkmn_error pkmn_pokemon_get_icon_filepath(
    struct pkmn_pokemon* p_pokemon,
    char* p_icon_filepath_out,
    size_t p_icon_filepath_buffer_len,
    size_t* p_actual_p_icon_filepath_len_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_icon_filepath_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::string_cpp_to_c(
            p_internal->cpp->get_icon_filepath(),
            p_icon_filepath_out,
            p_icon_filepath_buffer_len,
            p_actual_p_icon_filepath_len_out
        );
    )
}

enum pkmn_error pkmn_pokemon_get_sprite_filepath(
    struct pkmn_pokemon* p_pokemon,
    char* p_sprite_filepath_out,
    size_t p_sprite_filepath_buffer_len,
    size_t* p_actual_p_sprite_filepath_len_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_sprite_filepath_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::string_cpp_to_c(
            p_internal->cpp->get_sprite_filepath(),
            p_sprite_filepath_out,
            p_sprite_filepath_buffer_len,
            p_actual_p_sprite_filepath_len_out
        );
    )
}

// Attributes

enum pkmn_error pkmn_pokemon_get_numeric_attribute(
    struct pkmn_pokemon* p_pokemon,
    const char* p_attribute_name,
    int* p_value_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_attribute_name, p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_value_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        *p_value_out = p_internal->cpp->get_numeric_attribute(p_attribute_name);
    )
}

enum pkmn_error pkmn_pokemon_set_numeric_attribute(
    struct pkmn_pokemon* p_pokemon,
    const char* p_attribute_name,
    int value
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_attribute_name, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_numeric_attribute(p_attribute_name, value);
    )
}

enum pkmn_error pkmn_pokemon_get_string_attribute(
    struct pkmn_pokemon* p_pokemon,
    const char* p_attribute_name,
    char* p_value_out,
    size_t value_buffer_len,
    size_t* p_actual_value_len_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_attribute_name, p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_value_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::string_cpp_to_c(
            p_internal->cpp->get_string_attribute(p_attribute_name),
            p_value_out,
            value_buffer_len,
            p_actual_value_len_out
        );
    )
}

enum pkmn_error pkmn_pokemon_set_string_attribute(
    struct pkmn_pokemon* p_pokemon,
    const char* p_attribute_name,
    const char* p_value
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_attribute_name, p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_value, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_string_attribute(p_attribute_name, p_value);
    )
}

enum pkmn_error pkmn_pokemon_get_boolean_attribute(
    struct pkmn_pokemon* p_pokemon,
    const char* p_attribute_name,
    bool* p_value_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_attribute_name, p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_value_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        *p_value_out = p_internal->cpp->get_boolean_attribute(p_attribute_name);
    )
}

enum pkmn_error pkmn_pokemon_set_boolean_attribute(
    struct pkmn_pokemon* p_pokemon,
    const char* p_attribute_name,
    bool value
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_attribute_name, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_boolean_attribute(p_attribute_name, value);
    )
}

enum pkmn_error pkmn_pokemon_get_attribute_names(
    struct pkmn_pokemon* p_pokemon,
    struct pkmn_attribute_names* p_attribute_names_out
)
{
    PKMN_CHECK_NULL_PARAM(p_pokemon);
    pkmn_pokemon_internal_t* p_internal = POKEMON_INTERNAL_RCAST(p_pokemon->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_attribute_names_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::get_attribute_names_from_sptr(
            p_internal->cpp,
            p_attribute_names_out
        );
    )
}

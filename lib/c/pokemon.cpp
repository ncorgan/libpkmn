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

union pkmn_trainer_id pkmn_pokemon_default_trainer_id()
{
    return {pkmn::pokemon::DEFAULT_TRAINER_ID};
}

const char* pkmn_pokemon_default_trainer_name()
{
    return pkmn::pokemon::DEFAULT_TRAINER_NAME.c_str();
}

enum pkmn_error pkmn_pokemon_init(
    const char* species,
    const char* game,
    const char* form,
    int level,
    struct pkmn_pokemon* pokemon_out
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

enum pkmn_error pkmn_pokemon_init_from_file(
    const char* filepath,
    struct pkmn_pokemon* pokemon_out
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

enum pkmn_error pkmn_pokemon_free(
    struct pkmn_pokemon* pokemon_ptr
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);

    pkmn::c::free_pointer_and_set_to_null(&pokemon_ptr->p_species);
    pkmn::c::free_pointer_and_set_to_null(&pokemon_ptr->p_game);

    PKMN_CPP_TO_C(
        pkmn::c::delete_pointer_and_set_to_null(
            reinterpret_cast<pkmn_pokemon_internal_t**>(&pokemon_ptr->p_internal)
        );
    )
}

const char* pkmn_pokemon_strerror(
    struct pkmn_pokemon* pokemon_ptr
)
{
    if(!pokemon_ptr)
    {
        return nullptr;
    }

    try
    {
        pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
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

enum pkmn_error pkmn_pokemon_to_game(
    struct pkmn_pokemon* pokemon_ptr,
    const char* game,
    struct pkmn_pokemon* new_pokemon_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
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

enum pkmn_error pkmn_pokemon_export_to_file(
    struct pkmn_pokemon* pokemon_ptr,
    const char* filepath
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(filepath, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->export_to_file(filepath);
    )
}

enum pkmn_error pkmn_pokemon_get_form(
    struct pkmn_pokemon* pokemon_ptr,
    char* form_buffer,
    size_t form_buffer_len,
    size_t* form_length_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
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

enum pkmn_error pkmn_pokemon_set_form(
    struct pkmn_pokemon* pokemon_ptr,
    const char* form
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(form, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_form(form);
    )
}

enum pkmn_error pkmn_pokemon_is_egg(
    struct pkmn_pokemon* pokemon_ptr,
    bool* is_egg_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(is_egg_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *is_egg_out = internal_ptr->cpp->is_egg();
    )
}

enum pkmn_error pkmn_pokemon_set_is_egg(
    struct pkmn_pokemon* pokemon_ptr,
    bool is_egg
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_is_egg(is_egg);
    )
}

enum pkmn_error pkmn_pokemon_get_database_entry(
    struct pkmn_pokemon* pokemon_ptr,
    struct pkmn_database_pokemon_entry* database_entry_ptr
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(database_entry_ptr, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::pokemon_entry_cpp_to_c(
            internal_ptr->cpp->get_database_entry(),
            database_entry_ptr
        );
    )
}

enum pkmn_error pkmn_pokemon_get_condition(
    struct pkmn_pokemon* pokemon_ptr,
    enum pkmn_condition* condition_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(condition_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        std::string condition = internal_ptr->cpp->get_condition();

        const pkmn::c::condition_bimap_t& condition_bimap = pkmn::c::get_condition_bimap();
        BOOST_ASSERT(condition_bimap.left.count(condition) > 0);
        *condition_out = condition_bimap.left.at(condition);
    )
}

enum pkmn_error pkmn_pokemon_set_condition(
    struct pkmn_pokemon* pokemon_ptr,
    enum pkmn_condition condition
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        const pkmn::c::condition_bimap_t& condition_bimap = pkmn::c::get_condition_bimap();

        pkmn::enforce_value_in_map_keys(
            "Condition",
            condition,
            condition_bimap.right
        );

        internal_ptr->cpp->set_condition(
            condition_bimap.right.at(condition)
        );
    )
}

enum pkmn_error pkmn_pokemon_get_nickname(
    struct pkmn_pokemon* pokemon_ptr,
    char* nickname_out,
    size_t nickname_buffer_len,
    size_t* actual_nickname_len_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
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

enum pkmn_error pkmn_pokemon_set_nickname(
    struct pkmn_pokemon* pokemon_ptr,
    const char* nickname
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(nickname, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_nickname(nickname);
    )
}

enum pkmn_error pkmn_pokemon_get_gender(
    struct pkmn_pokemon* pokemon_ptr,
    enum pkmn_gender* gender_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(gender_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        std::string gender = internal_ptr->cpp->get_gender();

        const pkmn::c::gender_bimap_t& gender_bimap = pkmn::c::get_gender_bimap();
        BOOST_ASSERT(gender_bimap.left.count(gender) > 0);
        *gender_out = gender_bimap.left.at(gender);
    )
}

enum pkmn_error pkmn_pokemon_set_gender(
    struct pkmn_pokemon* pokemon_ptr,
    enum pkmn_gender gender
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        const pkmn::c::gender_bimap_t& gender_bimap = pkmn::c::get_gender_bimap();

        pkmn::enforce_value_in_map_keys(
            "Gender",
            gender,
            gender_bimap.right
        );

        internal_ptr->cpp->set_gender(
            gender_bimap.right.at(gender)
        );
    )
}

enum pkmn_error pkmn_pokemon_is_shiny(
    struct pkmn_pokemon* pokemon_ptr,
    bool* is_shiny_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(is_shiny_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *is_shiny_out = internal_ptr->cpp->is_shiny();
    )
}

enum pkmn_error pkmn_pokemon_set_is_shiny(
    struct pkmn_pokemon* pokemon_ptr,
    bool is_shiny
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_shininess(is_shiny);
    )
}

enum pkmn_error pkmn_pokemon_get_held_item(
    struct pkmn_pokemon* pokemon_ptr,
    char* held_item_out,
    size_t held_item_buffer_len,
    size_t* actual_held_item_len_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
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

enum pkmn_error pkmn_pokemon_set_held_item(
    struct pkmn_pokemon* pokemon_ptr,
    const char* held_item
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(held_item, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_held_item(held_item);
    )
}

enum pkmn_error pkmn_pokemon_get_pokerus_duration(
    struct pkmn_pokemon* pokemon_ptr,
    int* pokerus_duration_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(pokerus_duration_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *pokerus_duration_out = internal_ptr->cpp->get_pokerus_duration();
    )
}

enum pkmn_error pkmn_pokemon_set_pokerus_duration(
    struct pkmn_pokemon* pokemon_ptr,
    int pokerus_duration
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_pokerus_duration(pokerus_duration);
    )
}

enum pkmn_error pkmn_pokemon_get_original_trainer_info(
    struct pkmn_pokemon* pokemon_ptr,
    struct pkmn_trainer_info* original_trainer_info_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(original_trainer_info_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        // Use a separate struct so as to not have any side effects if there
        // are errors.
        struct pkmn_trainer_info trainer_info = {nullptr, {0}, PKMN_GENDER_MALE};

        pkmn::c::string_cpp_to_c_alloc(
            internal_ptr->cpp->get_original_trainer_name(),
            &trainer_info.p_name
        );
        trainer_info.id.id = internal_ptr->cpp->get_original_trainer_id();

        if(internal_ptr->generation >= 2)
        {
            std::string original_trainer_gender = internal_ptr->cpp->get_original_trainer_gender();

            const pkmn::c::gender_bimap_t& gender_bimap = pkmn::c::get_gender_bimap();
            BOOST_ASSERT(gender_bimap.left.count(original_trainer_gender) > 0);
            trainer_info.gender = gender_bimap.left.at(original_trainer_gender);
        }

        *original_trainer_info_out = std::move(trainer_info);
    )
}

enum pkmn_error pkmn_pokemon_set_original_trainer_name(
    struct pkmn_pokemon* pokemon_ptr,
    const char* original_trainer_name
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(original_trainer_name, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_original_trainer_name(original_trainer_name);
    )
}

enum pkmn_error pkmn_pokemon_set_original_trainer_public_id(
    struct pkmn_pokemon* pokemon_ptr,
    uint16_t original_trainer_public_id
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_original_trainer_public_id(
            original_trainer_public_id
        );
    )
}

enum pkmn_error pkmn_pokemon_set_original_trainer_secret_id(
    struct pkmn_pokemon* pokemon_ptr,
    uint16_t original_trainer_secret_id
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_original_trainer_secret_id(
            original_trainer_secret_id
        );
    )
}

enum pkmn_error pkmn_pokemon_set_original_trainer_id(
    struct pkmn_pokemon* pokemon_ptr,
    uint32_t original_trainer_id
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_original_trainer_id(original_trainer_id);
    )
}

enum pkmn_error pkmn_pokemon_set_original_trainer_gender(
    struct pkmn_pokemon* pokemon_ptr,
    enum pkmn_gender original_trainer_gender
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        const pkmn::c::gender_bimap_t& gender_bimap = pkmn::c::get_gender_bimap();

        pkmn::enforce_value_in_map_keys(
            "Original trainer gender",
            original_trainer_gender,
            gender_bimap.right
        );

        internal_ptr->cpp->set_original_trainer_gender(
            gender_bimap.right.at(original_trainer_gender)
        );
    )
}

enum pkmn_error pkmn_pokemon_get_current_trainer_friendship(
    struct pkmn_pokemon* pokemon_ptr,
    int* current_trainer_friendship_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(current_trainer_friendship_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *current_trainer_friendship_out = internal_ptr->cpp->get_current_trainer_friendship();
    )
}

enum pkmn_error pkmn_pokemon_set_current_trainer_friendship(
    struct pkmn_pokemon* pokemon_ptr,
    int current_trainer_friendship
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_current_trainer_friendship(current_trainer_friendship);
    )
}

enum pkmn_error pkmn_pokemon_get_ability(
    struct pkmn_pokemon* pokemon_ptr,
    char* ability_out,
    size_t ability_buffer_len,
    size_t* actual_ability_len_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
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

enum pkmn_error pkmn_pokemon_set_ability(
    struct pkmn_pokemon* pokemon_ptr,
    const char* ability
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(ability, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_ability(ability);
    )
}

enum pkmn_error pkmn_pokemon_get_ball(
    struct pkmn_pokemon* pokemon_ptr,
    char* ball_out,
    size_t ball_buffer_len,
    size_t* actual_ball_len_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
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

enum pkmn_error pkmn_pokemon_set_ball(
    struct pkmn_pokemon* pokemon_ptr,
    const char* ball
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(ball, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_ball(ball);
    )
}

enum pkmn_error pkmn_pokemon_get_level_met(
    struct pkmn_pokemon* pokemon_ptr,
    int* level_met_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(level_met_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *level_met_out = internal_ptr->cpp->get_level_met();
    )
}

enum pkmn_error pkmn_pokemon_set_level_met(
    struct pkmn_pokemon* pokemon_ptr,
    int level_met
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_level_met(level_met);
    )
}

enum pkmn_error pkmn_pokemon_get_location_met(
    struct pkmn_pokemon* pokemon_ptr,
    bool as_egg,
    char* location_met_out,
    size_t location_met_buffer_len,
    size_t* actual_location_met_len_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
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

enum pkmn_error pkmn_pokemon_set_location_met(
    struct pkmn_pokemon* pokemon_ptr,
    const char* location_met,
    bool as_egg
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(location_met, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_location_met(location_met, as_egg);
    )
}

enum pkmn_error pkmn_pokemon_get_original_game(
    struct pkmn_pokemon* pokemon_ptr,
    char* original_game_out,
    size_t original_game_buffer_len,
    size_t* actual_original_game_len_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
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

enum pkmn_error pkmn_pokemon_set_original_game(
    struct pkmn_pokemon* pokemon_ptr,
    const char* game
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(game, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_original_game(game);
    )
}

enum pkmn_error pkmn_pokemon_get_personality(
    struct pkmn_pokemon* pokemon_ptr,
    uint32_t* personality_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(personality_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *personality_out = internal_ptr->cpp->get_personality();
    )
}

enum pkmn_error pkmn_pokemon_set_personality(
    struct pkmn_pokemon* pokemon_ptr,
    uint32_t personality
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_personality(personality);
    )
}

enum pkmn_error pkmn_pokemon_get_experience(
    struct pkmn_pokemon* pokemon_ptr,
    int* experience_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(experience_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *experience_out = internal_ptr->cpp->get_experience();
    )
}

enum pkmn_error pkmn_pokemon_set_experience(
    struct pkmn_pokemon* pokemon_ptr,
    int experience
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_experience(experience);
    )
}

enum pkmn_error pkmn_pokemon_get_level(
    struct pkmn_pokemon* pokemon_ptr,
    int* level_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(level_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *level_out = internal_ptr->cpp->get_level();
    )
}

enum pkmn_error pkmn_pokemon_set_level(
    struct pkmn_pokemon* pokemon_ptr,
    int level
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_level(level);
    )
}

// Misc

enum pkmn_error pkmn_pokemon_get_markings(
    struct pkmn_pokemon* pokemon_ptr,
    bool* has_markings_buffer_out,
    size_t has_marking_buffer_size,
    size_t* actual_num_markings_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(has_markings_buffer_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        const pkmn::c::marking_bimap_t& marking_bimap = (internal_ptr->generation == 3) ?
            pkmn::c::get_gen3_marking_bimap() : pkmn::c::get_marking_bimap();

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

enum pkmn_error pkmn_pokemon_set_has_marking(
    struct pkmn_pokemon* pokemon_ptr,
    enum pkmn_marking marking,
    bool has_marking
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        const pkmn::c::marking_bimap_t& marking_bimap = (internal_ptr->generation == 3) ?
            pkmn::c::get_gen3_marking_bimap() : pkmn::c::get_marking_bimap();

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

enum pkmn_error pkmn_pokemon_has_ribbon(
    struct pkmn_pokemon* pokemon_ptr,
    const char* ribbon_name,
    bool* has_ribbon_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
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

enum pkmn_error pkmn_pokemon_set_has_ribbon(
    struct pkmn_pokemon* pokemon_ptr,
    const char* ribbon_name,
    bool has_ribbon
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
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

enum pkmn_error pkmn_pokemon_get_ribbon_names(
    struct pkmn_pokemon* pokemon_ptr,
    struct pkmn_string_list* ribbon_names_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(ribbon_names_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::string_map_keys_to_string_list(
            internal_ptr->cpp->get_ribbons(),
            ribbon_names_out
        );
    )
}

enum pkmn_error pkmn_pokemon_get_contest_stats(
    struct pkmn_pokemon* pokemon_ptr,
    int* contest_stats_buffer_out,
    size_t contest_stat_buffer_size,
    size_t* actual_num_contest_stats_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(contest_stats_buffer_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        copy_map_to_buffer(
            internal_ptr->cpp->get_contest_stats(),
            pkmn::c::get_contest_stat_bimap(),
            contest_stats_buffer_out,
            contest_stat_buffer_size,
            PKMN_NUM_CONTEST_STATS,
            actual_num_contest_stats_out
        );
    )
}

enum pkmn_error pkmn_pokemon_set_contest_stat(
    struct pkmn_pokemon* pokemon_ptr,
    enum pkmn_contest_stat contest_stat,
    int value
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        const pkmn::c::contest_stat_bimap_t& contest_stat_bimap = pkmn::c::get_contest_stat_bimap();

        pkmn::enforce_value_in_map_keys(
            "Contest stat",
            contest_stat,
            contest_stat_bimap.right
        );

        internal_ptr->cpp->set_contest_stat(
            contest_stat_bimap.right.at(contest_stat),
            value
        );
    )
}

// Moves

enum pkmn_error pkmn_pokemon_get_moves(
    struct pkmn_pokemon* pokemon_ptr,
    struct pkmn_move_slots* move_slots_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(move_slots_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::move_slots_cpp_to_c(
            internal_ptr->cpp->get_moves(),
            move_slots_out
        );
    )
}

enum pkmn_error pkmn_pokemon_set_move(
    struct pkmn_pokemon* pokemon_ptr,
    size_t index,
    const char* move
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(move, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_move(move, int(index));
    )
}

enum pkmn_error pkmn_pokemon_set_move_pp(
    struct pkmn_pokemon* pokemon_ptr,
    size_t index,
    int pp
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_move_pp(int(index), pp);
    )
}

// Stats

enum pkmn_error pkmn_pokemon_get_EVs(
    struct pkmn_pokemon* pokemon_ptr,
    int* EVs_buffer_out,
    size_t stat_buffer_size,
    size_t* actual_num_EVs_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(EVs_buffer_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        copy_map_to_buffer(
            internal_ptr->cpp->get_EVs(),
            pkmn::c::get_stat_bimap(),
            EVs_buffer_out,
            stat_buffer_size,
            PKMN_NUM_STATS,
            actual_num_EVs_out
        );
    )
}

enum pkmn_error pkmn_pokemon_set_EV(
    struct pkmn_pokemon* pokemon_ptr,
    enum pkmn_stat stat,
    int value
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        const pkmn::c::stat_bimap_t& stat_bimap = pkmn::c::get_stat_bimap();

        pkmn::enforce_value_in_map_keys(
            "Stat",
            stat,
            stat_bimap.right
        );

        internal_ptr->cpp->set_EV(
            stat_bimap.right.at(stat),
            value
        );
    )
}

enum pkmn_error pkmn_pokemon_get_IVs(
    struct pkmn_pokemon* pokemon_ptr,
    int* IVs_buffer_out,
    size_t stat_buffer_size,
    size_t* actual_num_IVs_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(IVs_buffer_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        copy_map_to_buffer(
            internal_ptr->cpp->get_IVs(),
            pkmn::c::get_stat_bimap(),
            IVs_buffer_out,
            stat_buffer_size,
            PKMN_NUM_STATS,
            actual_num_IVs_out
        );
    )
}

enum pkmn_error pkmn_pokemon_set_IV(
    struct pkmn_pokemon* pokemon_ptr,
    enum pkmn_stat stat,
    int value
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        const pkmn::c::stat_bimap_t& stat_bimap = pkmn::c::get_stat_bimap();

        pkmn::enforce_value_in_map_keys(
            "Stat",
            stat,
            stat_bimap.right
        );

        internal_ptr->cpp->set_IV(
            stat_bimap.right.at(stat),
            value
        );
    )
}

enum pkmn_error pkmn_pokemon_get_stats(
    struct pkmn_pokemon* pokemon_ptr,
    int* stats_buffer_out,
    size_t stat_buffer_size,
    size_t* actual_num_stats_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(stats_buffer_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        copy_map_to_buffer(
            internal_ptr->cpp->get_stats(),
            pkmn::c::get_stat_bimap(),
            stats_buffer_out,
            stat_buffer_size,
            PKMN_NUM_STATS,
            actual_num_stats_out
        );
    )
}

enum pkmn_error pkmn_pokemon_get_current_hp(
    struct pkmn_pokemon* pokemon_ptr,
    int* current_hp_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(current_hp_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *current_hp_out = internal_ptr->cpp->get_current_hp();
    )
}

enum pkmn_error pkmn_pokemon_set_current_hp(
    struct pkmn_pokemon* pokemon_ptr,
    int current_hp
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_current_hp(current_hp);
    )
}

enum pkmn_error pkmn_pokemon_get_icon_filepath(
    struct pkmn_pokemon* pokemon_ptr,
    char* icon_filepath_out,
    size_t icon_filepath_buffer_len,
    size_t* actual_icon_filepath_len_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
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

enum pkmn_error pkmn_pokemon_get_sprite_filepath(
    struct pkmn_pokemon* pokemon_ptr,
    char* sprite_filepath_out,
    size_t sprite_filepath_buffer_len,
    size_t* actual_sprite_filepath_len_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
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

enum pkmn_error pkmn_pokemon_get_numeric_attribute(
    struct pkmn_pokemon* pokemon_ptr,
    const char* attribute_name,
    int* value_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(attribute_name, internal_ptr);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(value_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *value_out = internal_ptr->cpp->get_numeric_attribute(attribute_name);
    )
}

enum pkmn_error pkmn_pokemon_set_numeric_attribute(
    struct pkmn_pokemon* pokemon_ptr,
    const char* attribute_name,
    int value
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(attribute_name, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_numeric_attribute(attribute_name, value);
    )
}

enum pkmn_error pkmn_pokemon_get_string_attribute(
    struct pkmn_pokemon* pokemon_ptr,
    const char* attribute_name,
    char* value_out,
    size_t value_buffer_len,
    size_t* actual_value_len_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
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

enum pkmn_error pkmn_pokemon_set_string_attribute(
    struct pkmn_pokemon* pokemon_ptr,
    const char* attribute_name,
    const char* value
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(attribute_name, internal_ptr);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(value, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_string_attribute(attribute_name, value);
    )
}

enum pkmn_error pkmn_pokemon_get_boolean_attribute(
    struct pkmn_pokemon* pokemon_ptr,
    const char* attribute_name,
    bool* value_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(attribute_name, internal_ptr);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(value_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *value_out = internal_ptr->cpp->get_boolean_attribute(attribute_name);
    )
}

enum pkmn_error pkmn_pokemon_set_boolean_attribute(
    struct pkmn_pokemon* pokemon_ptr,
    const char* attribute_name,
    bool value
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(attribute_name, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_boolean_attribute(attribute_name, value);
    )
}

enum pkmn_error pkmn_pokemon_get_attribute_names(
    struct pkmn_pokemon* pokemon_ptr,
    struct pkmn_attribute_names* attribute_names_out
)
{
    PKMN_CHECK_NULL_PARAM(pokemon_ptr);
    pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(pokemon_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(attribute_names_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::get_attribute_names_from_sptr(
            internal_ptr->cpp,
            attribute_names_out
        );
    )
}

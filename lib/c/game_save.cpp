/*
 * p_Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * p_Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * p_or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "enum_maps.hpp"
#include "error_internal.hpp"
#include "exception_internal.hpp"

#include <boost/assert.hpp>
#include <boost/thread/mutex.hpp>

#include <pkmn-c/game_save.h>

static const std::unordered_map<std::string, enum pkmn_game_save_type> GAME_SAVE_TYPES =
{
    {"None", PKMN_GAME_SAVE_TYPE_NONE},
    {"Red/Blue", PKMN_GAME_SAVE_TYPE_RED_BLUE},
    {"Yellow", PKMN_GAME_SAVE_TYPE_YELLOW},
    {"Gold/Silver", PKMN_GAME_SAVE_TYPE_GOLD_SILVER},
    {"Crystal", PKMN_GAME_SAVE_TYPE_CRYSTAL},
    {"Ruby/Sapphire", PKMN_GAME_SAVE_TYPE_RUBY_SAPPHIRE},
    {"Emerald", PKMN_GAME_SAVE_TYPE_EMERALD},
    {"FireRed/LeafGreen", PKMN_GAME_SAVE_TYPE_FIRERED_LEAFGREEN},
    {"Colosseum/XD", PKMN_GAME_SAVE_TYPE_COLOSSEUM_XD}
};

enum pkmn_error pkmn_game_save_detect_type(
    const char* p_filepath,
    enum pkmn_game_save_type* p_game_save_type_out
)
{
    PKMN_CHECK_NULL_PARAM(p_filepath);
    PKMN_CHECK_NULL_PARAM(p_game_save_type_out);

    PKMN_CPP_TO_C(
        std::string cpp_game_save_type = pkmn::game_save::detect_type(p_filepath);
        BOOST_ASSERT(GAME_SAVE_TYPES.count(cpp_game_save_type) > 0);

        *p_game_save_type_out = GAME_SAVE_TYPES.at(cpp_game_save_type);
    )
}

enum pkmn_error pkmn_game_save_init_from_file(
    const char* p_filepath,
    struct pkmn_game_save* p_game_save_out
)
{
    PKMN_CHECK_NULL_PARAM(p_filepath);
    PKMN_CHECK_NULL_PARAM(p_game_save_out);

    PKMN_CPP_TO_C(
        pkmn::game_save::sptr cpp = pkmn::game_save::from_file(p_filepath);

        pkmn::c::init_game_save(
            cpp,
            p_game_save_out
        );
    )
}

enum pkmn_error pkmn_game_save_free(
    struct pkmn_game_save* p_game_save
)
{
    PKMN_CHECK_NULL_PARAM(p_game_save);

    pkmn::c::free_pointer_and_set_to_null(&p_game_save->p_game);

    PKMN_CPP_TO_C(
        pkmn::c::delete_pointer_and_set_to_null(
            reinterpret_cast<pkmn_game_save_internal_t**>(&p_game_save->p_internal)
        );
    )
}

const char* pkmn_game_save_strerror(
    const struct pkmn_game_save* p_game_save
)
{
    if(!p_game_save)
    {
        return nullptr;
    }

    try
    {
        pkmn_game_save_internal_t* p_internal = GAME_SAVE_INTERNAL_RCAST(p_game_save->p_internal);
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

// Save file actions

PKMN_C_API enum pkmn_error pkmn_game_save_get_filepath(
    const struct pkmn_game_save* p_game_save,
    char* p_filepath_buffer,
    size_t filepath_buffer_length,
    size_t* p_actual_filepath_length_out
)
{
    PKMN_CHECK_NULL_PARAM(p_game_save);
    pkmn_game_save_internal_t* p_internal = GAME_SAVE_INTERNAL_RCAST(p_game_save->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_filepath_buffer, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::string_cpp_to_c(
            p_internal->cpp->get_filepath(),
            p_filepath_buffer,
            filepath_buffer_length,
            p_actual_filepath_length_out
        );
    )
}

enum pkmn_error pkmn_game_save_save(
    const struct pkmn_game_save* p_game_save
)
{
    PKMN_CHECK_NULL_PARAM(p_game_save);
    pkmn_game_save_internal_t* p_internal = GAME_SAVE_INTERNAL_RCAST(p_game_save->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->save();
    )
}

enum pkmn_error pkmn_game_save_save_as(
    const struct pkmn_game_save* p_game_save,
    const char* p_filepath
)
{
    PKMN_CHECK_NULL_PARAM(p_game_save);
    pkmn_game_save_internal_t* p_internal = GAME_SAVE_INTERNAL_RCAST(p_game_save->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_filepath, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->save_as(p_filepath);
    )
}

// Trainer info

enum pkmn_error pkmn_game_save_get_trainer_info(
    const struct pkmn_game_save* p_game_save,
    struct pkmn_trainer_info* p_trainer_info_out
)
{
    PKMN_CHECK_NULL_PARAM(p_game_save);
    pkmn_game_save_internal_t* p_internal = GAME_SAVE_INTERNAL_RCAST(p_game_save->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_trainer_info_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        // Put the values in a separate struct first so there are
        // no side effects if there's an error.
        struct pkmn_trainer_info trainer_info;

        pkmn::c::string_cpp_to_c_alloc(
            p_internal->cpp->get_trainer_name(),
            &trainer_info.p_name
        );
        trainer_info.id.id = p_internal->cpp->get_trainer_id();

        if(p_internal->generation >= 2)
        {
            std::string cpp_gender = p_internal->cpp->get_trainer_gender();

            const pkmn::c::gender_bimap_t& gender_bimap = pkmn::c::get_gender_bimap();
            BOOST_ASSERT(gender_bimap.left.count(cpp_gender) > 0);
            trainer_info.gender = gender_bimap.left.at(cpp_gender);
        }
        else
        {
            trainer_info.gender = PKMN_GENDER_MALE;
        }

        *p_trainer_info_out = std::move(trainer_info);
    )
}

enum pkmn_error pkmn_game_save_set_trainer_name(
    const struct pkmn_game_save* p_game_save,
    const char* p_trainer_name
)
{
    PKMN_CHECK_NULL_PARAM(p_game_save);
    pkmn_game_save_internal_t* p_internal = GAME_SAVE_INTERNAL_RCAST(p_game_save->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_trainer_name, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_trainer_name(p_trainer_name);
    )
}

enum pkmn_error pkmn_game_save_set_trainer_id(
    const struct pkmn_game_save* p_game_save,
    uint32_t trainer_id
)
{
    PKMN_CHECK_NULL_PARAM(p_game_save);
    pkmn_game_save_internal_t* p_internal = GAME_SAVE_INTERNAL_RCAST(p_game_save->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_trainer_id(trainer_id);
    )
}

PKMN_C_API enum pkmn_error pkmn_game_save_set_trainer_public_id(
    const struct pkmn_game_save* p_game_save,
    uint16_t trainer_public_id
)
{
    PKMN_CHECK_NULL_PARAM(p_game_save);
    pkmn_game_save_internal_t* p_internal = GAME_SAVE_INTERNAL_RCAST(p_game_save->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_trainer_public_id(trainer_public_id);
    )
}

PKMN_C_API enum pkmn_error pkmn_game_save_set_trainer_secret_id(
    const struct pkmn_game_save* p_game_save,
    uint16_t trainer_secret_id
)
{
    PKMN_CHECK_NULL_PARAM(p_game_save);
    pkmn_game_save_internal_t* p_internal = GAME_SAVE_INTERNAL_RCAST(p_game_save->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_trainer_secret_id(trainer_secret_id);
    )
}

enum pkmn_error pkmn_game_save_set_trainer_gender(
    const struct pkmn_game_save* p_game_save,
    enum pkmn_gender gender
)
{
    PKMN_CHECK_NULL_PARAM(p_game_save);
    pkmn_game_save_internal_t* p_internal = GAME_SAVE_INTERNAL_RCAST(p_game_save->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::enforce_value_in_vector(
            "Gender",
            gender,
            {PKMN_GENDER_MALE, PKMN_GENDER_FEMALE}
        );

        const pkmn::c::gender_bimap_t& gender_bimap = pkmn::c::get_gender_bimap();

        p_internal->cpp->set_trainer_gender(
            gender_bimap.right.at(gender)
        );
    )
}

// Other fields

enum pkmn_error pkmn_game_save_get_time_played(
    const struct pkmn_game_save* p_game_save,
    struct pkmn_time_duration* p_time_played_out
)
{
    PKMN_CHECK_NULL_PARAM(p_game_save);
    pkmn_game_save_internal_t* p_internal = GAME_SAVE_INTERNAL_RCAST(p_game_save->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_time_played_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::time_duration_cpp_to_c(
            p_internal->cpp->get_time_played(),
            p_time_played_out
        );
    )
}

enum pkmn_error pkmn_game_save_set_time_played(
    const struct pkmn_game_save* p_game_save,
    const struct pkmn_time_duration* p_time_played
)
{
    PKMN_CHECK_NULL_PARAM(p_game_save);
    pkmn_game_save_internal_t* p_internal = GAME_SAVE_INTERNAL_RCAST(p_game_save->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_time_played, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_time_played(
            pkmn::c::time_duration_c_to_cpp(p_time_played)
        );
    )
}

enum pkmn_error pkmn_game_save_get_rival_name(
    const struct pkmn_game_save* p_game_save,
    char* p_rival_name_buffer,
    size_t rival_name_buffer_length,
    size_t* p_actual_rival_name_length_out
)
{
    PKMN_CHECK_NULL_PARAM(p_game_save);
    pkmn_game_save_internal_t* p_internal = GAME_SAVE_INTERNAL_RCAST(p_game_save->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_rival_name_buffer, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::string_cpp_to_c(
            p_internal->cpp->get_rival_name(),
            p_rival_name_buffer,
            rival_name_buffer_length,
            p_actual_rival_name_length_out
        );
    )
}

enum pkmn_error pkmn_game_save_set_rival_name(
    const struct pkmn_game_save* p_game_save,
    const char* p_rival_name
)
{
    PKMN_CHECK_NULL_PARAM(p_game_save);
    pkmn_game_save_internal_t* p_internal = GAME_SAVE_INTERNAL_RCAST(p_game_save->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_rival_name, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_rival_name(p_rival_name);
    )
}

enum pkmn_error pkmn_game_save_get_money(
    const struct pkmn_game_save* p_game_save,
    int* p_money_out
)
{
    PKMN_CHECK_NULL_PARAM(p_game_save);
    pkmn_game_save_internal_t* p_internal = GAME_SAVE_INTERNAL_RCAST(p_game_save->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_money_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        *p_money_out = p_internal->cpp->get_money();
    )
}

enum pkmn_error pkmn_game_save_set_money(
    const struct pkmn_game_save* p_game_save,
    int money
)
{
    PKMN_CHECK_NULL_PARAM(p_game_save);
    pkmn_game_save_internal_t* p_internal = GAME_SAVE_INTERNAL_RCAST(p_game_save->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_money(money);
    )
}

// Pokémon and items

enum pkmn_error pkmn_game_save_get_pokedex(
    const struct pkmn_game_save* p_game_save,
    struct pkmn_pokedex* p_pokedex_out
)
{
    PKMN_CHECK_NULL_PARAM(p_game_save);
    pkmn_game_save_internal_t* p_internal = GAME_SAVE_INTERNAL_RCAST(p_game_save->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_pokedex_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::init_pokedex(
            p_internal->cpp->get_pokedex(),
            p_pokedex_out
        );
    )
}

enum pkmn_error pkmn_game_save_get_pokemon_party(
    const struct pkmn_game_save* p_game_save,
    struct pkmn_pokemon_party* p_pokemon_party_out
)
{
    PKMN_CHECK_NULL_PARAM(p_game_save);
    pkmn_game_save_internal_t* p_internal = GAME_SAVE_INTERNAL_RCAST(p_game_save->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_pokemon_party_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::init_pokemon_party(
            p_internal->cpp->get_pokemon_party(),
            p_pokemon_party_out
        );
    )
}

enum pkmn_error pkmn_game_save_get_pokemon_pc(
    const struct pkmn_game_save* p_game_save,
    struct pkmn_pokemon_pc* p_pokemon_pc_out
)
{
    PKMN_CHECK_NULL_PARAM(p_game_save);
    pkmn_game_save_internal_t* p_internal = GAME_SAVE_INTERNAL_RCAST(p_game_save->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_pokemon_pc_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::init_pokemon_pc(
            p_internal->cpp->get_pokemon_pc(),
            p_pokemon_pc_out
        );
    )
}

enum pkmn_error pkmn_game_save_get_item_bag(
    const struct pkmn_game_save* p_game_save,
    struct pkmn_item_bag* p_item_bag_out
)
{
    PKMN_CHECK_NULL_PARAM(p_game_save);
    pkmn_game_save_internal_t* p_internal = GAME_SAVE_INTERNAL_RCAST(p_game_save->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_item_bag_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::init_item_bag(
            p_internal->cpp->get_item_bag(),
            p_item_bag_out
        );
    )
}

enum pkmn_error pkmn_game_save_get_item_pc(
    const struct pkmn_game_save* p_game_save,
    struct pkmn_item_list* p_item_pc_out
)
{
    PKMN_CHECK_NULL_PARAM(p_game_save);
    pkmn_game_save_internal_t* p_internal = GAME_SAVE_INTERNAL_RCAST(p_game_save->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_item_pc_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::init_item_list(
            p_internal->cpp->get_item_pc(),
            p_item_pc_out
        );
    )
}

// Attributes

enum pkmn_error pkmn_game_save_get_numeric_attribute(
    const struct pkmn_game_save* p_game_save,
    const char* p_attribute_name,
    int* p_value_out
)
{
    PKMN_CHECK_NULL_PARAM(p_game_save);
    pkmn_game_save_internal_t* p_internal = GAME_SAVE_INTERNAL_RCAST(p_game_save->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_attribute_name, p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_value_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        *p_value_out = p_internal->cpp->get_numeric_attribute(p_attribute_name);
    )
}

enum pkmn_error pkmn_game_save_set_numeric_attribute(
    const struct pkmn_game_save* p_game_save,
    const char* p_attribute_name,
    int value
)
{
    PKMN_CHECK_NULL_PARAM(p_game_save);
    pkmn_game_save_internal_t* p_internal = GAME_SAVE_INTERNAL_RCAST(p_game_save->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_attribute_name, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_numeric_attribute(p_attribute_name, value);
    )
}

enum pkmn_error pkmn_game_save_get_string_attribute(
    const struct pkmn_game_save* p_game_save,
    const char* p_attribute_name,
    char* p_value_out,
    size_t value_buffer_len,
    size_t* p_actual_value_len_out
)
{
    PKMN_CHECK_NULL_PARAM(p_game_save);
    pkmn_game_save_internal_t* p_internal = GAME_SAVE_INTERNAL_RCAST(p_game_save->p_internal);
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

enum pkmn_error pkmn_game_save_set_string_attribute(
    const struct pkmn_game_save* p_game_save,
    const char* p_attribute_name,
    const char* p_value
)
{
    PKMN_CHECK_NULL_PARAM(p_game_save);
    pkmn_game_save_internal_t* p_internal = GAME_SAVE_INTERNAL_RCAST(p_game_save->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_attribute_name, p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_value, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_string_attribute(p_attribute_name, p_value);
    )
}

enum pkmn_error pkmn_game_save_get_boolean_attribute(
    const struct pkmn_game_save* p_game_save,
    const char* p_attribute_name,
    bool* p_value_out
)
{
    PKMN_CHECK_NULL_PARAM(p_game_save);
    pkmn_game_save_internal_t* p_internal = GAME_SAVE_INTERNAL_RCAST(p_game_save->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_attribute_name, p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_value_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        *p_value_out = p_internal->cpp->get_boolean_attribute(p_attribute_name);
    )
}

enum pkmn_error pkmn_game_save_set_boolean_attribute(
    const struct pkmn_game_save* p_game_save,
    const char* p_attribute_name,
    bool value
)
{
    PKMN_CHECK_NULL_PARAM(p_game_save);
    pkmn_game_save_internal_t* p_internal = GAME_SAVE_INTERNAL_RCAST(p_game_save->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_attribute_name, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        p_internal->cpp->set_boolean_attribute(p_attribute_name, value);
    )
}

enum pkmn_error pkmn_game_save_get_attribute_names(
    const struct pkmn_game_save* p_game_save,
    struct pkmn_attribute_names* p_attribute_names_out
)
{
    PKMN_CHECK_NULL_PARAM(p_game_save);
    pkmn_game_save_internal_t* p_internal = GAME_SAVE_INTERNAL_RCAST(p_game_save->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_attribute_names_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::c::get_attribute_names_from_sptr(
            p_internal->cpp,
            p_attribute_names_out
        );
    )
}

/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "common/attributes.hpp"
#include "common/misc.hpp"

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <boost/assert.hpp>
#include <boost/thread/mutex.hpp>

#include <pkmn-c/game_save.h>

#include <pkmn/game_save.hpp>

enum pkmn_error pkmn_game_save_detect_type(
    const char* p_filepath,
    enum pkmn_game_save_type* p_game_save_type_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_param(
                                p_filepath,
                                "p_filepath"
                            );
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_game_save_type_out,
                    "p_game_save_type_out"
                );
    }
    if(!error)
    {
        auto impl = [&]()
        {
            *p_game_save_type_out = static_cast<enum pkmn_game_save_type>(
                                        pkmn::game_save::detect_type(p_filepath)
                                    );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

enum pkmn_error pkmn_game_save_init_from_file(
    const char* p_filepath,
    struct pkmn_game_save* p_game_save_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_param(
                                p_filepath,
                                "p_filepath"
                            );
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_game_save_out,
                    "p_game_save_out"
                );
    }
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::game_save::sptr cpp = pkmn::game_save::from_file(p_filepath);

            pkmn::c::init_game_save(
                cpp,
                p_game_save_out
            );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

enum pkmn_error pkmn_game_save_free(
    struct pkmn_game_save* p_game_save
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_game_save,
                                "p_game_save"
                            );
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::c::delete_pointer_and_set_to_null(
                reinterpret_cast<pkmn::c::game_save_internal_t**>(
                    &p_game_save->p_internal
                )
            );

            std::memset(p_game_save, 0, sizeof(*p_game_save));
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

const char* pkmn_game_save_strerror(
    const struct pkmn_game_save* p_game_save
)
{
    return pkmn::c::strerror<struct pkmn_game_save, pkmn::game_save>(p_game_save);
}

// Save file actions

enum pkmn_error pkmn_game_save_get_filepath(
    const struct pkmn_game_save* p_game_save,
    char* p_filepath_buffer,
    size_t filepath_buffer_length,
    size_t* p_actual_filepath_length_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_game_save,
                                "p_game_save"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_game_save_internal_ptr(p_game_save);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_filepath_buffer,
                    "p_filepath_buffer",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::string_cpp_to_c(
                    p_internal->cpp->get_filepath(),
                    p_filepath_buffer,
                    filepath_buffer_length,
                    p_actual_filepath_length_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_game_save_save(
    const struct pkmn_game_save* p_game_save
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_game_save,
                                "p_game_save"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_game_save_internal_ptr(p_game_save);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->save();
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_game_save_save_as(
    const struct pkmn_game_save* p_game_save,
    const char* p_filepath
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_game_save,
                                "p_game_save"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_game_save_internal_ptr(p_game_save);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_filepath,
                    "p_filepath",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                p_internal->cpp->save_as(p_filepath);
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

// Trainer info

enum pkmn_error pkmn_game_save_get_trainer_info(
    const struct pkmn_game_save* p_game_save,
    struct pkmn_trainer_info* p_trainer_info_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_game_save,
                                "p_game_save"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_game_save_internal_ptr(p_game_save);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_trainer_info_out,
                    "p_trainer_info_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
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
                    trainer_info.gender = static_cast<enum pkmn_gender>(
                                              p_internal->cpp->get_trainer_gender()
                                          );
                }
                else
                {
                    trainer_info.gender = PKMN_GENDER_MALE;
                }

                *p_trainer_info_out = std::move(trainer_info);
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_game_save_set_trainer_name(
    const struct pkmn_game_save* p_game_save,
    const char* p_trainer_name
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_game_save,
                                "p_game_save"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_game_save_internal_ptr(p_game_save);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_trainer_name,
                    "p_trainer_name",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                p_internal->cpp->set_trainer_name(p_trainer_name);
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_game_save_set_trainer_id(
    const struct pkmn_game_save* p_game_save,
    uint32_t trainer_id
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_game_save,
                                "p_game_save"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_game_save_internal_ptr(p_game_save);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_trainer_id(trainer_id);
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_game_save_set_trainer_public_id(
    const struct pkmn_game_save* p_game_save,
    uint16_t trainer_public_id
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_game_save,
                                "p_game_save"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_game_save_internal_ptr(p_game_save);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_trainer_public_id(trainer_public_id);
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_game_save_set_trainer_secret_id(
    const struct pkmn_game_save* p_game_save,
    uint16_t trainer_secret_id
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_game_save,
                                "p_game_save"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_game_save_internal_ptr(p_game_save);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_trainer_secret_id(trainer_secret_id);
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_game_save_set_trainer_gender(
    const struct pkmn_game_save* p_game_save,
    enum pkmn_gender gender
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_game_save,
                                "p_game_save"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_game_save_internal_ptr(p_game_save);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_trainer_gender(
                static_cast<pkmn::e_gender>(gender)
            );
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

// Other fields

enum pkmn_error pkmn_game_save_get_time_played(
    const struct pkmn_game_save* p_game_save,
    struct pkmn_time_duration* p_time_played_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_game_save,
                                "p_game_save"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_game_save_internal_ptr(p_game_save);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_time_played_out,
                    "p_time_played_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::time_duration_cpp_to_c(
                    p_internal->cpp->get_time_played(),
                    p_time_played_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_game_save_set_time_played(
    const struct pkmn_game_save* p_game_save,
    const struct pkmn_time_duration* p_time_played
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_game_save,
                                "p_game_save"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_game_save_internal_ptr(p_game_save);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_time_played,
                    "p_time_played",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                p_internal->cpp->set_time_played(
                    pkmn::c::time_duration_c_to_cpp(p_time_played)
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_game_save_get_rival_name(
    const struct pkmn_game_save* p_game_save,
    char* p_rival_name_buffer,
    size_t rival_name_buffer_length,
    size_t* p_actual_rival_name_length_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_game_save,
                                "p_game_save"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_game_save_internal_ptr(p_game_save);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_rival_name_buffer,
                    "p_rival_name_buffer",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::string_cpp_to_c(
                    p_internal->cpp->get_rival_name(),
                    p_rival_name_buffer,
                    rival_name_buffer_length,
                    p_actual_rival_name_length_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_game_save_set_rival_name(
    const struct pkmn_game_save* p_game_save,
    const char* p_rival_name
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_game_save,
                                "p_game_save"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_game_save_internal_ptr(p_game_save);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_rival_name,
                    "p_rival_name",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                p_internal->cpp->set_rival_name(p_rival_name);
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_game_save_get_money(
    const struct pkmn_game_save* p_game_save,
    int* p_money_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_game_save,
                                "p_game_save"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_game_save_internal_ptr(p_game_save);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_money_out,
                    "p_money_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                *p_money_out = p_internal->cpp->get_money();
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_game_save_set_money(
    const struct pkmn_game_save* p_game_save,
    int money
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_game_save,
                                "p_game_save"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_game_save_internal_ptr(p_game_save);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_money(money);
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

// Other LibPKMN structs

enum pkmn_error pkmn_game_save_get_pokedex(
    const struct pkmn_game_save* p_game_save,
    struct pkmn_pokedex* p_pokedex_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_game_save,
                                "p_game_save"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_game_save_internal_ptr(p_game_save);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_pokedex_out,
                    "p_pokedex_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::init_pokedex(
                    p_internal->cpp->get_pokedex(),
                    p_pokedex_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_game_save_get_pokemon_party(
    const struct pkmn_game_save* p_game_save,
    struct pkmn_pokemon_party* p_pokemon_party_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_game_save,
                                "p_game_save"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_game_save_internal_ptr(p_game_save);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_pokemon_party_out,
                    "p_pokemon_party_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::init_pokemon_party(
                    p_internal->cpp->get_pokemon_party(),
                    p_pokemon_party_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_game_save_get_pokemon_pc(
    const struct pkmn_game_save* p_game_save,
    struct pkmn_pokemon_pc* p_pokemon_pc_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_game_save,
                                "p_game_save"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_game_save_internal_ptr(p_game_save);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_pokemon_pc_out,
                    "p_pokemon_pc_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::init_pokemon_pc(
                    p_internal->cpp->get_pokemon_pc(),
                    p_pokemon_pc_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_game_save_get_item_bag(
    const struct pkmn_game_save* p_game_save,
    struct pkmn_item_bag* p_item_bag_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_game_save,
                                "p_game_save"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_game_save_internal_ptr(p_game_save);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_item_bag_out,
                    "p_item_bag_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::init_item_bag(
                    p_internal->cpp->get_item_bag(),
                    p_item_bag_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_game_save_get_item_pc(
    const struct pkmn_game_save* p_game_save,
    struct pkmn_item_list* p_item_pc_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_game_save,
                                "p_game_save"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_game_save_internal_ptr(p_game_save);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_item_pc_out,
                    "p_item_pc_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::init_item_list(
                    p_internal->cpp->get_item_pc(),
                    p_item_pc_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

// Attributes

enum pkmn_error pkmn_game_save_get_numeric_attribute(
    const struct pkmn_game_save* p_game_save,
    const char* p_attribute_name,
    int* p_value_out
)
{
    return pkmn::c::get_numeric_attribute<struct pkmn_game_save, pkmn::game_save>(
               p_game_save,
               p_attribute_name,
               p_value_out,
               "p_game_save"
           );
}

enum pkmn_error pkmn_game_save_set_numeric_attribute(
    const struct pkmn_game_save* p_game_save,
    const char* p_attribute_name,
    int value
)
{
    return pkmn::c::set_numeric_attribute<struct pkmn_game_save, pkmn::game_save>(
               p_game_save,
               p_attribute_name,
               value,
               "p_game_save"
           );
}

enum pkmn_error pkmn_game_save_get_string_attribute(
    const struct pkmn_game_save* p_game_save,
    const char* p_attribute_name,
    char* p_value_out,
    size_t value_buffer_len,
    size_t* p_actual_value_len_out
)
{
    return pkmn::c::get_string_attribute<struct pkmn_game_save, pkmn::game_save>(
               p_game_save,
               p_attribute_name,
               p_value_out,
               value_buffer_len,
               p_actual_value_len_out,
               "p_game_save"
           );
}

enum pkmn_error pkmn_game_save_set_string_attribute(
    const struct pkmn_game_save* p_game_save,
    const char* p_attribute_name,
    const char* p_value
)
{
    return pkmn::c::set_string_attribute<struct pkmn_game_save, pkmn::game_save>(
               p_game_save,
               p_attribute_name,
               p_value,
               "p_game_save"
           );
}

enum pkmn_error pkmn_game_save_get_boolean_attribute(
    const struct pkmn_game_save* p_game_save,
    const char* p_attribute_name,
    bool* p_value_out
)
{
    return pkmn::c::get_boolean_attribute<struct pkmn_game_save, pkmn::game_save>(
               p_game_save,
               p_attribute_name,
               p_value_out,
               "p_game_save"
           );
}

enum pkmn_error pkmn_game_save_set_boolean_attribute(
    const struct pkmn_game_save* p_game_save,
    const char* p_attribute_name,
    bool value
)
{
    return pkmn::c::set_boolean_attribute<struct pkmn_game_save, pkmn::game_save>(
               p_game_save,
               p_attribute_name,
               value,
               "p_game_save"
           );
}

enum pkmn_error pkmn_game_save_get_attribute_names(
    const struct pkmn_game_save* p_game_save,
    struct pkmn_attribute_names* p_attribute_names_out
)
{
    return pkmn::c::get_attribute_names<struct pkmn_game_save, pkmn::game_save>(
               p_game_save,
               p_attribute_names_out,
               "p_game_save"
           );
}

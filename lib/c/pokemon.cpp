/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "enum_maps.hpp"
#include "error_internal.hpp"
#include "exception_internal.hpp"

#include "common/attributes.hpp"
#include "common/misc.hpp"

#include "private_exports.hpp"

#include <pkmn-c/pokemon.h>

#include <pkmn/exception.hpp>

#include <boost/assert.hpp>
#include <boost/thread/mutex.hpp>

#include <cstdio>
#include <type_traits>

union pkmn_trainer_id pkmn_pokemon_default_trainer_id()
{
    return {pkmn::pokemon::DEFAULT_TRAINER_ID};
}

const char* pkmn_pokemon_default_trainer_name()
{
    return pkmn::pokemon::DEFAULT_TRAINER_NAME.c_str();
}

enum pkmn_error pkmn_pokemon_init(
    enum pkmn_species species,
    enum pkmn_game game,
    const char* p_form,
    int level,
    struct pkmn_pokemon* p_pokemon_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_param(
                                p_form,
                                "p_form"
                            );
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_pokemon_out,
                    "p_pokemon_out"
                );
    }
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::pokemon::sptr cpp = pkmn::pokemon::make(
                                          static_cast<pkmn::e_species>(species),
                                          static_cast<pkmn::e_game>(game),
                                          p_form,
                                          level
                                      );

            pkmn::c::init_pokemon(
                cpp,
                p_pokemon_out
            );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_init_from_file(
    const char* p_filepath,
    struct pkmn_pokemon* p_pokemon_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_param(
                                p_filepath,
                                "p_filepath"
                            );
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_pokemon_out,
                    "p_pokemon_out"
                );
    }
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::pokemon::sptr cpp = pkmn::pokemon::from_file(p_filepath);

            pkmn::c::init_pokemon(
                cpp,
                p_pokemon_out
            );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_free(
    struct pkmn_pokemon* p_pokemon
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::c::delete_pointer_and_set_to_null(
                reinterpret_cast<pkmn::c::pokemon_internal_t**>(&p_pokemon->p_internal)
            );

            std::memset(p_pokemon, 0, sizeof(*p_pokemon));
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

const char* pkmn_pokemon_strerror(
    const struct pkmn_pokemon* p_pokemon
)
{
    return pkmn::c::strerror<struct pkmn_pokemon, pkmn::pokemon>(p_pokemon);
}

enum pkmn_error pkmn_pokemon_to_game(
    const struct pkmn_pokemon* p_pokemon,
    enum pkmn_game game,
    struct pkmn_pokemon* p_new_pokemon_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_new_pokemon_out,
                    "p_new_pokemon_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::pokemon::sptr new_pokemon_cpp = p_internal->cpp->to_game(
                                                          static_cast<pkmn::e_game>(game)
                                                      );

                pkmn::c::init_pokemon(
                    new_pokemon_cpp,
                    p_new_pokemon_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_export_to_file(
    const struct pkmn_pokemon* p_pokemon,
    const char* p_filepath
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
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
                p_internal->cpp->export_to_file(p_filepath);
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_get_form(
    const struct pkmn_pokemon* p_pokemon,
    char* p_form_buffer,
    size_t form_buffer_len,
    size_t* p_form_length_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_form_buffer,
                    "p_form_buffer",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::string_cpp_to_c(
                    p_internal->cpp->get_form(),
                    p_form_buffer,
                    form_buffer_len,
                    p_form_length_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_form(
    const struct pkmn_pokemon* p_pokemon,
    const char* p_form
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_form,
                    "p_form",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                p_internal->cpp->set_form(p_form);
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_is_egg(
    const struct pkmn_pokemon* p_pokemon,
    bool* p_is_egg_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_is_egg_out,
                    "p_is_egg_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                *p_is_egg_out = p_internal->cpp->is_egg();
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_is_egg(
    const struct pkmn_pokemon* p_pokemon,
    bool is_egg
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_is_egg(is_egg);
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_get_database_entry(
    const struct pkmn_pokemon* p_pokemon,
    struct pkmn_database_pokemon_entry* p_database_entry_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_database_entry_out,
                    "p_database_entry_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::pokemon_entry_cpp_to_c(
                    p_internal->cpp->get_database_entry(),
                    p_database_entry_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_get_condition(
    const struct pkmn_pokemon* p_pokemon,
    enum pkmn_condition* p_condition_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_condition_out,
                    "p_condition_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                *p_condition_out = static_cast<enum pkmn_condition>(
                                       p_internal->cpp->get_condition()
                                   );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_condition(
    const struct pkmn_pokemon* p_pokemon,
    enum pkmn_condition condition
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_condition(static_cast<pkmn::e_condition>(condition));
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_get_nickname(
    const struct pkmn_pokemon* p_pokemon,
    char* p_nickname_out,
    size_t nickname_buffer_len,
    size_t* p_actual_nickname_len_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_nickname_out,
                    "p_nickname_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::string_cpp_to_c(
                    p_internal->cpp->get_nickname(),
                    p_nickname_out,
                    nickname_buffer_len,
                    p_actual_nickname_len_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_nickname(
    const struct pkmn_pokemon* p_pokemon,
    const char* p_nickname
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_nickname,
                    "p_nickname",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                p_internal->cpp->set_nickname(p_nickname);
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_get_gender(
    const struct pkmn_pokemon* p_pokemon,
    enum pkmn_gender* p_gender_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_gender_out,
                    "p_gender_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::e_gender gender = p_internal->cpp->get_gender();

                *p_gender_out = static_cast<enum pkmn_gender>(gender);
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_gender(
    const struct pkmn_pokemon* p_pokemon,
    enum pkmn_gender gender
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_gender(
                static_cast<pkmn::e_gender>(gender)
            );
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_is_shiny(
    const struct pkmn_pokemon* p_pokemon,
    bool* p_is_shiny_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_is_shiny_out,
                    "p_is_shiny_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                *p_is_shiny_out = p_internal->cpp->is_shiny();
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_is_shiny(
    const struct pkmn_pokemon* p_pokemon,
    bool is_shiny
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_shininess(is_shiny);
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_get_held_item(
    const struct pkmn_pokemon* p_pokemon,
    enum pkmn_item* p_held_item_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_held_item_out,
                    "p_held_item_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                *p_held_item_out = static_cast<enum pkmn_item>(
                                       p_internal->cpp->get_held_item()
                                   );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_held_item(
    const struct pkmn_pokemon* p_pokemon,
    enum pkmn_item item
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_held_item(
                static_cast<pkmn::e_item>(item)
            );
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_get_nature(
    const struct pkmn_pokemon* p_pokemon,
    enum pkmn_nature* p_nature_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_nature_out,
                    "p_nature_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                *p_nature_out = static_cast<enum pkmn_nature>(
                                       p_internal->cpp->get_nature()
                                   );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_nature(
    const struct pkmn_pokemon* p_pokemon,
    enum pkmn_nature nature
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_nature(
                static_cast<pkmn::e_nature>(nature)
            );
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_get_pokerus_duration(
    const struct pkmn_pokemon* p_pokemon,
    int* p_pokerus_duration_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_pokerus_duration_out,
                    "p_pokerus_duration_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                *p_pokerus_duration_out = p_internal->cpp->get_pokerus_duration();
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_pokerus_duration(
    const struct pkmn_pokemon* p_pokemon,
    int pokerus_duration
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_pokerus_duration(pokerus_duration);
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_get_original_trainer_info(
    const struct pkmn_pokemon* p_pokemon,
    struct pkmn_trainer_info* p_original_trainer_info_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_original_trainer_info_out,
                    "p_original_trainer_info_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
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
                    pkmn::e_gender original_trainer_gender = p_internal->cpp->get_original_trainer_gender();

                    trainer_info.gender = static_cast<enum pkmn_gender>(original_trainer_gender);
                }

                *p_original_trainer_info_out = std::move(trainer_info);
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_original_trainer_name(
    const struct pkmn_pokemon* p_pokemon,
    const char* p_original_trainer_name
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_original_trainer_name,
                    "p_original_trainer_name",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                p_internal->cpp->set_original_trainer_name(p_original_trainer_name);
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_original_trainer_public_id(
    const struct pkmn_pokemon* p_pokemon,
    uint16_t original_trainer_public_id
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_original_trainer_public_id(
                original_trainer_public_id
            );
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_original_trainer_secret_id(
    const struct pkmn_pokemon* p_pokemon,
    uint16_t original_trainer_secret_id
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_original_trainer_secret_id(
                original_trainer_secret_id
            );
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_original_trainer_id(
    const struct pkmn_pokemon* p_pokemon,
    uint32_t original_trainer_id
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_original_trainer_id(
                original_trainer_id
            );
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_original_trainer_gender(
    const struct pkmn_pokemon* p_pokemon,
    enum pkmn_gender original_trainer_gender
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_original_trainer_gender(
                static_cast<pkmn::e_gender>(original_trainer_gender)
            );
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_get_language(
    const struct pkmn_pokemon* p_pokemon,
    enum pkmn_language* p_language_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            *p_language_out = static_cast<enum pkmn_language>(
                                  p_internal->cpp->get_language()
                              );
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_language(
    const struct pkmn_pokemon* p_pokemon,
    enum pkmn_language language
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_language(
                static_cast<pkmn::e_language>(language)
            );
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_get_current_trainer_friendship(
    const struct pkmn_pokemon* p_pokemon,
    int* p_current_trainer_friendship_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_current_trainer_friendship_out,
                    "p_current_trainer_friendship_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                *p_current_trainer_friendship_out = p_internal->cpp->get_current_trainer_friendship();
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_current_trainer_friendship(
    const struct pkmn_pokemon* p_pokemon,
    int current_trainer_friendship
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_current_trainer_friendship(current_trainer_friendship);
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_get_ability(
    const struct pkmn_pokemon* p_pokemon,
    enum pkmn_ability* p_ability_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_ability_out,
                    "p_ability_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                *p_ability_out = static_cast<enum pkmn_ability>(
                                     p_internal->cpp->get_ability()
                                 );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_ability(
    const struct pkmn_pokemon* p_pokemon,
    enum pkmn_ability ability
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_ability(static_cast<pkmn::e_ability>(ability));
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_get_ball(
    const struct pkmn_pokemon* p_pokemon,
    enum pkmn_ball* p_ball_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_ball_out,
                    "p_ball_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                *p_ball_out = static_cast<enum pkmn_ball>(p_internal->cpp->get_ball());
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_ball(
    const struct pkmn_pokemon* p_pokemon,
    enum pkmn_ball ball
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_ball(static_cast<pkmn::e_ball>(ball));
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_get_level_met(
    const struct pkmn_pokemon* p_pokemon,
    int* p_level_met_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_level_met_out,
                    "p_level_met_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                *p_level_met_out = p_internal->cpp->get_level_met();
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_level_met(
    const struct pkmn_pokemon* p_pokemon,
    int level_met
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_level_met(level_met);
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_get_location_met(
    const struct pkmn_pokemon* p_pokemon,
    bool as_egg,
    char* p_location_met_out,
    size_t location_met_buffer_len,
    size_t* p_actual_location_met_len_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_location_met_out,
                    "p_location_met_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::string_cpp_to_c(
                    p_internal->cpp->get_location_met(as_egg),
                    p_location_met_out,
                    location_met_buffer_len,
                    p_actual_location_met_len_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_location_met(
    const struct pkmn_pokemon* p_pokemon,
    const char* p_location_met,
    bool as_egg
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_location_met,
                    "p_location_met",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                p_internal->cpp->set_location_met(
                    p_location_met,
                    as_egg
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_get_original_game(
    const struct pkmn_pokemon* p_pokemon,
    enum pkmn_game* p_original_game_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_original_game_out,
                    "p_original_game_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::e_game original_game_cpp = p_internal->cpp->get_original_game();

                *p_original_game_out = static_cast<enum pkmn_game>(original_game_cpp);
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_original_game(
    const struct pkmn_pokemon* p_pokemon,
    enum pkmn_game original_game
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_original_game(static_cast<pkmn::e_game>(original_game));
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_get_personality(
    const struct pkmn_pokemon* p_pokemon,
    uint32_t* p_personality_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_personality_out,
                    "p_personality_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                *p_personality_out = p_internal->cpp->get_personality();
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_personality(
    const struct pkmn_pokemon* p_pokemon,
    uint32_t personality
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_personality(personality);
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_get_experience(
    const struct pkmn_pokemon* p_pokemon,
    int* p_experience_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_experience_out,
                    "p_experience_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                *p_experience_out = p_internal->cpp->get_experience();
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_experience(
    const struct pkmn_pokemon* p_pokemon,
    int experience
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_experience(experience);
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_get_level(
    const struct pkmn_pokemon* p_pokemon,
    int* p_level_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_level_out,
                    "p_level_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                *p_level_out = p_internal->cpp->get_level();
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_level(
    const struct pkmn_pokemon* p_pokemon,
    int level
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_level(level);
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

// Misc

enum pkmn_error pkmn_pokemon_get_markings(
    const struct pkmn_pokemon* p_pokemon,
    struct pkmn_marking_enum_map* p_markings_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_markings_out,
                    "p_markings_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::marking_enum_map_cpp_to_c(
                    p_internal->cpp->get_markings(),
                    p_markings_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_has_marking(
    const struct pkmn_pokemon* p_pokemon,
    enum pkmn_marking marking,
    bool has_marking
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_marking(
                static_cast<pkmn::e_marking>(marking),
                has_marking
            );
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_has_ribbon(
    const struct pkmn_pokemon* p_pokemon,
    const char* p_ribbon_name,
    bool* p_has_ribbon_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_ribbon_name,
                    "p_ribbon_name",
                    p_internal
                );
        if(!error)
        {
            error = pkmn::c::check_for_null_param(
                        p_has_ribbon_out,
                        "p_has_ribbon_out",
                        p_internal
                    );
        }
        if(!error)
        {
            auto impl = [&]()
            {
                const std::map<std::string, bool>& cpp_ribbons = p_internal->cpp->get_ribbons();

                pkmn::enforce_value_in_map_keys(
                    "Ribbon name",
                    p_ribbon_name,
                    cpp_ribbons
                );

                *p_has_ribbon_out = cpp_ribbons.at(p_ribbon_name);
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_has_ribbon(
    const struct pkmn_pokemon* p_pokemon,
    const char* p_ribbon_name,
    bool has_ribbon
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_ribbon_name,
                    "p_ribbon_name",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                const std::map<std::string, bool>& cpp_ribbons = p_internal->cpp->get_ribbons();

                pkmn::enforce_value_in_map_keys(
                    "Ribbon name",
                    p_ribbon_name,
                    cpp_ribbons
                );

                p_internal->cpp->set_ribbon(p_ribbon_name, has_ribbon);
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_get_ribbon_names(
    const struct pkmn_pokemon* p_pokemon,
    struct pkmn_string_list* p_ribbon_names_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_ribbon_names_out,
                    "p_ribbon_names_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::string_map_keys_to_string_list(
                    p_internal->cpp->get_ribbons(),
                    p_ribbon_names_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_get_contest_stats(
    const struct pkmn_pokemon* p_pokemon,
    struct pkmn_contest_stat_enum_map* p_contest_stats_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_contest_stats_out,
                    "p_contest_stats_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::contest_stat_enum_map_cpp_to_c(
                    p_internal->cpp->get_contest_stats(),
                    p_contest_stats_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_contest_stat(
    const struct pkmn_pokemon* p_pokemon,
    enum pkmn_contest_stat contest_stat,
    int value
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_contest_stat(
                static_cast<pkmn::e_contest_stat>(contest_stat),
                value
            );
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

// Moves

enum pkmn_error pkmn_pokemon_get_moves(
    const struct pkmn_pokemon* p_pokemon,
    struct pkmn_move_slots* p_move_slots_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_move_slots_out,
                    "p_move_slots_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::move_slots_cpp_to_c(
                    p_internal->cpp->get_moves(),
                    p_move_slots_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_move(
    const struct pkmn_pokemon* p_pokemon,
    size_t index,
    enum pkmn_move move
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_move(
                static_cast<pkmn::e_move>(move),
                static_cast<int>(index)
            );
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_move_pp(
    const struct pkmn_pokemon* p_pokemon,
    size_t index,
    int pp
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_move_pp(
                static_cast<int>(index),
                pp
            );
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

// Stats

enum pkmn_error pkmn_pokemon_get_EVs(
    const struct pkmn_pokemon* p_pokemon,
    struct pkmn_stat_enum_map* p_EVs_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_EVs_out,
                    "p_EVs_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::stat_enum_map_cpp_to_c(
                    p_internal->cpp->get_EVs(),
                    p_EVs_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_EV(
    const struct pkmn_pokemon* p_pokemon,
    enum pkmn_stat stat,
    int value
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
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
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_get_IVs(
    const struct pkmn_pokemon* p_pokemon,
    struct pkmn_stat_enum_map* p_IVs_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_IVs_out,
                    "p_IVs_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::stat_enum_map_cpp_to_c(
                    p_internal->cpp->get_IVs(),
                    p_IVs_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_IV(
    const struct pkmn_pokemon* p_pokemon,
    enum pkmn_stat stat,
    int value
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
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
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_get_stats(
    const struct pkmn_pokemon* p_pokemon,
    struct pkmn_stat_enum_map* p_stats_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_stats_out,
                    "p_stats_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::stat_enum_map_cpp_to_c(
                    p_internal->cpp->get_stats(),
                    p_stats_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_get_current_hp(
    const struct pkmn_pokemon* p_pokemon,
    int* p_current_hp_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_current_hp_out,
                    "p_current_hp_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                *p_current_hp_out = p_internal->cpp->get_current_hp();
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_set_current_hp(
    const struct pkmn_pokemon* p_pokemon,
    int current_hp
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_current_hp(current_hp);
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_pokemon_get_icon_filepath(
    const struct pkmn_pokemon* p_pokemon,
    char* p_icon_filepath_out,
    size_t icon_filepath_buffer_len,
    size_t* p_actual_icon_filepath_len_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_icon_filepath_out,
                    "p_icon_filepath_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::string_cpp_to_c(
                    p_internal->cpp->get_icon_filepath(),
                    p_icon_filepath_out,
                    icon_filepath_buffer_len,
                    p_actual_icon_filepath_len_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_pokemon_get_sprite_filepath(
    const struct pkmn_pokemon* p_pokemon,
    char* p_sprite_filepath_out,
    size_t sprite_filepath_buffer_len,
    size_t* p_actual_sprite_filepath_len_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_pokemon,
                                "p_pokemon"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_pokemon_internal_ptr(p_pokemon);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_sprite_filepath_out,
                    "p_sprite_filepath_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::string_cpp_to_c(
                    p_internal->cpp->get_sprite_filepath(),
                    p_sprite_filepath_out,
                    sprite_filepath_buffer_len,
                    p_actual_sprite_filepath_len_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

// Attributes

enum pkmn_error pkmn_pokemon_get_numeric_attribute(
    const struct pkmn_pokemon* p_pokemon,
    const char* p_attribute_name,
    int* p_value_out
)
{
    return pkmn::c::get_numeric_attribute<struct pkmn_pokemon, pkmn::pokemon>(
               p_pokemon,
               p_attribute_name,
               p_value_out,
               "p_pokemon"
           );
}

enum pkmn_error pkmn_pokemon_set_numeric_attribute(
    const struct pkmn_pokemon* p_pokemon,
    const char* p_attribute_name,
    int value
)
{
    return pkmn::c::set_numeric_attribute<struct pkmn_pokemon, pkmn::pokemon>(
               p_pokemon,
               p_attribute_name,
               value,
               "p_pokemon"
           );
}

enum pkmn_error pkmn_pokemon_get_string_attribute(
    const struct pkmn_pokemon* p_pokemon,
    const char* p_attribute_name,
    char* p_value_out,
    size_t value_buffer_len,
    size_t* p_actual_value_len_out
)
{
    return pkmn::c::get_string_attribute<struct pkmn_pokemon, pkmn::pokemon>(
               p_pokemon,
               p_attribute_name,
               p_value_out,
               value_buffer_len,
               p_actual_value_len_out,
               "p_pokemon"
           );
}

enum pkmn_error pkmn_pokemon_set_string_attribute(
    const struct pkmn_pokemon* p_pokemon,
    const char* p_attribute_name,
    const char* p_value
)
{
    return pkmn::c::set_string_attribute<struct pkmn_pokemon, pkmn::pokemon>(
               p_pokemon,
               p_attribute_name,
               p_value,
               "p_pokemon"
           );
}

enum pkmn_error pkmn_pokemon_get_boolean_attribute(
    const struct pkmn_pokemon* p_pokemon,
    const char* p_attribute_name,
    bool* p_value_out
)
{
    return pkmn::c::get_boolean_attribute<struct pkmn_pokemon, pkmn::pokemon>(
               p_pokemon,
               p_attribute_name,
               p_value_out,
               "p_pokemon"
           );
}

enum pkmn_error pkmn_pokemon_set_boolean_attribute(
    const struct pkmn_pokemon* p_pokemon,
    const char* p_attribute_name,
    bool value
)
{
    return pkmn::c::set_boolean_attribute<struct pkmn_pokemon, pkmn::pokemon>(
               p_pokemon,
               p_attribute_name,
               value,
               "p_pokemon"
           );
}

enum pkmn_error pkmn_pokemon_get_attribute_names(
    const struct pkmn_pokemon* p_pokemon,
    struct pkmn_attribute_names* p_attribute_names_out
)
{
    return pkmn::c::get_attribute_names<struct pkmn_pokemon, pkmn::pokemon>(
               p_pokemon,
               p_attribute_names_out,
               "p_pokemon"
           );
}

/*
 * p_Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * p_Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * p_or copy at http://opensource.org/licenses/MIT)
 */

#include "common/item_container.hpp"
#include "common/misc.hpp"

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <boost/assert.hpp>
#include <boost/thread/mutex.hpp>

#include <pkmn-c/item_list.h>

enum pkmn_error pkmn_item_list_init(
    const char* p_name,
    enum pkmn_game game,
    struct pkmn_item_list* p_item_list_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_param(
                                p_name,
                                "p_name"
                            );
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_item_list_out,
                    "p_item_list_out"
                );
    }
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::item_list::sptr cpp = pkmn::item_list::make(
                                            p_name,
                                            static_cast<pkmn::e_game>(game)
                                        );

            pkmn::c::init_item_list(
                cpp,
                p_item_list_out
            );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

enum pkmn_error pkmn_item_list_free(
    struct pkmn_item_list* p_item_list
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_item_list,
                                "p_item_list"
                            );
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::c::free_pointer_and_set_to_null(&p_item_list->p_name);
            pkmn::c::delete_pointer_and_set_to_null(
                reinterpret_cast<pkmn::c::item_list_internal_t**>(&p_item_list->p_internal)
            );

            std::memset(p_item_list, 0, sizeof(*p_item_list));
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

const char* pkmn_item_list_strerror(
    const struct pkmn_item_list* p_item_list
)
{
    return pkmn::c::strerror<struct pkmn_item_list, pkmn::item_list>(p_item_list);
}

enum pkmn_error pkmn_item_list_get_num_items(
    const struct pkmn_item_list* p_item_list,
    size_t* p_num_items_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_item_list,
                                "p_item_list"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_item_list_internal_ptr(p_item_list);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_num_items_out,
                    "p_num_items_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                *p_num_items_out = p_internal->cpp->get_num_items();
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_item_list_at(
    const struct pkmn_item_list* p_item_list,
    size_t position,
    struct pkmn_item_slot* p_item_slot_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_item_list,
                                "p_item_list"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_item_list_internal_ptr(p_item_list);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_item_slot_out,
                    "p_item_slot_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::item_slot_cpp_to_c(
                    p_internal->cpp->at(static_cast<int>(position)),
                    p_item_slot_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_item_list_add(
    const struct pkmn_item_list* p_item_list,
    enum pkmn_item item,
    size_t amount
)
{
    return pkmn::c::add_item<struct pkmn_item_list, pkmn::item_list>(
               p_item_list,
               item,
               amount,
               "p_item_list"
           );
}

enum pkmn_error pkmn_item_list_remove(
    const struct pkmn_item_list* p_item_list,
    enum pkmn_item item,
    size_t amount
)
{
    return pkmn::c::remove_item<struct pkmn_item_list, pkmn::item_list>(
               p_item_list,
               item,
               amount,
               "p_item_list"
           );
}

enum pkmn_error pkmn_item_list_move(
    const struct pkmn_item_list* p_item_list,
    size_t old_position,
    size_t new_position
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_item_list,
                                "p_item_list"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_item_list_internal_ptr(p_item_list);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->move(
                static_cast<int>(old_position),
                static_cast<int>(new_position)
            );
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_item_list_set_item(
    const struct pkmn_item_list* p_item_list,
    size_t position,
    enum pkmn_item item,
    size_t amount
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_item_list,
                                "p_item_list"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_item_list_internal_ptr(p_item_list);
        BOOST_ASSERT(p_internal != nullptr);

        auto impl = [&]()
        {
            p_internal->cpp->set_item(
                static_cast<int>(position),
                static_cast<pkmn::e_item>(item),
                static_cast<int>(amount)
            );
        };

        error = pkmn::c::handle_exceptions(impl, p_internal);
    }

    return error;
}

enum pkmn_error pkmn_item_list_get_valid_items(
    const struct pkmn_item_list* p_item_list,
    struct pkmn_item_enum_list* p_valid_items_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_item_list,
                                "p_item_list"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_item_list_internal_ptr(p_item_list);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_valid_items_out,
                    "p_valid_items_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::item_enum_list_cpp_to_c(
                    p_internal->cpp->get_valid_items(),
                    p_valid_items_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_item_list_get_valid_item_names(
    const struct pkmn_item_list* p_item_list,
    struct pkmn_string_list* p_valid_item_names_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_item_list,
                                "p_item_list"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_item_list_internal_ptr(p_item_list);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_valid_item_names_out,
                    "p_valid_item_names_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::string_list_cpp_to_c(
                    p_internal->cpp->get_valid_item_names(),
                    p_valid_item_names_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_item_list_as_list(
    const struct pkmn_item_list* p_item_list,
    struct pkmn_item_slots* p_item_slots_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_item_list,
                                "p_item_list"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_item_list_internal_ptr(p_item_list);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_item_slots_out,
                    "p_item_slots_out",
                    p_internal
                );
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::item_slots_cpp_to_c(
                    p_internal->cpp->as_vector(),
                    p_item_slots_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

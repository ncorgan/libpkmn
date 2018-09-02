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

#include <boost/thread/mutex.hpp>

#include <pkmn-c/item_bag.h>

#include <cstdio>

enum pkmn_error pkmn_item_bag_init(
    enum pkmn_game game,
    struct pkmn_item_bag* p_item_bag_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_param(
                                p_item_bag_out,
                                "p_item_bag_out"
                            );
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::item_bag::sptr cpp = pkmn::item_bag::make(
                                           static_cast<pkmn::e_game>(game)
                                       );

            pkmn::c::init_item_bag(
                cpp,
                p_item_bag_out
            );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

enum pkmn_error pkmn_item_bag_free(
    struct pkmn_item_bag* p_item_bag
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_item_bag,
                                "p_item_bag"
                            );
    if(!error)
    {
        auto impl = [&]()
        {
            error = pkmn_string_list_free(&p_item_bag->pocket_names);
            if(!error)
            {
                pkmn::c::delete_pointer_and_set_to_null(
                    reinterpret_cast<pkmn::c::item_bag_internal_t**>(
                        &p_item_bag->p_internal
                    )
                );

                std::memset(p_item_bag, 0, sizeof(*p_item_bag));
            }
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}

const char* pkmn_item_bag_strerror(
    const struct pkmn_item_bag* p_item_bag
)
{
    return pkmn::c::strerror<struct pkmn_item_bag, pkmn::item_bag>(p_item_bag);
}

enum pkmn_error pkmn_item_bag_get_pocket(
    const struct pkmn_item_bag* p_item_bag,
    const char* p_pocket_name,
    struct pkmn_item_list* p_item_list_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_wrapper_param(
                                p_item_bag,
                                "p_item_bag"
                            );
    if(!error)
    {
        auto* p_internal = pkmn::c::get_item_bag_internal_ptr(p_item_bag);
        BOOST_ASSERT(p_internal != nullptr);

        error = pkmn::c::check_for_null_param(
                    p_pocket_name,
                    "p_pocket_name",
                    p_internal
                );
        if(!error)
        {
            error = pkmn::c::check_for_null_param(
                        p_item_list_out,
                        "p_item_list_out",
                        p_internal
                    );
        }
        if(!error)
        {
            auto impl = [&]()
            {
                pkmn::c::init_item_list(
                    p_internal->cpp->get_pocket(p_pocket_name),
                    p_item_list_out
                );
            };

            error = pkmn::c::handle_exceptions(impl, p_internal);
        }
    }

    return error;
}

enum pkmn_error pkmn_item_bag_add(
    const struct pkmn_item_bag* p_item_bag,
    enum pkmn_item item,
    size_t amount
)
{
    return pkmn::c::add_item<struct pkmn_item_bag, pkmn::item_bag>(
               p_item_bag,
               item,
               amount,
               "p_item_bag"
           );
}

enum pkmn_error pkmn_item_bag_remove(
    const struct pkmn_item_bag* p_item_bag,
    enum pkmn_item item,
    size_t amount
)
{
    return pkmn::c::remove_item<struct pkmn_item_bag, pkmn::item_bag>(
               p_item_bag,
               item,
               amount,
               "p_item_bag"
           );
}

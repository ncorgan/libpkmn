/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMNTEST_MISC_COMPARISON_HPP
#define PKMNTEST_MISC_COMPARISON_HPP

#include <pkmntest/config.hpp>

#include <pkmn/types/class_with_attributes.hpp>

#include <memory>

namespace pkmntest
{
    PKMNTEST_API void compare_attributes(
        const std::shared_ptr<pkmn::class_with_attributes>& class1,
        const std::shared_ptr<pkmn::class_with_attributes>& class2
    );
}

#endif /* PKMNTEST_MISC_COMPARISON_HPP */

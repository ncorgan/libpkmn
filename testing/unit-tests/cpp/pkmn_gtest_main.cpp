/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "private_exports.hpp"

#include <gtest/gtest.h>

GTEST_API_ int main(int argc, char** argv)
{
    // So the first initialization doesn't get included in the first test time.
    pkmn::priv::initialize_database_connection();

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

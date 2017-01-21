#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

#
# Python tests
#
SET(python_tests
    python_calculations_test
    python_database_entry_test
    python_items_test
    python_lists_test
    python_paths_test
    python_pokemon_test
    python_pokemon_pc_test
)

FOREACH(test ${python_tests})
    PKMN_ADD_PYTHON_TEST(${test})
ENDFOREACH(test ${python_tests})

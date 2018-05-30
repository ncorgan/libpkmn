#
# Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

#
# Python tests
#
SET(python_tests
    python_buildinfo_test
    python_calculations_test
    python_database_entry_test
    python_daycare_test
    python_game_save_test
    python_items_test
    python_lists_test
    python_paths_test
    python_pokedex_test
    python_pokemon_test
    python_pokemon_party_test
    python_pokemon_pc_test
    python_qt_test
)

FOREACH(test ${python_tests})
    PKMN_ADD_PYTHON_TEST(${test})
ENDFOREACH(test ${python_tests})

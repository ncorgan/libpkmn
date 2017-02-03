#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(pkmntest_python_sources
    items_tests.py
    pokemon_tests.py
    gen1_items_tests.py
    gen1_pokemon_tests.py
    gen2_items_tests.py
    gen2_pokemon_tests.py
    gba_items_tests.py
    gba_pokemon_tests.py
    pokemon_io_tests.py
    pokemon_party_tests.py
    pokemon_pc_tests.py
    __init__.py
)

FOREACH(src ${pkmntest_python_sources})
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/${src}
        ${CMAKE_CURRENT_BINARY_DIR}/${src}
    @ONLY)
ENDFOREACH(src ${pkmntest_python_sources})

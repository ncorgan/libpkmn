#
# Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

ALL_GAMES = [
    pkmn.game.RED, pkmn.game.BLUE, pkmn.game.YELLOW,
    pkmn.game.GOLD, pkmn.game.SILVER, pkmn.game.CRYSTAL,
    pkmn.game.RUBY, pkmn.game.SAPPHIRE, pkmn.game.EMERALD, pkmn.game.FIRERED, pkmn.game.LEAFGREEN,
    pkmn.game.COLOSSEUM, pkmn.game.XD
]
ALL_GAME_NAMES = [pkmn.game_to_string(game) for game in ALL_GAMES]

ALL_MAIN_SERIES_GAMES = [game for game in ALL_GAMES if game not in [pkmn.game.COLOSSEUM, pkmn.game.XD]]
ALL_MAIN_SERIES_GAME_NAMES = [pkmn.game_to_string(game) for game in ALL_MAIN_SERIES_GAMES]

DAYCARE_SUPPORTED_GAMES = [
    pkmn.game.RED, pkmn.game.BLUE, pkmn.game.YELLOW,
    pkmn.game.GOLD, pkmn.game.SILVER, pkmn.game.CRYSTAL,
    pkmn.game.COLOSSEUM, pkmn.game.XD
]
DAYCARE_SUPPORTED_GAME_NAMES = [pkmn.game_to_string(game) for game in DAYCARE_SUPPORTED_GAMES]

GAME_TO_GENERATION = {
    pkmn.game.RED: 1,
    pkmn.game.BLUE: 1,
    pkmn.game.YELLOW: 1,
    pkmn.game.GOLD: 2,
    pkmn.game.SILVER: 2,
    pkmn.game.CRYSTAL: 2,
    pkmn.game.RUBY: 3,
    pkmn.game.SAPPHIRE: 3,
    pkmn.game.EMERALD: 3,
    pkmn.game.FIRERED: 3,
    pkmn.game.LEAFGREEN: 3,
    pkmn.game.COLOSSEUM: 3,
    pkmn.game.XD: 3
}

from .base_test import *
from .gen1_items_tests import *
from .gen1_pokemon_tests import *
from .gen2_items_tests import *
from .gen2_pokemon_tests import *
from .gen3_pokemon_tests import *
from .gba_items_tests import *
from .gcn_items_tests import *
from .pokemon_common_tests import *
from .pokemon_conversions_tests import *
from .pokemon_io_tests import *
from .pokemon_party_tests import *
from .pokemon_pc_tests import *
from .pokemon_polymorphism_test import *
from .pokemon_subclass import *

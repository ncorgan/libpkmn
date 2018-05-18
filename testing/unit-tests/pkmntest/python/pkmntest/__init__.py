#
# Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

ALL_GAMES = [
    "Red", "Blue", "Yellow",
    "Gold", "Silver", "Crystal",
    "Ruby", "Sapphire", "Emerald", "FireRed", "LeafGreen", "Colosseum", "XD"
]
ALL_MAIN_SERIES_GAMES = [game for game in ALL_GAMES if game not in ["Colosseum", "XD"]]

GAME_TO_GENERATION = {
    "Red": 1,
    "Blue": 1,
    "Yellow": 1,
    "Gold": 2,
    "Silver": 2,
    "Crystal": 2,
    "Ruby": 3,
    "Sapphire": 3,
    "Emerald": 3,
    "FireRed": 3,
    "LeafGreen": 3,
    "Colosseum": 3,
    "XD": 3
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

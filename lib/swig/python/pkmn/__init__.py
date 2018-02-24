#
# Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

from .pkmn_python import *
from .stl_python import *

from . import calculations
from . import database

DEFAULT_TRAINER_ID = 2105214279
DEFAULT_TRAINER_NAME = "LibPKMN"

VERSION = "@PKMN_VERSION@"

initialize_database_connection()

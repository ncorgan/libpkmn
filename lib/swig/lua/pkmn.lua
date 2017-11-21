--
-- Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn_core_lua")

pkmn.buildinfo = require("pkmn_buildinfo_lua")
pkmn.calculations = require("pkmn_calculations_lua")
pkmn.database = require("pkmn_database_lua")
pkmn.gui = require("pkmn_gui_lua")
pkmn.paths = require("pkmn_paths_lua")
pkmn.stl = require("pkmn_stl_lua")

pkmn.game_save = pkmn.make_game_save
pkmn.item_bag = pkmn.make_item_bag
pkmn.item_list = pkmn.make_item_list
pkmn.pokemon = pkmn.make_pokemon
pkmn.pokemon_box = pkmn.make_pokemon_box
pkmn.pokemon_party = pkmn.make_pokemon_party
pkmn.pokemon_pc = pkmn.make_pokemon_pc

pkmn.DEFAULT_TRAINER_ID = 2105214279
pkmn.DEFAULT_TRAINER_NAME = "LibPKMN"

pkmn.VERSION = "@PKMN_VERSION@"

pkmn.buildinfo.SWIG_VERSION = "@SWIG_VERSION@"

return pkmn

--
-- Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = {}

pkmn.buildinfo = require("pkmn_buildinfo_lua")
pkmn.calculations = require("pkmn_calculations_lua")
pkmn.database = require("pkmn_database_lua")
pkmn.paths = require("pkmn_paths_lua")
pkmn.stl = require("pkmn_stl_lua")

pkmn.core = require("pkmn_core_lua")

pkmn.item_slot = pkmn.core.item_slot
pkmn.item_slot_list = pkmn.core.item_slot_list
pkmn.item_pockets = pkmn.core.item_pockets
pkmn.item_bag = pkmn.core.make_item_bag
pkmn.item_list = pkmn.core.make_item_list
pkmn.move_slot = pkmn.core.move_slot
pkmn.move_slot_list = pkmn.core.move_slot_list
pkmn.pokemon = pkmn.core.make_pokemon
pkmn.pokemon_list = pkmn.core.pokemon_list
pkmn.pokemon_box = pkmn.core.make_pokemon_box
pkmn.pokemon_box_list = pkmn.core.pokemon_box_list
pkmn.pokemon_pc = pkmn.core.make_pokemon_pc

pkmn.LIBPKMN_OT_ID = 2105214279;
pkmn.LIBPKMN_OT_NAME = "LibPKMN";

return pkmn

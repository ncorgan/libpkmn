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
pkmn.item_pockets = pkmn.core.item_pockets
pkmn.item_bag = pkmn.core.make_item_bag
pkmn.item_list = pkmn.core.make_item_list

return pkmn

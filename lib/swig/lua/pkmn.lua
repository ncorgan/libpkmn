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

pkmn.VERSION = "@PKMN_VERSION@"

pkmn.buildinfo.SWIG_VERSION = "@SWIG_VERSION@"

return pkmn

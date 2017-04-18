#!/usr/bin/ruby
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

require_relative "PKMN/PKMN"

require_relative "PKMN/BuildInfo"
require_relative "PKMN/Calculations"
require_relative "PKMN/Database"
require_relative "PKMN/Paths"
require_relative "PKMN/STL"

module PKMN
    module BuildInfo
        SWIG_VERSION = "@SWIG_VERSION@"
    end
end

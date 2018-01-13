/*
 * Copyright (c) 2015-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <csharp/csharp_init.i>
PKMN_CSHARP_INIT

%include <std_string.i>

%import <database_csharp.i>
%import <stl_csharp.i>

%{
    #include <pkmn/build_info.hpp>

    inline std::string GetSWIGVersion()
    {
        return std::string("@SWIG_VERSION@");
    }
%}

// Convert Doxygen docs to Python docstrings
%include <pkmn_csharp_docs.i>

// Build Info
%csmethodmodifiers pkmn::build_info::get_boost_version "internal";
%csmethodmodifiers pkmn::build_info::get_libpkmgc_version "internal";
%csmethodmodifiers pkmn::build_info::get_pksav_version "internal";
%csmethodmodifiers pkmn::build_info::get_qt_version "internal";
%csmethodmodifiers pkmn::build_info::get_sqlite3_version "internal";
%csmethodmodifiers pkmn::build_info::get_sqlitecpp_version "internal";
%include <pkmn/build_info.hpp>
%csmethodmodifiers GetSWIGVersion "internal";
std::string GetSWIGVersion();

// Attribute Maps
%include <csharp/pkmn_attribute_maps.i>

// Game Save
%include <csharp/pkmn_game_save.i>

// Item Slot
%include <csharp/pkmn_item_slot.i>

// Item List
%include <csharp/pkmn_item_list.i>

// Item Bag
%include <csharp/pkmn_item_bag.i>

// Pokémon
%include <csharp/pkmn_pokemon_helpers.i>
%include <csharp/pkmn_pokemon.i>

// Pokémon Box
%include <csharp/pkmn_pokemon_box.i>

// Pokémon Party
%include <csharp/pkmn_pokemon_party.i>

// Pokémon PC
%include <csharp/pkmn_pokemon_pc.i>

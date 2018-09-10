/*
 * Copyright (c) 2015-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%module(directors="1") PKMN

%include <csharp/csharp_init.i>
PKMN_CSHARP_INIT

%include <std_string.i>

%import <database_csharp.i>
%import <stl_csharp.i>

%{
    #include <pkmn/build_info.hpp>

    #include <pkmn/enums/enum_to_string.hpp>

    #include "private_exports.hpp"

    inline std::string GetSWIGVersion()
    {
        return std::string("@SWIG_VERSION@");
    }
%}

// Convert Doxygen docs to Python docstrings
%include <pkmn_csharp_docs.i>

// Private exports

%rename(InitializeDatabaseConnection) initialize_database_connection;
%rename(ConvertImageForMono) convert_image_for_mono;

%csmethodmodifiers initialize_database_connection "internal";
%csmethodmodifiers convert_image_for_mono "internal";

namespace pkmn { namespace priv {
    void initialize_database_connection();
    std::string convert_image_for_mono(const std::string& filepath);
}}

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

// Enums
%include <pkmn/enums/enum_to_string.hpp>

// Attribute Maps
%include <csharp/pkmn_attribute_maps.i>

// Daycare
%include <csharp/pkmn_daycare_helpers.i>
%include <csharp/pkmn_daycare.i>

// Time Duration
%include <csharp/pkmn_time_duration.i>

// Game Save
%include <csharp/pkmn_game_save.i>

// Item Slot
%include <csharp/pkmn_item_slot.i>

// Item List
%include <csharp/pkmn_item_list.i>

// Item Bag
%include <csharp/pkmn_item_bag.i>

// Pokédex
%include <csharp/pkmn_pokedex_helpers.i>
%include <csharp/pkmn_pokedex.i>

// Pokémon
%include <csharp/pkmn_pokemon_helpers.i>
%include <csharp/pkmn_pokemon.i>

// Pokémon Box
%include <csharp/pkmn_pokemon_box.i>

// Pokémon Party
%include <csharp/pkmn_pokemon_party.i>

// Pokémon PC
%include <csharp/pkmn_pokemon_pc.i>

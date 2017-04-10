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
    #include <pkmn/item_list.hpp>
    #include <pkmn/item_bag.hpp>

    #include <pkmn/types/datetime.hpp>

    #include <pkmn/utils/paths.hpp>

    #include <pkmn/build_info.hpp>

    PKMN_INLINE std::string GetSWIGVersion() {
        return std::string("@SWIG_VERSION@");
    }
%}

// Convert Doxygen docs to Python docstrings
%include <pkmn_csharp_docs.i>

// Build Info
%csmethodmodifiers pkmn::build_info::get_boost_version "internal";
%csmethodmodifiers pkmn::build_info::get_pksav_version "internal";
%csmethodmodifiers pkmn::build_info::get_sqlite3_version "internal";
%csmethodmodifiers pkmn::build_info::get_sqlitecpp_version "internal";
%include <pkmn/build_info.hpp>
%csmethodmodifiers GetSWIGVersion "internal";
std::string GetSWIGVersion();

// For all sptrs, for some reason didn't get ignored from pkmn.i
%ignore make;
%ignore get_native;

// Bring in our wrapped make functions, don't expose them
%csmethodmodifiers detect_game_save_type "internal";
%csmethodmodifiers make_game_save "internal";
%csmethodmodifiers make_item_bag "internal";
%csmethodmodifiers make_item_list "internal";
%csmethodmodifiers make_pokemon "internal";
%csmethodmodifiers make_pokemon_box "internal";
%csmethodmodifiers make_pokemon_party "internal";
%csmethodmodifiers make_pokemon_pc "internal";
%include <pkmn_statics.i>

// Datetime
%include <pkmn/types/datetime.hpp>

// Game Save
%include <csharp/pkmn_game_save.i>

// Item Slot
%include <csharp/pkmn_item_slot.i>

// Item List
%include <csharp/pkmn_item_list.i>
PKMN_CSHARP_MAP(std::string, pkmn::item_list::sptr, String, ItemList, ItemPockets);

// Item Bag
%include <csharp/pkmn_item_bag.i>

// Move Slot
%include <csharp/pkmn_move_slot.i>

// Pokémon 
%include <csharp/pkmn_pokemon.i>

// Pokémon Box
%include <csharp/pkmn_pokemon_box.i>

// Pokémon Party
%include <csharp/pkmn_pokemon_party.i>

// Pokémon PC
%include <csharp/pkmn_pokemon_pc.i>

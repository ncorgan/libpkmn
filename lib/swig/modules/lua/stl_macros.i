/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <std_pair.i>
%include <pkmn_shared_ptr.i>
%include <lua/pkmn_std_map.i>
%include <lua/pkmn_std_vector.i>

%define PKMN_LUA_MAP(cpp_key, cpp_val, lua_name)
    %template(lua_name) std::map<cpp_key, cpp_val>;
%enddef

%define PKMN_LUA_PAIR(cpp_type1, cpp_type2, lua_name)
    %template(lua_name) std::pair<cpp_type1, cpp_type2>;
%enddef

%define PKMN_LUA_SPTR(cpp_type)
    %extend std::shared_ptr<pkmn:: ## cpp_type> {
        bool __eq__(const std::shared_ptr<pkmn:: ## cpp_type> rhs) {
            return ((*self) == rhs);
        }
    }
    %template(cpp_type ## _sptr) std::shared_ptr<pkmn:: ## cpp_type>;
%enddef

%define PKMN_LUA_VECTOR(cpp_type, lua_name)
    %template(lua_name) std::vector<cpp_type >;
%enddef

/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <std_map.i>
%include <std_pair.i>
%include <std_vector.i>

%define PKMN_RUBY_MAP(cpp_key, cpp_val, ruby_name)
    %extend std::map<cpp_key, cpp_val > {
        bool __eq__(const std::map<cpp_key, cpp_val > & rhs) {
            return (*self) == rhs;
        }
    }
    %template(ruby_name) std::map<cpp_key, cpp_val >;
%enddef

%define PKMN_RUBY_PAIR(cpp_type1, cpp_type2, ruby_name)
    %extend std::pair<cpp_type1 , cpp_type2 > {
        bool __eq__(const std::pair<cpp_type1, cpp_type2 > & rhs) {
            return (*self) == rhs;
        }
    }
    %template(ruby_name) std::pair<cpp_type1, cpp_type2 >;
%enddef

%define PKMN_RUBY_VECTOR(cpp_type, ruby_name)
    %extend std::vector<cpp_type > {
        bool __eq__(const std::vector<cpp_type > & rhs) {
            return (*self) == rhs;
        }
    }
    %template(ruby_name) std::vector<cpp_type >;
%enddef

/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/attribute_maps.hpp"
%}

%include <attribute.i>

// Subset of classes we actually want wrapped

namespace pkmn { namespace swig {

template <class sptr_type>
class numeric_attribute_map
{
    public:
        std::vector<std::string> get_attribute_names();
};

template <class sptr_type>
class string_attribute_map
{
    public:
        std::vector<std::string> get_attribute_names();
};

template <class sptr_type>
class boolean_attribute_map
{
    public:
        std::vector<std::string> get_attribute_names();
};

}}

// Numeric attributes

%extend pkmn::swig::numeric_attribute_map
{
    int __getitem__(
        const std::string& attribute_name
    )
    {
        return self->get_attribute(attribute_name);
    }

    void __setitem__(
        const std::string& attribute_name,
        int value
    )
    {
        self->set_attribute(attribute_name, value);
    }

    int __len(void*)
    {
        return int(self->get_attribute_names().size());
    }
}

// String attributes

%extend pkmn::swig::string_attribute_map
{
    std::string __getitem__(
        const std::string& attribute_name
    )
    {
        return self->get_attribute(attribute_name);
    }

    void __setitem__(
        const std::string& attribute_name,
        const std::string& value
    )
    {
        self->set_attribute(attribute_name, value);
    }

    int __len(void*)
    {
        return int(self->get_attribute_names().size());
    }
}

// Boolean attributes

%extend pkmn::swig::boolean_attribute_map
{
    bool __getitem__(
        const std::string& attribute_name
    )
    {
        return self->get_attribute(attribute_name);
    }

    void __setitem__(
        const std::string& attribute_name,
        bool value
    )
    {
        self->set_attribute(attribute_name, value);
    }

    int __len(void*)
    {
        return int(self->get_attribute_names().size());
    }
}

// Definitions

%define PKMN_LUA_ATTRIBUTE_MAPS(sptr_type, lua_name_numeric, lua_name_string, lua_name_boolean)
    %ignore pkmn::swig::numeric_attribute_map<sptr_type>::numeric_attribute_map();
    %attributeval(pkmn::swig::numeric_attribute_map<sptr_type>, %arg(std::vector<std::string>), names, get_attribute_names);
    %template(lua_name_numeric) pkmn::swig::numeric_attribute_map<sptr_type>;

    %ignore pkmn::swig::string_attribute_map<sptr_type>::string_attribute_map();
    %attributeval(pkmn::swig::string_attribute_map<sptr_type>, %arg(std::vector<std::string>), names, get_attribute_names);
    %template(lua_name_string)  pkmn::swig::string_attribute_map<sptr_type>;

    %ignore pkmn::swig::boolean_attribute_map<sptr_type>::boolean_attribute_map();
    %attributeval(pkmn::swig::boolean_attribute_map<sptr_type>, %arg(std::vector<std::string>), names, get_attribute_names);
    %template(lua_name_boolean)  pkmn::swig::boolean_attribute_map<sptr_type>;
%enddef

PKMN_LUA_ATTRIBUTE_MAPS(pkmn::pokemon, pokemon_numeric_attribute_map, pokemon_string_attribute_map, pokemon_boolean_attribute_map);
PKMN_LUA_ATTRIBUTE_MAPS(pkmn::game_save, game_save_numeric_attribute_map, game_save_string_attribute_map, game_save_boolean_attribute_map);

/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

// TODO: equality, hash code

%{
    #include "cpp_wrappers/attribute_maps.hpp"
%}

// Subset of classes we actually want wrapped

namespace pkmn { namespace swig {

template <class sptr_type>
class numeric_attribute_map
{
    public:
        int get_attribute(const std::string& attribute);
        void set_attribute(const std::string& attribute, int value);
        std::vector<std::string> get_attribute_names();

        // For internal use
        uintmax_t cptr();
};

template <class sptr_type>
class string_attribute_map
{
    public:
        std::string get_attribute(const std::string& attribute);
        void set_attribute(const std::string& attribute, const std::string& value);
        std::vector<std::string> get_attribute_names();

        // For internal use
        uintmax_t cptr();
};

}}

// Numeric attributes

%typemap(csimports) pkmn::swig::numeric_attribute_map "
using System;
using System.Runtime.InteropServices;"

%csmethodmodifiers pkmn::swig::numeric_attribute_map::get_attribute "private";
%csmethodmodifiers pkmn::swig::numeric_attribute_map::set_attribute "private";
%csmethodmodifiers pkmn::swig::numeric_attribute_map::get_attribute_names "private";
%csmethodmodifiers pkmn::swig::numeric_attribute_map::cptr "private";

%typemap(cscode) pkmn::swig::numeric_attribute_map
%{
    public int this[string attributeName]
    {
        get { return GetAttribute(attributeName); }
        set { SetAttribute(attributeName, value); }
    }

    public StringList Names
    {
        get { return GetAttributeNames(); }
    }

    public bool Equals($csclassname rhs)
    {
        return (this.Cptr() == rhs.Cptr());
    }

    public override bool Equals(System.Object rhs)
    {
        if(rhs == null)
        {
            return false;
        }

        $csclassname rhsAsClass = rhs as $csclassname;
        if(rhsAsClass == null)
        {
            return false;
        }
        else
        {
            return this.Equals(rhsAsClass);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<ulong>(this.Cptr())
                                       .AddValue<StringList>(this.Names)
                                       .AddValue<String>(this.GetType().Name)
                                       .ToHashCode();
    }
%}

// String attributes

%typemap(csimports) pkmn::swig::string_attribute_map "
using System;
using System.Runtime.InteropServices;"

%csmethodmodifiers pkmn::swig::string_attribute_map::get_attribute "private";
%csmethodmodifiers pkmn::swig::string_attribute_map::set_attribute "private";
%csmethodmodifiers pkmn::swig::string_attribute_map::get_attribute_names "private";
%csmethodmodifiers pkmn::swig::string_attribute_map::cptr "private";

%typemap(cscode) pkmn::swig::string_attribute_map
%{
    public string this[string attributeName]
    {
        get { return GetAttribute(attributeName); }
        set { SetAttribute(attributeName, value); }
    }

    public StringList Names
    {
        get { return GetAttributeNames(); }
    }

    public bool Equals($csclassname rhs)
    {
        return (this.Cptr() == rhs.Cptr());
    }

    public override bool Equals(System.Object rhs)
    {
        if(rhs == null)
        {
            return false;
        }

        $csclassname rhsAsClass = rhs as $csclassname;
        if(rhsAsClass == null)
        {
            return false;
        }
        else
        {
            return this.Equals(rhsAsClass);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<ulong>(this.Cptr())
                                       .AddValue<StringList>(this.Names)
                                       .AddValue<String>(this.GetType().Name)
                                       .ToHashCode();
    }
%}

// Definitions

%define PKMN_ATTRIBUTE_MAPS(sptr_type, csharp_name_numeric, csharp_name_string)
    %ignore pkmn::swig::numeric_attribute_map<sptr_type>::numeric_attribute_map();
    %template(csharp_name_numeric) pkmn::swig::numeric_attribute_map<sptr_type>;

    %ignore pkmn::swig::string_attribute_map<sptr_type>::string_attribute_map();
    %template(csharp_name_string)  pkmn::swig::string_attribute_map<sptr_type>;
%enddef

PKMN_ATTRIBUTE_MAPS(pkmn::pokemon, PokemonNumericAttributeMap, PokemonStringAttributeMap);

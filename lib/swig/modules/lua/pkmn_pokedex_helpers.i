/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/pokedex_helpers.hpp"
%}

//
// Has Seen helper
//

%ignore pkmn::swig::pokedex_has_seen_helper::pokedex_has_seen_helper();
%ignore pkmn::swig::pokedex_has_seen_helper::pokedex_has_seen_helper(const pkmn::pokedex::sptr&);
%ignore pkmn::swig::pokedex_has_seen_helper::pokedex_has_seen_helper(const pkmn::pokedex::sptr&, bool);
%ignore pkmn::swig::pokedex_has_seen_helper::get_has_seen(const std::string&);
%ignore pkmn::swig::pokedex_has_seen_helper::set_has_seen(const std::string&, bool);

// Replace methods with more idiomatic Lua.

%extend pkmn::swig::pokedex_has_seen_helper
{
    bool __getitem__(const std::string& species)
    {
        return self->get_has_seen(species);
    }

    void __setitem__(
        const std::string& species,
        bool value
    )
    {
        self->set_has_seen(species, value);
    }
}

//
// Has Caught helper
//

%ignore pkmn::swig::pokedex_has_caught_helper::pokedex_has_caught_helper();
%ignore pkmn::swig::pokedex_has_caught_helper::pokedex_has_caught_helper(const pkmn::pokedex::sptr&);
%ignore pkmn::swig::pokedex_has_caught_helper::get_has_caught(const std::string&);
%ignore pkmn::swig::pokedex_has_caught_helper::set_has_caught(const std::string&, bool);

// Replace methods with more idiomatic Lua.

%extend pkmn::swig::pokedex_has_caught_helper
{
    bool __getitem__(const std::string& species)
    {
        return self->get_has_caught(species);
    }

    void __setitem__(const std::string& species, bool value)
    {
        self->set_has_caught(species, value);
    }
}

%include "cpp_wrappers/pokedex_helpers.hpp"

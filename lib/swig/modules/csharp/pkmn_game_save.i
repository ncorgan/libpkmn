/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/game_save.hpp>
%}

%rename(game_save_base) pkmn::game_save;

%csmethodmodifiers pkmn::shared_ptr<pkmn::game_save>::__cptr "
private";
%csmethodmodifiers pkmn::shared_ptr<pkmn::game_save>::__sptr_eq "
private";

%typemap(cscode) pkmn::shared_ptr<pkmn::game_save> %{
    /// <summary> Returns the type of save at the given filepath.
    public static string DetectType(string filepath) {
        string ret = PKMNPINVOKE.detect_game_save_type(filepath);
        if(PKMNPINVOKE.SWIGPendingException.Pending) {
            throw PKMNPINVOKE.SWIGPendingException.Retrieve();
        }
        return ret;
    }

    /// <summary>Compares two GameSave instances to determine value equality.</summary>
    /// <remarks>
    /// Returns true if the internal shared_ptrs' pointers are equal.
    /// </remarks>
    /// <param name="rhs">GameSave with which to compare self</param>
    /// <returns>Whether or not GameSave instances are equal</returns>
    /// </remarks>
    public bool Equals(GameSave rhs) {
        if(rhs == null) {
            return false;
        } else if(this == rhs) {
            return true;
        } else {
            return this.__sptr_eq(rhs);
        }
    }

    /// <summary>Compares an instance of GameSave to a C# object.</summary>
    /// <param name="rhs">Object with which to compare self</param>
    /// <returns>Whether or not GameSave and Object are equal</returns>
    public override bool Equals(System.Object rhs) {
        if(rhs == null) {
            return false;
        }

        GameSave rhsSptr = rhs as GameSave;
        if(rhsSptr == null) {
            return false;
        } else {
            return this.Equals(rhsSptr);
        }
    }

    /// <summary>Generates a unique hash code for the given GameSave.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        return HashCodeBuilder.Create().AddValue<ulong>(__cptr())
                              .ToHashCode();
    }
%}

%ignore from_file;
%ignore detect_type;
%include <pkmn/game_save.hpp>
%template(GameSave) pkmn::shared_ptr<pkmn::game_save>;

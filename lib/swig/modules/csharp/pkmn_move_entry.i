/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/database/move_entry.hpp>
%}

%include <attribute.i>

// Convert getter/setter functions into attributes for more idiomatic C#.

%attribute(pkmn::database::move_entry, pkmn::e_move, Move, get_move);
%attributestring(pkmn::database::move_entry, std::string, Name, get_name);
%attribute(pkmn::database::move_entry, pkmn::e_game, Game, get_game);
%attribute(pkmn::database::move_entry, pkmn::e_type, MoveType, get_type);
%attributestring(pkmn::database::move_entry, std::string, Description, get_description);
%attribute(pkmn::database::move_entry, pkmn::e_move_target, Target, get_target);
%attribute(pkmn::database::move_entry, pkmn::e_move_damage_class, DamageClass, get_damage_class);
%attribute(pkmn::database::move_entry, int, BasePower, get_base_power);
%attribute(pkmn::database::move_entry, float, Accuracy, get_accuracy);
%attribute(pkmn::database::move_entry, int, Priority, get_priority);
%attributestring(pkmn::database::move_entry, std::string, Effect, get_effect);
%attribute(pkmn::database::move_entry, pkmn::e_contest_stat, ContestType, get_contest_type);
%attributestring(pkmn::database::move_entry, std::string, ContestEffect, get_contest_effect);
%attributestring(pkmn::database::move_entry, std::string, SuperContestEffect, get_super_contest_effect);

%ignore pkmn::database::move_entry::get_move_id;
%ignore pkmn::database::move_entry::get_game_id;

%typemap(cscode) pkmn::database::move_entry
%{
    public bool Equals(MoveEntry rhs)
    {
        if(rhs == null)
        {
            return false;
        }
        else if(this == rhs)
        {
            return true;
        }
        else
        {
            return this.Name.Equals(rhs.Name) &&
                   this.Game.Equals(rhs.Game);
        }
    }

    public override bool Equals(System.Object rhs)
    {
        if(rhs == null)
        {
            return false;
        }

        MoveEntry rhsEntry = rhs as MoveEntry;
        if(rhsEntry == null)
        {
            return false;
        }
        else
        {
            return this.Equals(rhsEntry);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<string>(this.Name)
                                       .AddValue<Game>(this.Game)
                                       .ToHashCode();
    }
%}

%include <pkmn/database/move_entry.hpp>

%include <csharp/stl_macros.i>
PKMN_CSHARP_VECTOR(pkmn::database::move_entry, MoveEntry, MoveEntryList)

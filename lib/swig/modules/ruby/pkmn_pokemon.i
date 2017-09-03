/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <ruby/stl_macros.i>

%{
    #include <type_traits>

    #include <pkmn/pokemon.hpp>

    #include "cpp_wrappers/pokemon.hpp"
%}

%include <std_string.i>

// map renames/ignores
%ignore get_internal;
%rename("PokemonEVHash") pokemon_EV_map;
%rename("has_key?") has_key;
%rename("__getitem__") get_EV;
%rename("__setitem__") set_EV;
%rename("PokemonIVHash") pokemon_IV_map;
%rename("has_key?") has_key;
%rename("__getitem__") get_IV;
%rename("__setitem__") set_IV;
%rename("PokemonMarkingHash") pokemon_marking_map;
%rename("has_key?") has_key;
%rename("__getitem__") get_marking;
%rename("__setitem__") set_marking;
%rename("PokemonRibbonHash") pokemon_ribbon_map;
%rename("has_key?") has_key;
%rename("__getitem__") get_ribbon;
%rename("__setitem__") set_ribbon;
%rename("PokemonContestStatHash") pokemon_contest_stat_map;
%rename("has_key?") has_key;
%rename("__getitem__") get_contest_stat;
%rename("__setitem__") set_contest_stat;

// Move slots
%rename("PokemonMoveSlotWrapper") pokemon_move_slot_wrapper;
%rename("move") get_move;
%rename("move=") set_move;
%rename("pp") get_pp;
%rename("PokemonMoveSlotsWrapper") pokemon_move_slots_wrapper;
%rename("__getitem__") get_move_slot;
%rename("length") size;

// pkmn::swig::pokemon renames
%rename("Pokemon") pokemon;
%rename("species") get_species;
%rename("game") get_game;
%rename("form") get_form;
%rename("form=") set_form;
%rename("database_entry") get_database_entry;
%rename("nickname") get_nickname;
%rename("nickname=") set_nickname;
%rename("gender") get_gender;
%rename("gender=") set_gender;
%rename("is_shiny?") is_shiny;
%rename("is_shiny=") set_shininess;
%rename("held_item") get_held_item;
%rename("held_item=") set_held_item;
%rename("trainer_name") get_trainer_name;
%rename("trainer_name=") set_trainer_name;
%rename("trainer_public_id") get_trainer_public_id;
%rename("trainer_public_id=") set_trainer_public_id;
%rename("trainer_secret_id") get_trainer_secret_id;
%rename("trainer_secret_id=") set_trainer_secret_id;
%rename("trainer_id") get_trainer_id;
%rename("trainer_id=") set_trainer_id;
%rename("trainer_gender") get_trainer_gender;
%rename("trainer_gender=") set_trainer_gender;
%rename("friendship") get_friendship;
%rename("friendship=") set_friendship;
%rename("ability") get_ability;
%rename("ability=") set_ability;
%rename("ball") get_ball;
%rename("ball=") set_ball;
%rename("level_met") get_level_met;
%rename("level_met=") set_level_met;
%rename("original_game") get_original_game;
%rename("original_game=") set_original_game;
%rename("personality") get_personality;
%rename("personality=") set_personality;
%rename("experience") get_experience;
%rename("experience=") set_experience;
%rename("level") get_level;
%rename("level=") set_level;
%rename("markings") get_markings;
%rename("ribbons") get_ribbons;
%rename("contest_stats") get_contest_stats;
%rename("moves") get_moves;
%rename("EVs") get_EVs;
%rename("IVs") get_IVs;
%rename("stats") get_stats;
%rename("icon_filepath") get_icon_filepath;
%rename("sprite_filepath") get_sprite_filepath;

%include "cpp_wrappers/pokemon.hpp"

// Needed to avoid compile error
%{
    namespace swig
    {
        template <> struct traits<bool>
        {
            typedef pointer_category category;
            static const char* type_name()
            {
                return "bool";
            }
        };

        template <> struct traits<pkmn::swig::pokemon_move_slot_wrapper>
        {
            typedef pointer_category category;
            static const char* type_name()
            {
                return "pkmn::swig::pokemon_move_slot_wrapper";
            }
        };
    }
%}

// Based on SWIG's std_map.i
%define EXTEND_POKEMON_MAP(field, val_type)
    %extend pkmn::swig::pokemon_ ## field ## _map
    {
        VALUE keys()
        {
            const std::map<std::string, val_type>& internal = self->get_internal()->get_ ## field ## s();

            std::map<std::string, val_type>::size_type size = internal.size();
            int rubysize = (size <= (std::map<std::string, val_type>::size_type) INT_MAX) ? (int) size : -1;
            if (rubysize < 0)
            {
                SWIG_RUBY_THREAD_BEGIN_BLOCK;
                rb_raise(rb_eRuntimeError, "map size not valid in Ruby");
                SWIG_RUBY_THREAD_END_BLOCK;
                return Qnil;
            }
            VALUE ary = rb_ary_new2(rubysize);
            std::map<std::string, val_type>::const_iterator i = internal.begin();
            std::map<std::string, val_type>::const_iterator e = internal.end();
            for ( ; i != e; ++i )
            {
                rb_ary_push( ary, swig::from(i->first) );
            }
            return ary;
        }

        pkmn::swig::pokemon_ ## field ## _map* each()
        {
            if ( !rb_block_given_p() )
                rb_raise( rb_eArgError, "no block given");

            const std::map<std::string, val_type>& internal = self->get_internal()->get_ ## field ## s();

            VALUE k, v;
            std::map<std::string, val_type>::const_iterator i = internal.begin();
            std::map<std::string, val_type>::const_iterator e = internal.end();
            for ( ; i != e; ++i )
            {
                const std::map<std::string, val_type>::key_type&    key = i->first;
                const std::map<std::string, val_type>::mapped_type& val = i->second;

                k = swig::from<std::map<std::string, val_type>::key_type>(key);
                if(std::is_same<val_type, bool>::value)
                {
                    v = val ? Qtrue : Qfalse;
                }
                else
                {
                    v = swig::from<std::map<std::string, val_type>::mapped_type>(val);
                }
                rb_yield_values(2, k, v);
            }

            return self;
        }
    }
%enddef

EXTEND_POKEMON_MAP(EV, int);
EXTEND_POKEMON_MAP(IV, int);
EXTEND_POKEMON_MAP(marking, bool);
EXTEND_POKEMON_MAP(ribbon, bool);
EXTEND_POKEMON_MAP(contest_stat, int);

%extend pkmn::swig::pokemon_move_slots_wrapper
{
    pkmn::swig::pokemon_move_slots_wrapper* each()
    {
        if(!rb_block_given_p())
            rb_raise(rb_eArgError, "no block given");

        VALUE r;
        for(int i = 0; i < 4; ++i)
        {
            r = swig::from<pkmn::swig::pokemon_move_slot_wrapper>(self->get_move_slot(i));
            rb_yield(r);
        }

        return self;
    }
}

// Suppress shadowing warning when adding static variables.
%warnfilter(508) pkmn::shared_ptr<pkmn::pokemon>;

%extend pkmn::swig::pokemon
{
    static const uint32_t DEFAULT_TRAINER_ID = pkmn::pokemon::DEFAULT_TRAINER_ID;
    static const std::string DEFAULT_TRAINER_NAME = pkmn::pokemon::DEFAULT_TRAINER_NAME;
}

PKMN_RUBY_VECTOR(pkmn::swig::pokemon, PokemonList);
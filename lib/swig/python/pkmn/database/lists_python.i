/*
 * Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

/*
 * Passing in vectors by reference is fine in C++ but isn't very Pythonic.
 * This is fine with the way SWIG works, as returning the Python-level
 * vector just copies a pointer, not the whole vector.
 */

%{
    #include <pkmn/database/lists.hpp>
%}

%rename(__get_ability_list) get_ability_list;
%rename(__get_game_list) get_game_list;
%rename(__get_item_list) get_item_list;
%rename(__get_location_list) get_location_list;
%rename(__get_move_list) get_move_list;
%rename(__get_nature_list) get_nature_list;
%rename(__get_pokemon_list) get_pokemon_list;
%rename(__get_region_list) get_region_list;
%rename(__get_ribbon_list) get_ribbon_list;
%rename(__get_super_training_medal_list) get_super_training_medal_list;
%rename(__get_type_list) get_type_list;

%include <pkmn/database/lists.hpp>

%pythoncode %{

    import pkmn

    def get_ability_list(generation):
        ret = pkmn.string_list()
        __get_ability_list(generation, ret)
        return ret

    def get_game_list(generation, include_previous):
        ret = pkmn.string_list()
        __get_game_list(generation, include_previous, ret)
        return ret

    def get_item_list(game):
        ret = pkmn.string_list()
        __get_item_list(game, ret)
        return ret

    def get_location_list(game, whole_generation):
        ret = pkmn.string_list()
        __get_location_list(game, whole_generation, ret)
        return ret

    def get_move_list(game):
        ret = pkmn.string_list()
        __get_move_list(game, ret)
        return ret

    def get_nature_list():
        ret = pkmn.string_list()
        __get_nature_list(ret)
        return ret

    def get_pokemon_list(generation, include_previous):
        ret = pkmn.string_list()
        __get_pokemon_list(generation, include_previous, ret)
        return ret

    def get_region_list():
        ret = pkmn.string_list()
        __get_region_list(ret)
        return ret

    def get_ribbon_list(generation):
        ret = pkmn.string_list()
        __get_ribbon_list(generation, ret)
        return ret

    def get_super_training_medal_list():
        ret = pkmn.string_list()
        __get_super_training_medal_list(ret)
        return ret

    def get_type_list(game):
        ret = pkmn.string_list()
        __get_type_list(game, ret)
        return ret

%}

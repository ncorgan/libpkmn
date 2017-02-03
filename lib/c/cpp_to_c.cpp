/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"

namespace pkmn {

    void pkmn_hidden_power_cpp_to_c(
        const pkmn::calculations::hidden_power &hp_cpp,
        pkmn_hidden_power_t* hp_c
    ) {
        hp_c->type = (char*)std::malloc(hp_cpp.type.size() + 1);
        std::strcpy(hp_c->type, hp_cpp.type.c_str());
        hp_c->type[hp_cpp.type.size()] = '\0';

        hp_c->base_power = hp_cpp.base_power;
    }

    void pkmn_item_entry_cpp_to_c(
        const pkmn::database::item_entry &item_entry_cpp,
        pkmn_database_item_entry_t* item_entry_c
    ) {
        pkmn::std_string_to_c_str_alloc(
            item_entry_cpp.get_name(),
            &item_entry_c->name
        );
        pkmn::std_string_to_c_str_alloc(
            item_entry_cpp.get_game(),
            &item_entry_c->game
        );
        pkmn::std_string_to_c_str_alloc(
            item_entry_cpp.get_category(),
            &item_entry_c->category
        );
        pkmn::std_string_to_c_str_alloc(
            item_entry_cpp.get_pocket(),
            &item_entry_c->pocket
        );
        pkmn::std_string_to_c_str_alloc(
            item_entry_cpp.get_description(),
            &item_entry_c->description
        );

        item_entry_c->cost = item_entry_cpp.get_cost();
        item_entry_c->holdable = item_entry_cpp.holdable();
        item_entry_c->fling_power = item_entry_cpp.get_fling_power();

        pkmn::std_string_to_c_str_alloc(
            item_entry_cpp.get_fling_effect(),
            &item_entry_c->fling_effect
        );
    }

    void pkmn_item_slot_cpp_to_c(
        const pkmn::item_slot &islot_cpp,
        pkmn_item_slot_t* islot_c
    ) {
        std::string item_name = islot_cpp.item.get_name();
        islot_c->item = (char*)std::malloc(item_name.size() + 1);
        std::strcpy(islot_c->item, item_name.c_str());
        islot_c->item[item_name.size()] = '\0';

        islot_c->amount = islot_cpp.amount;
    }

    void pkmn_item_slots_cpp_to_c(
        const pkmn::item_slots_t &islots_cpp,
        pkmn_item_slots_t* islots_c
    ) {
        islots_c->item_slots = (pkmn_item_slot_t*)std::malloc(sizeof(pkmn_item_slot_t) * islots_cpp.size());
        for(size_t i = 0; i < islots_cpp.size(); ++i) {
            pkmn_item_slot_cpp_to_c(
                islots_cpp[i],
                &(islots_c->item_slots[i])
            );
        }
        islots_c->length = islots_cpp.size();
    }

    void pkmn_levelup_move_cpp_to_c(
        const pkmn::database::levelup_move &lmove_cpp,
        pkmn_levelup_move_t* lmove_c
    ) {
        std::string move_name = lmove_cpp.move.get_name();
        lmove_c->move = (char*)std::malloc(move_name.size() + 1);
        std::strcpy(lmove_c->move, move_name.c_str());
        lmove_c->move[move_name.size()] = '\0';

        lmove_c->level = lmove_cpp.level;
    }

    void pkmn_levelup_moves_cpp_to_c(
        const pkmn::database::levelup_moves_t &lmoves_cpp,
        pkmn_levelup_moves_t* lmoves_c
    ) {
        lmoves_c->levelup_moves = (pkmn_levelup_move_t*)std::malloc(sizeof(pkmn_levelup_move_t) * lmoves_cpp.size());
        for(size_t i = 0; i < lmoves_cpp.size(); ++i) {
            pkmn_levelup_move_cpp_to_c(
                lmoves_cpp[i],
                &(lmoves_c->levelup_moves[i])
            );
        }

        lmoves_c->length = lmoves_cpp.size();
    }

    void pkmn_move_entry_cpp_to_c(
        const pkmn::database::move_entry &move_entry_cpp,
        pkmn_database_move_entry_t* move_entry_c
    ) {
        pkmn::std_string_to_c_str_alloc(
            move_entry_cpp.get_name(),
            &move_entry_c->name
        );
        pkmn::std_string_to_c_str_alloc(
            move_entry_cpp.get_game(),
            &move_entry_c->game
        );
        pkmn::std_string_to_c_str_alloc(
            move_entry_cpp.get_description(),
            &move_entry_c->description
        );
        pkmn::std_string_to_c_str_alloc(
            move_entry_cpp.get_target(),
            &move_entry_c->target
        );
        pkmn::std_string_to_c_str_alloc(
            move_entry_cpp.get_damage_class(),
            &move_entry_c->damage_class
        );

        move_entry_c->base_power = move_entry_cpp.get_base_power();
        for(int i = 0; i < 4; ++i) {
            move_entry_c->pp[i] = move_entry_cpp.get_pp(i);
        }
        move_entry_c->accuracy = move_entry_cpp.get_accuracy();

        pkmn::std_string_to_c_str_alloc(
            move_entry_cpp.get_effect(),
            &move_entry_c->effect
        );
        pkmn::std_string_to_c_str_alloc(
            move_entry_cpp.get_contest_type(),
            &move_entry_c->contest_type
        );
        pkmn::std_string_to_c_str_alloc(
            move_entry_cpp.get_contest_effect(),
            &move_entry_c->contest_effect
        );
        pkmn::std_string_to_c_str_alloc(
            move_entry_cpp.get_super_contest_effect(),
            &move_entry_c->super_contest_effect
        );
    }

    void pkmn_move_list_to_string_list(
        const pkmn::database::move_list_t &move_list,
        pkmn_string_list_t* string_list_out
    ) {
        string_list_out->strings = (char**)std::malloc(sizeof(char*) * move_list.size());
        for(size_t i = 0; i < move_list.size(); ++i) {
            std::string move_name = move_list[i].get_name();
            string_list_out->strings[i] = (char*)std::malloc(move_name.size() + 1);
            std::strcpy(string_list_out->strings[i], move_name.c_str());
            string_list_out->strings[i][move_name.size()] = '\0';
        }

        string_list_out->length = move_list.size();
    }

    void pkmn_pokemon_entry_cpp_to_c(
        const pkmn::database::pokemon_entry &pokemon_entry_cpp,
        pkmn_database_pokemon_entry_t* pokemon_entry_c
    ) {
        pkmn::std_string_to_c_str_alloc(
            pokemon_entry_cpp.get_name(),
            &pokemon_entry_c->name
        );
        pkmn::std_string_to_c_str_alloc(
            pokemon_entry_cpp.get_game(),
            &pokemon_entry_c->game
        );
        pkmn::std_string_to_c_str_alloc(
            pokemon_entry_cpp.get_form(),
            &pokemon_entry_c->form
        );
        pkmn::std_string_to_c_str_alloc(
            pokemon_entry_cpp.get_species(),
            &pokemon_entry_c->species
        );
        pkmn::std_string_to_c_str_alloc(
            pokemon_entry_cpp.get_pokedex_entry(),
            &pokemon_entry_c->pokedex_entry
        );

        pokemon_entry_c->height = pokemon_entry_cpp.get_height();
        pokemon_entry_c->weight = pokemon_entry_cpp.get_weight();
        pokemon_entry_c->chance_male = pokemon_entry_cpp.get_chance_male();
        pokemon_entry_c->chance_female = pokemon_entry_cpp.get_chance_female();
        pokemon_entry_c->has_gender_differences = pokemon_entry_cpp.has_gender_differences();
        pokemon_entry_c->base_friendship = pokemon_entry_cpp.get_base_friendship();

        pkmn::std_pair_std_string_to_string_pair(
            pokemon_entry_cpp.get_types(),
            &pokemon_entry_c->types
        );
        pkmn::std_pair_std_string_to_string_pair(
            pokemon_entry_cpp.get_abilities(),
            &pokemon_entry_c->abilities
        );

        pkmn::std_string_to_c_str_alloc(
            pokemon_entry_cpp.get_hidden_ability(),
            &pokemon_entry_c->hidden_ability
        );

        pkmn::std_pair_std_string_to_string_pair(
            pokemon_entry_cpp.get_egg_groups(),
            &pokemon_entry_c->egg_groups
        );

        std::map<std::string, int> base_stats = pokemon_entry_cpp.get_base_stats();
        pokemon_entry_c->base_stats[PKMN_STAT_HP] = base_stats.at("HP");
        pokemon_entry_c->base_stats[PKMN_STAT_ATTACK] = base_stats.at("Attack");
        pokemon_entry_c->base_stats[PKMN_STAT_DEFENSE] = base_stats.at("Defense");
        pokemon_entry_c->base_stats[PKMN_STAT_SPEED] = base_stats.at("Speed");
        if(base_stats.count("Special") > 0) {
            pokemon_entry_c->base_stats[PKMN_STAT_SPECIAL] = base_stats.at("Special");
            pokemon_entry_c->base_stats[PKMN_STAT_SPATK] = -1;
            pokemon_entry_c->base_stats[PKMN_STAT_SPDEF] = -1;
        } else {
            pokemon_entry_c->base_stats[PKMN_STAT_SPECIAL] = -1;
            pokemon_entry_c->base_stats[PKMN_STAT_SPATK] = base_stats.at("Special Attack");
            pokemon_entry_c->base_stats[PKMN_STAT_SPDEF] = base_stats.at("Special Defense");
        }

        std::map<std::string, int> EV_yields = pokemon_entry_cpp.get_EV_yields();
        pokemon_entry_c->EV_yields[PKMN_STAT_HP] = EV_yields.at("HP");
        pokemon_entry_c->EV_yields[PKMN_STAT_ATTACK] = EV_yields.at("Attack");
        pokemon_entry_c->EV_yields[PKMN_STAT_DEFENSE] = EV_yields.at("Defense");
        pokemon_entry_c->EV_yields[PKMN_STAT_SPEED] = EV_yields.at("Speed");
        if(EV_yields.count("Special") > 0) {
            pokemon_entry_c->EV_yields[PKMN_STAT_SPECIAL] = EV_yields.at("Special");
            pokemon_entry_c->EV_yields[PKMN_STAT_SPATK] = -1;
            pokemon_entry_c->EV_yields[PKMN_STAT_SPDEF] = -1;
        } else {
            pokemon_entry_c->EV_yields[PKMN_STAT_SPECIAL] = -1;
            pokemon_entry_c->EV_yields[PKMN_STAT_SPATK] = EV_yields.at("Special Attack");
            pokemon_entry_c->EV_yields[PKMN_STAT_SPDEF] = EV_yields.at("Special Defense");
        }

        pokemon_entry_c->experience_yield = pokemon_entry_cpp.get_experience_yield();

        pkmn::pkmn_levelup_moves_cpp_to_c(
            pokemon_entry_cpp.get_levelup_moves(),
            &pokemon_entry_c->levelup_moves
        );

        pkmn::pkmn_move_list_to_string_list(
            pokemon_entry_cpp.get_tm_hm_moves(),
            &pokemon_entry_c->tm_hm_moves
        );
        pkmn::pkmn_move_list_to_string_list(
            pokemon_entry_cpp.get_egg_moves(),
            &pokemon_entry_c->egg_moves
        );
        pkmn::pkmn_move_list_to_string_list(
            pokemon_entry_cpp.get_tutor_moves(),
            &pokemon_entry_c->tutor_moves
        );

        pkmn::std_vector_std_string_to_string_list(
            pokemon_entry_cpp.get_forms(),
            &pokemon_entry_c->forms
        );

        pkmn::pkmn_pokemon_entries_to_string_list(
            pokemon_entry_cpp.get_evolutions(),
            &pokemon_entry_c->evolutions
        );
    }

    void pkmn_move_slot_cpp_to_c(
        const pkmn::move_slot &mslot_cpp,
        pkmn_move_slot_t* mslot_c
    ) {
        std::string move_name = mslot_cpp.move.get_name();
        mslot_c->move = (char*)std::malloc(move_name.size() + 1);
        std::strcpy(mslot_c->move, move_name.c_str());
        mslot_c->move[move_name.size()] = '\0';

        mslot_c->pp = mslot_cpp.pp;
    }

    void pkmn_move_slots_cpp_to_c(
        const pkmn::move_slots_t &mslots_cpp,
        pkmn_move_slots_t* mslots_c
    ) {
        mslots_c->move_slots = (pkmn_move_slot_t*)std::malloc(sizeof(pkmn_move_slot_t) * mslots_cpp.size());
        for(size_t i = 0; i < mslots_cpp.size(); ++i) {
            pkmn_move_slot_cpp_to_c(
                mslots_cpp[i],
                &(mslots_c->move_slots[i])
            );
        }
        mslots_c->length = mslots_cpp.size();
    }

    void pkmn_pokemon_entries_to_string_list(
        const pkmn::database::pokemon_entries_t &pokemon_entries,
        pkmn_string_list_t* string_list_out
    ) {
        string_list_out->strings = (char**)std::malloc(sizeof(char*) * pokemon_entries.size());
        for(size_t i = 0; i < pokemon_entries.size(); ++i) {
            std::string move_name = pokemon_entries[i].get_name();
            string_list_out->strings[i] = (char*)std::malloc(move_name.size() + 1);
            std::strcpy(string_list_out->strings[i], move_name.c_str());
            string_list_out->strings[i][move_name.size()] = '\0';
        }

        string_list_out->length = pokemon_entries.size();
    }

    void pkmn_pokemon_list_cpp_to_c(
        const pkmn::pokemon_list_t &pokemon_list_cpp,
        pkmn_pokemon_list_t* pokemon_list_c
    ) {
        pokemon_list_c->pokemon_list = (pkmn_pokemon_handle_t*)std::malloc(sizeof(pkmn_pokemon_handle_t) * pokemon_list_cpp.size());
        for(size_t i = 0; i < pokemon_list_cpp.size(); ++i) {
            pokemon_list_c->pokemon_list[i] = new pkmn_pokemon_t;
            pokemon_list_c->pokemon_list[i]->cpp = pokemon_list_cpp.at(i);
            pokemon_list_c->pokemon_list[i]->last_error = "None";
        }

        pokemon_list_c->length = pokemon_list_cpp.size();
    }

    void pkmn_pokemon_box_list_cpp_to_c(
        const pkmn::pokemon_box_list_t &pokemon_box_list_cpp,
        pkmn_pokemon_box_list_t* pokemon_box_list_c
    ) {
        pokemon_box_list_c->pokemon_boxes = (pkmn_pokemon_box_handle_t*)std::malloc(sizeof(pkmn_pokemon_box_handle_t) * pokemon_box_list_cpp.size());
        for(size_t i = 0; i < pokemon_box_list_cpp.size(); ++i) {
            pokemon_box_list_c->pokemon_boxes[i] = new pkmn_pokemon_box_t;
            pokemon_box_list_c->pokemon_boxes[i]->cpp = pokemon_box_list_cpp.at(i);
            pokemon_box_list_c->pokemon_boxes[i]->last_error = "None";
        }

        pokemon_box_list_c->length = pokemon_box_list_cpp.size();
    }

    void std_pair_std_string_to_string_pair(
        const std::pair<std::string, std::string> &cpp_pair,
        pkmn_string_pair_t* c_pair_out
    ) {
        c_pair_out->first = (char*)std::malloc(cpp_pair.first.size() + 1);
        std::strcpy(c_pair_out->first, cpp_pair.first.c_str());
        c_pair_out->first[cpp_pair.first.size()] = '\0';

        c_pair_out->second = (char*)std::malloc(cpp_pair.second.size() + 1);
        std::strcpy(c_pair_out->second, cpp_pair.second.c_str());
        c_pair_out->second[cpp_pair.second.size()] = '\0';
    }

    void std_vector_std_string_to_string_list(
        const std::vector<std::string> &vec,
        pkmn_string_list_t* string_list_out
    ) {
        string_list_out->strings = (char**)std::malloc(sizeof(char*) * vec.size());
        for(size_t i = 0; i < vec.size(); ++i) {
            string_list_out->strings[i] = (char*)std::malloc(vec[i].size() + 1);
            std::strcpy(string_list_out->strings[i], vec[i].c_str());
            string_list_out->strings[i][vec[i].size()] = '\0';
        }

        string_list_out->length = vec.size();
    }

}

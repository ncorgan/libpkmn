/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"

namespace pkmn {

    void pkmn_hidden_power_cpp_to_c(
        const pkmn::calculations::hidden_power_t &hp_cpp,
        pkmn_hidden_power_t* hp_c
    ) {
        hp_c->type = (char*)std::malloc(hp_cpp.type.size() + 1);
        std::strcpy(hp_c->type, hp_cpp.type.c_str());
        hp_c->type[hp_cpp.type.size()] = '\0';

        hp_c->base_power = hp_cpp.base_power;
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
        pkmn_item_slots_t* islots_c,
        size_t* list_length_out
    ) {
        *islots_c = (pkmn_item_slots_t)std::malloc(sizeof(pkmn_item_slot_t) * islots_cpp.size());
        for(size_t i = 0; i < islots_cpp.size(); ++i) {
            pkmn_item_slot_cpp_to_c(
                islots_cpp[i],
                &((*islots_c)[i])
            );
        }
        *list_length_out = islots_cpp.size();
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
        pkmn_levelup_moves_t* lmoves_c,
        size_t* list_length_out
    ) {
        *lmoves_c = (pkmn_levelup_moves_t)std::malloc(sizeof(pkmn_levelup_move_t) * lmoves_cpp.size());
        for(size_t i = 0; i < lmoves_cpp.size(); ++i) {
            pkmn_levelup_move_cpp_to_c(
                lmoves_cpp[i],
                &((*lmoves_c)[i])
            );
        }
        *list_length_out = lmoves_cpp.size();
    }

    void pkmn_move_list_to_string_list(
        const pkmn::database::move_list_t &move_list,
        pkmn_string_list_t* string_list_out,
        size_t* list_length_out
    ) {
        *string_list_out = (pkmn_string_list_t)std::malloc(sizeof(char*) * move_list.size());
        for(size_t i = 0; i < move_list.size(); ++i) {
            std::string move_name = move_list[i].get_name();
            (*string_list_out)[i] = (char*)std::malloc(move_name.size() + 1);
            std::strcpy((*string_list_out)[i], move_name.c_str());
            (*string_list_out)[i][move_name.size()] = '\0';
        }
        *list_length_out = move_list.size();
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
        pkmn_move_slots_t* mslots_c,
        size_t* list_length_out
    ) {
        *mslots_c = (pkmn_move_slots_t)std::malloc(sizeof(pkmn_move_slot_t) * mslots_cpp.size());
        for(size_t i = 0; i < mslots_cpp.size(); ++i) {
            pkmn_move_slot_cpp_to_c(
                mslots_cpp[i],
                &((*mslots_c)[i])
            );
        }
        *list_length_out = mslots_cpp.size();
    }

    void pkmn_pokemon_entries_to_string_list(
        const pkmn::database::pokemon_entries_t &pokemon_entries,
        pkmn_string_list_t* string_list_out,
        size_t* list_length_out
    ) {
        *string_list_out = (pkmn_string_list_t)std::malloc(sizeof(char*) * pokemon_entries.size());
        for(size_t i = 0; i < pokemon_entries.size(); ++i) {
            std::string move_name = pokemon_entries[i].get_name();
            (*string_list_out)[i] = (char*)std::malloc(move_name.size() + 1);
            std::strcpy((*string_list_out)[i], move_name.c_str());
            (*string_list_out)[i][move_name.size()] = '\0';
        }
        *list_length_out = pokemon_entries.size();
    }

    void pkmn_pokemon_list_cpp_to_c(
        const pkmn::pokemon_list_t &pokemon_list_cpp,
        pkmn_pokemon_list_t* pokemon_list_c,
        size_t* list_length_out
    ) {
        *pokemon_list_c = (pkmn_pokemon_list_t)std::malloc(sizeof(pkmn_pokemon_handle_t) * pokemon_list_cpp.size());
        for(size_t i = 0; i < pokemon_list_cpp.size(); ++i) {
            (*pokemon_list_c)[i] = new pkmn_pokemon_t;
            (*pokemon_list_c)[i]->cpp = pokemon_list_cpp.at(i);
            (*pokemon_list_c)[i]->last_error = "None";
        }
        *list_length_out = pokemon_list_cpp.size();
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
        pkmn_string_list_t* string_list_out,
        size_t* list_length_out
    ) {
        *string_list_out = (pkmn_string_list_t)std::malloc(sizeof(char*) * vec.size());
        for(size_t i = 0; i < vec.size(); ++i) {
            (*string_list_out)[i] = (char*)std::malloc(vec[i].size() + 1);
            std::strcpy((*string_list_out)[i], vec[i].c_str());
            (*string_list_out)[i][vec[i].size()] = '\0';
        }
        *list_length_out = vec.size();
    }

}

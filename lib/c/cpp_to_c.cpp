/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"

#include <boost/assert.hpp>

namespace pkmn { namespace c {

    void string_cpp_to_c(
        const std::string& string_cpp,
        char* c_str_ptr,
        size_t buffer_len,
        size_t* string_length_out
    )
    {
        BOOST_ASSERT(c_str_ptr);

        if(!string_cpp.empty())
        {
            // cppcheck-suppress redundantCopy
            std::memset(
                c_str_ptr,
                0,
                buffer_len
            );
            std::strncpy(
                c_str_ptr,
                string_cpp.c_str(),
                buffer_len
            );

            if(string_length_out)
            {
                *string_length_out = string_cpp.size();
            }
        }
    }

    void string_list_cpp_to_c(
        const std::vector<std::string>& string_list_cpp,
        pkmn_string_list_t* string_list_c_ptr
    )
    {
        BOOST_ASSERT(string_list_c_ptr);

        if(!string_list_cpp.empty())
        {
            string_list_c_ptr->strings = (char**)std::calloc(
                                                     string_list_cpp.size(),
                                                     sizeof(char*)
                                                 );
            for(size_t index = 0; index < string_list_cpp.size(); ++index)
            {
                string_cpp_to_c_alloc(
                    string_list_cpp[index],
                    &string_list_c_ptr->strings[index]
                );
            }
        }

        string_list_c_ptr->length = string_list_cpp.size();
    }

    void item_slots_cpp_to_c(
        const pkmn::item_slots_t& item_slots_cpp,
        pkmn_item_slots_t* item_slots_c_ptr
    )
    {
        BOOST_ASSERT(item_slots_c_ptr);

        size_t num_slots = item_slots_cpp.size();

        if(num_slots > 0)
        {
            item_slots_c_ptr->item_slots = (pkmn_item_slot_t*)std::calloc(
                                                                  sizeof(pkmn_item_slot_t),
                                                                  num_slots
                                                              );
            for(size_t index = 0; index < num_slots; ++index)
            {
                string_cpp_to_c_alloc(
                    item_slots_cpp[index].item,
                    &item_slots_c_ptr->item_slots[index].item
                );
                item_slots_c_ptr->item_slots[index].amount = item_slots_cpp[index].amount;
            }
        }

        item_slots_c_ptr->length = num_slots;
    }

    void levelup_moves_cpp_to_c(
        const pkmn::database::levelup_moves_t& levelup_moves_cpp,
        pkmn_levelup_moves_t* levelup_moves_c_ptr
    )
    {
        BOOST_ASSERT(levelup_moves_c_ptr);

        size_t num_levelup_moves = levelup_moves_cpp.size();

        if(num_levelup_moves > 0)
        {
            levelup_moves_c_ptr->levelup_moves = (pkmn_levelup_move_t*)std::calloc(
                                                                           num_levelup_moves,
                                                                           sizeof(pkmn_levelup_move_t)
                                                                       );
            for(size_t index = 0; index < num_levelup_moves; ++index)
            {
                levelup_move_cpp_to_c(
                    levelup_moves_cpp[index],
                    &levelup_moves_c_ptr->levelup_moves[index]
                );
            }
        }

        levelup_moves_c_ptr->length = num_levelup_moves;
    }

    void move_list_to_string_list(
        const pkmn::database::move_list_t& move_list,
        pkmn_string_list_t* string_list_ptr
    )
    {
        BOOST_ASSERT(string_list_ptr);

        size_t num_moves = move_list.size();
        if(num_moves > 0)
        {
            string_list_ptr->strings = (char**)std::calloc(
                                                   move_list.size(),
                                                   sizeof(char*)
                                               );
            for(size_t index = 0; index < num_moves; ++index)
            {
                string_cpp_to_c_alloc(
                    move_list[index].get_name(),
                    &string_list_ptr->strings[index]
                );
            }
        }

        string_list_ptr->length = num_moves;
    }

    void move_slots_cpp_to_c(
        const pkmn::move_slots_t& move_slots_cpp,
        pkmn_move_slots_t* move_slots_c_ptr
    )
    {
        BOOST_ASSERT(move_slots_c_ptr);

        size_t num_slots = move_slots_cpp.size();

        if(num_slots > 0)
        {
            move_slots_c_ptr->move_slots = (pkmn_move_slot_t*)std::calloc(
                                                                  sizeof(pkmn_move_slot_t),
                                                                  num_slots
                                                              );
            for(size_t index = 0; index < num_slots; ++index)
            {
                string_cpp_to_c_alloc(
                    move_slots_cpp[index].move,
                    &move_slots_c_ptr->move_slots[index].move
                );
                move_slots_c_ptr->move_slots[index].pp = move_slots_cpp[index].pp;
            }
        }

        move_slots_c_ptr->length = num_slots;
    }

    void pokemon_entries_to_string_list(
        const pkmn::database::pokemon_entries_t& pokemon_entries,
        pkmn_string_list_t* string_list_ptr
    )
    {
        BOOST_ASSERT(string_list_ptr);

        size_t num_moves = pokemon_entries.size();
        if(num_moves > 0)
        {
            string_list_ptr->strings = (char**)std::calloc(
                                                   pokemon_entries.size(),
                                                   sizeof(char*)
                                               );
            for(size_t index = 0; index < num_moves; ++index)
            {
                string_cpp_to_c_alloc(
                    pokemon_entries[index].get_name(),
                    &string_list_ptr->strings[index]
                );
            }
        }

        string_list_ptr->length = num_moves;
    }

    void item_entry_cpp_to_c(
        const pkmn::database::item_entry& item_entry_cpp,
        pkmn_database_item_entry_t* item_entry_c_ptr
    )
    {
        BOOST_ASSERT(item_entry_c_ptr);

        string_cpp_to_c_alloc(
            item_entry_cpp.get_name(),
            &item_entry_c_ptr->name
        );
        string_cpp_to_c_alloc(
            item_entry_cpp.get_game(),
            &item_entry_c_ptr->game
        );
        string_cpp_to_c_alloc(
            item_entry_cpp.get_category(),
            &item_entry_c_ptr->category
        );
        string_cpp_to_c_alloc(
            item_entry_cpp.get_pocket(),
            &item_entry_c_ptr->pocket
        );
        string_cpp_to_c_alloc(
            item_entry_cpp.get_description(),
            &item_entry_c_ptr->description
        );
        string_cpp_to_c_alloc(
            item_entry_cpp.get_fling_effect(),
            &item_entry_c_ptr->fling_effect
        );

        item_entry_c_ptr->cost = item_entry_cpp.get_cost();
        item_entry_c_ptr->holdable = item_entry_cpp.holdable();
        item_entry_c_ptr->fling_power = item_entry_cpp.get_fling_power();
    }

    void move_entry_cpp_to_c(
        const pkmn::database::move_entry& move_entry_cpp,
        pkmn_database_move_entry_t* move_entry_c_ptr
    )
    {
        BOOST_ASSERT(move_entry_c_ptr);

        string_cpp_to_c_alloc(
            move_entry_cpp.get_name(),
            &move_entry_c_ptr->name
        );
        string_cpp_to_c_alloc(
            move_entry_cpp.get_game(),
            &move_entry_c_ptr->game
        );
        string_cpp_to_c_alloc(
            move_entry_cpp.get_description(),
            &move_entry_c_ptr->description
        );
        string_cpp_to_c_alloc(
            move_entry_cpp.get_target(),
            &move_entry_c_ptr->target
        );
        string_cpp_to_c_alloc(
            move_entry_cpp.get_damage_class(),
            &move_entry_c_ptr->damage_class
        );
        string_cpp_to_c_alloc(
            move_entry_cpp.get_effect(),
            &move_entry_c_ptr->effect
        );
        string_cpp_to_c_alloc(
            move_entry_cpp.get_contest_type(),
            &move_entry_c_ptr->contest_type
        );
        string_cpp_to_c_alloc(
            move_entry_cpp.get_contest_effect(),
            &move_entry_c_ptr->contest_effect
        );
        string_cpp_to_c_alloc(
            move_entry_cpp.get_super_contest_effect(),
            &move_entry_c_ptr->super_contest_effect
        );

        move_entry_c_ptr->base_power = move_entry_cpp.get_base_power();
        for(int i = 0; i < 4; ++i) {
            move_entry_c_ptr->pp[i] = move_entry_cpp.get_pp(i);
        }
        move_entry_c_ptr->accuracy = move_entry_cpp.get_accuracy();
    }

    void pokemon_entry_cpp_to_c(
        const pkmn::database::pokemon_entry& pokemon_entry_cpp,
        pkmn_database_pokemon_entry_t* pokemon_entry_c_ptr
    )
    {
        BOOST_ASSERT(pokemon_entry_c_ptr);

        string_cpp_to_c_alloc(
            pokemon_entry_cpp.get_name(),
            &pokemon_entry_c_ptr->name
        );
        string_cpp_to_c_alloc(
            pokemon_entry_cpp.get_game(),
            &pokemon_entry_c_ptr->game
        );
        string_cpp_to_c_alloc(
            pokemon_entry_cpp.get_form(),
            &pokemon_entry_c_ptr->form
        );
        string_cpp_to_c_alloc(
            pokemon_entry_cpp.get_species(),
            &pokemon_entry_c_ptr->species
        );
        string_cpp_to_c_alloc(
            pokemon_entry_cpp.get_pokedex_entry(),
            &pokemon_entry_c_ptr->pokedex_entry
        );
        string_pair_cpp_to_c(
            pokemon_entry_cpp.get_types(),
            &pokemon_entry_c_ptr->types
        );
        string_cpp_to_c_alloc(
            pokemon_entry_cpp.get_hidden_ability(),
            &pokemon_entry_c_ptr->hidden_ability
        );

        string_pair_cpp_to_c(
            pokemon_entry_cpp.get_abilities(),
            &pokemon_entry_c_ptr->abilities
        );
        string_pair_cpp_to_c(
            pokemon_entry_cpp.get_egg_groups(),
            &pokemon_entry_c_ptr->egg_groups
        );

        pokemon_entry_c_ptr->height = pokemon_entry_cpp.get_height();
        pokemon_entry_c_ptr->weight = pokemon_entry_cpp.get_weight();
        pokemon_entry_c_ptr->chance_male = pokemon_entry_cpp.get_chance_male();
        pokemon_entry_c_ptr->chance_female = pokemon_entry_cpp.get_chance_female();
        pokemon_entry_c_ptr->has_gender_differences = pokemon_entry_cpp.has_gender_differences();
        pokemon_entry_c_ptr->base_friendship = pokemon_entry_cpp.get_base_friendship();
        pokemon_entry_c_ptr->experience_yield = pokemon_entry_cpp.get_experience_yield();

        std::map<std::string, int> base_stats = pokemon_entry_cpp.get_base_stats();
        pokemon_entry_c_ptr->base_stats[PKMN_STAT_HP] = base_stats.at("HP");
        pokemon_entry_c_ptr->base_stats[PKMN_STAT_ATTACK] = base_stats.at("Attack");
        pokemon_entry_c_ptr->base_stats[PKMN_STAT_DEFENSE] = base_stats.at("Defense");
        pokemon_entry_c_ptr->base_stats[PKMN_STAT_SPEED] = base_stats.at("Speed");
        if(base_stats.count("Special") > 0) {
            pokemon_entry_c_ptr->base_stats[PKMN_STAT_SPECIAL] = base_stats.at("Special");
            pokemon_entry_c_ptr->base_stats[PKMN_STAT_SPATK] = -1;
            pokemon_entry_c_ptr->base_stats[PKMN_STAT_SPDEF] = -1;
        } else {
            pokemon_entry_c_ptr->base_stats[PKMN_STAT_SPECIAL] = -1;
            pokemon_entry_c_ptr->base_stats[PKMN_STAT_SPATK] = base_stats.at("Special Attack");
            pokemon_entry_c_ptr->base_stats[PKMN_STAT_SPDEF] = base_stats.at("Special Defense");
        }

        std::map<std::string, int> EV_yields = pokemon_entry_cpp.get_EV_yields();
        pokemon_entry_c_ptr->EV_yields[PKMN_STAT_HP] = EV_yields.at("HP");
        pokemon_entry_c_ptr->EV_yields[PKMN_STAT_ATTACK] = EV_yields.at("Attack");
        pokemon_entry_c_ptr->EV_yields[PKMN_STAT_DEFENSE] = EV_yields.at("Defense");
        pokemon_entry_c_ptr->EV_yields[PKMN_STAT_SPEED] = EV_yields.at("Speed");
        if(EV_yields.count("Special") > 0) {
            pokemon_entry_c_ptr->EV_yields[PKMN_STAT_SPECIAL] = EV_yields.at("Special");
            pokemon_entry_c_ptr->EV_yields[PKMN_STAT_SPATK] = -1;
            pokemon_entry_c_ptr->EV_yields[PKMN_STAT_SPDEF] = -1;
        } else {
            pokemon_entry_c_ptr->EV_yields[PKMN_STAT_SPECIAL] = -1;
            pokemon_entry_c_ptr->EV_yields[PKMN_STAT_SPATK] = EV_yields.at("Special Attack");
            pokemon_entry_c_ptr->EV_yields[PKMN_STAT_SPDEF] = EV_yields.at("Special Defense");
        }

        levelup_moves_cpp_to_c(
            pokemon_entry_cpp.get_levelup_moves(),
            &pokemon_entry_c_ptr->levelup_moves
        );

        move_list_to_string_list(
            pokemon_entry_cpp.get_tm_hm_moves(),
            &pokemon_entry_c_ptr->tm_hm_moves
        );
        move_list_to_string_list(
            pokemon_entry_cpp.get_egg_moves(),
            &pokemon_entry_c_ptr->egg_moves
        );
        move_list_to_string_list(
            pokemon_entry_cpp.get_tutor_moves(),
            &pokemon_entry_c_ptr->tutor_moves
        );
        string_list_cpp_to_c(
            pokemon_entry_cpp.get_forms(),
            &pokemon_entry_c_ptr->forms
        );
        pokemon_entries_to_string_list(
            pokemon_entry_cpp.get_evolutions(),
            &pokemon_entry_c_ptr->evolutions
        );
    }

    void pokemon_list_cpp_to_c(
        const pkmn::pokemon_list_t& pokemon_list_cpp,
        pkmn_pokemon_list_t* pokemon_list_c_ptr
    )
    {
        BOOST_ASSERT(pokemon_list_c_ptr);

        size_t num_pokemon = pokemon_list_cpp.size();

        if(num_pokemon > 0)
        {
            pokemon_list_c_ptr->pokemon_list = (pkmn_pokemon_handle_t*)std::calloc(
                                                                           num_pokemon,
                                                                           sizeof(pkmn_pokemon_handle_t)
                                                                       );
            for(size_t index = 0; index < num_pokemon; ++index)
            {
                pokemon_list_c_ptr->pokemon_list[index] = new pkmn_pokemon_t;
                pokemon_list_c_ptr->pokemon_list[index]->cpp = pokemon_list_cpp.at(index);
                pokemon_list_c_ptr->pokemon_list[index]->last_error = "None";
            }
        }

        pokemon_list_c_ptr->length = num_pokemon;
    }

    void pokemon_box_list_cpp_to_c(
        const pkmn::pokemon_box_list_t& pokemon_box_list_cpp,
        pkmn_pokemon_box_list_t* pokemon_box_list_c_ptr
    )
    {
        BOOST_ASSERT(pokemon_box_list_c_ptr);

        size_t num_boxes = pokemon_box_list_cpp.size();

        if(num_boxes > 0)
        {
            pokemon_box_list_c_ptr->pokemon_boxes = (pkmn_pokemon_box_handle_t*)std::calloc(
                                                                                    num_boxes,
                                                                                    sizeof(pkmn_pokemon_box_handle_t)
                                                                                );
            for(size_t index = 0; index < num_boxes; ++index)
            {
                pokemon_box_list_c_ptr->pokemon_boxes[index] = new pkmn_pokemon_box_t;
                pokemon_box_list_c_ptr->pokemon_boxes[index]->cpp = pokemon_box_list_cpp.at(index);
                pokemon_box_list_c_ptr->pokemon_boxes[index]->last_error = "None";
            }
        }

        pokemon_box_list_c_ptr->length = num_boxes;
    }
}}

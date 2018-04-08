/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"

#include "private_exports.hpp"

#include <boost/assert.hpp>

/*
 * Note that despite pkmn_*_t being C structs, we can't use designated
 * initializers in C++.
 */

namespace pkmn { namespace c {

    // LibPKMN wrapper class initialization.

    void init_item_bag(
        const pkmn::item_bag::sptr& cpp_item_bag,
        struct pkmn_item_bag* item_bag_ptr
    )
    {
        BOOST_ASSERT(item_bag_ptr != nullptr);
        BOOST_ASSERT(cpp_item_bag.get() != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_item_bag temp_item_bag =
        {
            nullptr, // game
            // pocket_names
            {
                nullptr, // strings
                0        // length
            },
            nullptr // _internal
        };

        string_cpp_to_c_alloc(
            cpp_item_bag->get_game(),
            &temp_item_bag.game
        );
        string_list_cpp_to_c(
            cpp_item_bag->get_pocket_names(),
            &temp_item_bag.pocket_names
        );

        temp_item_bag._internal = new pkmn_item_bag_internal_t;
        pkmn_item_bag_internal_t* internal_ptr = ITEM_BAG_INTERNAL_RCAST(temp_item_bag._internal);
        internal_ptr->cpp = cpp_item_bag;
        internal_ptr->last_error = "None";

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *item_bag_ptr = std::move(temp_item_bag);
    }

    void init_item_list(
        const pkmn::item_list::sptr& cpp_item_list,
        struct pkmn_item_list* item_list_ptr
    )
    {
        BOOST_ASSERT(item_list_ptr != nullptr);
        BOOST_ASSERT(cpp_item_list.get() != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_item_list temp_item_list =
        {
            nullptr, // name
            nullptr, // game
            0ULL,    // capacity
            nullptr  // _internal
        };

        string_cpp_to_c_alloc(
            cpp_item_list->get_name(),
            &temp_item_list.name
        );
        string_cpp_to_c_alloc(
            cpp_item_list->get_game(),
            &temp_item_list.game
        );
        temp_item_list.capacity = cpp_item_list->get_capacity();

        temp_item_list._internal = new pkmn_item_list_internal_t;
        pkmn_item_list_internal_t* internal_ptr = ITEM_LIST_INTERNAL_RCAST(temp_item_list._internal);
        internal_ptr->cpp = cpp_item_list;
        internal_ptr->last_error = "None";

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *item_list_ptr = std::move(temp_item_list);
    }

    void init_pokedex(
        const pkmn::pokedex::sptr& cpp_pokedex,
        struct pkmn_pokedex* pokedex_ptr
    )
    {
        BOOST_ASSERT(pokedex_ptr != nullptr);
        BOOST_ASSERT(cpp_pokedex.get() != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_pokedex temp_pokedex =
        {
            nullptr, // game
            nullptr // _internal
        };

        string_cpp_to_c_alloc(
            cpp_pokedex->get_game(),
            &temp_pokedex.game
        );

        temp_pokedex._internal = new pkmn_pokedex_internal_t;
        pkmn_pokedex_internal_t* internal_ptr = POKEDEX_INTERNAL_RCAST(temp_pokedex._internal);
        internal_ptr->cpp = cpp_pokedex;
        internal_ptr->last_error = "None";

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *pokedex_ptr = std::move(temp_pokedex);
    }

    void init_pokemon(
        const pkmn::pokemon::sptr& cpp_pokemon,
        struct pkmn_pokemon* pokemon_ptr
    )
    {
        BOOST_ASSERT(pokemon_ptr != nullptr);
        BOOST_ASSERT(cpp_pokemon.get() != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_pokemon temp_pokemon =
        {
            nullptr, // species
            nullptr, // game
            nullptr // _internal
        };

        string_cpp_to_c_alloc(
            cpp_pokemon->get_species(),
            &temp_pokemon.species
        );
        string_cpp_to_c_alloc(
            cpp_pokemon->get_game(),
            &temp_pokemon.game
        );

        temp_pokemon._internal = new pkmn_pokemon_internal_t;
        pkmn_pokemon_internal_t* internal_ptr = POKEMON_INTERNAL_RCAST(temp_pokemon._internal);
        internal_ptr->cpp = cpp_pokemon;
        internal_ptr->last_error = "None";
        internal_ptr->generation = pkmn::priv::game_name_to_generation(
                                       temp_pokemon.game
                                   );

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *pokemon_ptr = std::move(temp_pokemon);
    }

    void init_pokemon_box(
        const pkmn::pokemon_box::sptr& cpp_pokemon_box,
        struct pkmn_pokemon_box* pokemon_box_ptr
    )
    {
        BOOST_ASSERT(pokemon_box_ptr != nullptr);
        BOOST_ASSERT(cpp_pokemon_box.get() != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_pokemon_box temp_pokemon_box =
        {
            nullptr, // game
            0ULL,    // capacity
            nullptr  // _internal
        };

        string_cpp_to_c_alloc(
            cpp_pokemon_box->get_game(),
            &temp_pokemon_box.game
        );
        temp_pokemon_box.capacity = cpp_pokemon_box->get_capacity();

        temp_pokemon_box._internal = new pkmn_pokemon_box_internal_t;
        pkmn_pokemon_box_internal_t* internal_ptr = POKEMON_BOX_INTERNAL_RCAST(temp_pokemon_box._internal);
        internal_ptr->cpp = cpp_pokemon_box;
        internal_ptr->last_error = "None";

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *pokemon_box_ptr = std::move(temp_pokemon_box);
    }

    void init_pokemon_party(
        const pkmn::pokemon_party::sptr& cpp_pokemon_party,
        struct pkmn_pokemon_party* pokemon_party_ptr
    )
    {
        BOOST_ASSERT(pokemon_party_ptr != nullptr);
        BOOST_ASSERT(cpp_pokemon_party.get() != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_pokemon_party temp_pokemon_party =
        {
            nullptr, // game
            0ULL,    // capacity
            nullptr  // _internal
        };

        string_cpp_to_c_alloc(
            cpp_pokemon_party->get_game(),
            &temp_pokemon_party.game
        );
        temp_pokemon_party.capacity = 6ULL;

        temp_pokemon_party._internal = new pkmn_pokemon_party_internal_t;
        pkmn_pokemon_party_internal_t* internal_ptr = POKEMON_PARTY_INTERNAL_RCAST(temp_pokemon_party._internal);
        internal_ptr->cpp = cpp_pokemon_party;
        internal_ptr->last_error = "None";

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *pokemon_party_ptr = std::move(temp_pokemon_party);
    }

    void init_pokemon_pc(
        const pkmn::pokemon_pc::sptr& cpp_pokemon_pc,
        struct pkmn_pokemon_pc* pokemon_pc_ptr
    )
    {
        BOOST_ASSERT(pokemon_pc_ptr != nullptr);
        BOOST_ASSERT(cpp_pokemon_pc.get() != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_pokemon_pc temp_pokemon_pc =
        {
            nullptr, // game
            0ULL,    // capacity
            nullptr  // _internal
        };

        string_cpp_to_c_alloc(
            cpp_pokemon_pc->get_game(),
            &temp_pokemon_pc.game
        );
        temp_pokemon_pc.capacity = cpp_pokemon_pc->get_num_boxes();

        temp_pokemon_pc._internal = new pkmn_pokemon_pc_internal_t;
        pkmn_pokemon_pc_internal_t* internal_ptr = POKEMON_PC_INTERNAL_RCAST(temp_pokemon_pc._internal);
        internal_ptr->cpp = cpp_pokemon_pc;
        internal_ptr->last_error = "None";

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *pokemon_pc_ptr = std::move(temp_pokemon_pc);
    }

    void init_game_save(
        const pkmn::game_save::sptr& cpp_game_save,
        struct pkmn_game_save* game_save_ptr
    )
    {
        BOOST_ASSERT(game_save_ptr != nullptr);
        BOOST_ASSERT(cpp_game_save.get() != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_game_save temp_game_save =
        {
            nullptr, // game
            nullptr  // _internal
        };

        string_cpp_to_c_alloc(
            cpp_game_save->get_game(),
            &temp_game_save.game
        );

        temp_game_save._internal = new pkmn_game_save_internal_t;
        pkmn_game_save_internal_t* internal_ptr = GAME_SAVE_INTERNAL_RCAST(temp_game_save._internal);
        internal_ptr->cpp = cpp_game_save;
        internal_ptr->last_error = "None";
        internal_ptr->generation = pkmn::priv::game_name_to_generation(
                                       temp_game_save.game
                                   );

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *game_save_ptr = std::move(temp_game_save);
    }

    // Helper classes

    void string_cpp_to_c(
        const std::string& string_cpp,
        char* c_str_ptr,
        size_t buffer_len,
        size_t* string_length_out
    )
    {
        BOOST_ASSERT(c_str_ptr);

        size_t string_length = string_cpp.size();

        if(!string_cpp.empty())
        {
            std::strncpy(
                c_str_ptr,
                string_cpp.c_str(),
                buffer_len
            );

            // Null-terminate if we can
            if(string_length < buffer_len)
            {
                c_str_ptr[string_length] = '\0';
            }
        }

        if(string_length_out)
        {
            *string_length_out = string_length;
        }
    }

    void string_list_cpp_to_c(
        const std::vector<std::string>& string_list_cpp,
        struct pkmn_string_list* string_list_c_ptr
    )
    {
        BOOST_ASSERT(string_list_c_ptr != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_string_list temp_string_list =
        {
            nullptr,               // strings
            string_list_cpp.size() // length
        };

        if(temp_string_list.length > 0)
        {
            temp_string_list.strings = (char**)std::calloc(
                                                   string_list_cpp.size(),
                                                   sizeof(char*)
                                               );
            for(size_t string_index = 0;
                string_index < temp_string_list.length;
                ++string_index)
            {
                string_cpp_to_c_alloc(
                    string_list_cpp[string_index],
                    &temp_string_list.strings[string_index]
                );
            }
        }

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *string_list_c_ptr = std::move(temp_string_list);
    }

    void item_slots_cpp_to_c(
        const pkmn::item_slots_t& item_slots_cpp,
        struct pkmn_item_slots* item_slots_c_ptr
    )
    {
        BOOST_ASSERT(item_slots_c_ptr != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_item_slots temp_item_slots =
        {
            nullptr,              // item_slots
            item_slots_cpp.size() // length
        };

        if(temp_item_slots.length > 0)
        {
            temp_item_slots.item_slots = (struct pkmn_item_slot*)std::calloc(
                                                                item_slots_cpp.size(),
                                                                sizeof(struct pkmn_item_slot)
                                                            );
            for(size_t item_slot_index = 0;
                item_slot_index < temp_item_slots.length;
                ++item_slot_index)
            {
                item_slot_cpp_to_c(
                    item_slots_cpp[item_slot_index],
                    &temp_item_slots.item_slots[item_slot_index]
                );
            }
        }

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *item_slots_c_ptr = std::move(temp_item_slots);
    }

    void levelup_moves_cpp_to_c(
        const pkmn::database::levelup_moves_t& levelup_moves_cpp,
        struct pkmn_levelup_moves* levelup_moves_c_ptr
    )
    {
        BOOST_ASSERT(levelup_moves_c_ptr != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_levelup_moves temp_levelup_moves =
        {
            nullptr,                 // levelup_moves
            levelup_moves_cpp.size() // length
        };

        if(temp_levelup_moves.length > 0)
        {
            temp_levelup_moves.levelup_moves = (struct pkmn_levelup_move*)std::calloc(
                                                                         levelup_moves_cpp.size(),
                                                                         sizeof(struct pkmn_levelup_move)
                                                                     );
            for(size_t levelup_move_index = 0;
                levelup_move_index < temp_levelup_moves.length;
                ++levelup_move_index)
            {
                levelup_move_cpp_to_c(
                    levelup_moves_cpp[levelup_move_index],
                    &temp_levelup_moves.levelup_moves[levelup_move_index]
                );
            }
        }

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *levelup_moves_c_ptr = std::move(temp_levelup_moves);
    }

    void move_list_to_string_list(
        const pkmn::database::move_list_t& move_list,
        struct pkmn_string_list* string_list_ptr
    )
    {
        BOOST_ASSERT(string_list_ptr != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_string_list temp_string_list =
        {
            nullptr,         // strings
            move_list.size() // length
        };

        if(temp_string_list.length > 0)
        {
            temp_string_list.strings = (char**)std::calloc(
                                                   move_list.size(),
                                                   sizeof(char*)
                                               );
            for(size_t move_index = 0;
                move_index < temp_string_list.length;
                ++move_index)
            {
                string_cpp_to_c_alloc(
                    move_list[move_index].get_name(),
                    &temp_string_list.strings[move_index]
                );
            }
        }

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *string_list_ptr = std::move(temp_string_list);
    }

    void move_slots_cpp_to_c(
        const pkmn::move_slots_t& move_slots_cpp,
        struct pkmn_move_slots* move_slots_c_ptr
    )
    {
        BOOST_ASSERT(move_slots_c_ptr != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_move_slots temp_move_slots =
        {
            nullptr,              // move_slots
            move_slots_cpp.size() // length
        };

        if(temp_move_slots.length > 0)
        {
            temp_move_slots.move_slots = (struct pkmn_move_slot*)std::calloc(
                                                                 move_slots_cpp.size(),
                                                                 sizeof(struct pkmn_move_slot)
                                                            );
            for(size_t move_slot_index = 0;
                move_slot_index < temp_move_slots.length;
                ++move_slot_index)
            {
                move_slot_cpp_to_c(
                    move_slots_cpp[move_slot_index],
                    &temp_move_slots.move_slots[move_slot_index]
                );
            }
        }

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *move_slots_c_ptr = std::move(temp_move_slots);
    }

    void pokemon_entries_to_string_list(
        const pkmn::database::pokemon_entries_t& pokemon_entries,
        struct pkmn_string_list* string_list_ptr
    )
    {
        BOOST_ASSERT(string_list_ptr != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_string_list temp_string_list =
        {
            nullptr,               // strings
            pokemon_entries.size() // length
        };

        if(temp_string_list.length > 0)
        {
            temp_string_list.strings = (char**)std::calloc(
                                                   pokemon_entries.size(),
                                                   sizeof(char*)
                                               );
            for(size_t entry_index = 0;
                entry_index < temp_string_list.length;
                ++entry_index)
            {
                string_cpp_to_c_alloc(
                    pokemon_entries[entry_index].get_name(),
                    &temp_string_list.strings[entry_index]
                );
            }
        }

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *string_list_ptr = std::move(temp_string_list);
    }

    void item_entry_cpp_to_c(
        const pkmn::database::item_entry& item_entry_cpp,
        struct pkmn_database_item_entry* item_entry_c_ptr
    )
    {
        BOOST_ASSERT(item_entry_c_ptr != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_database_item_entry temp_item_entry =
        {
            nullptr, // name
            nullptr, // game
            nullptr, // category
            nullptr, // pocket
            nullptr, // description
            0,       // cost
            false,   // holdable
            0,       // fling_power
            nullptr  // fling_effect
        };

        string_cpp_to_c_alloc(
            item_entry_cpp.get_name(),
            &temp_item_entry.name
        );
        string_cpp_to_c_alloc(
            item_entry_cpp.get_game(),
            &temp_item_entry.game
        );
        string_cpp_to_c_alloc(
            item_entry_cpp.get_category(),
            &temp_item_entry.category
        );
        string_cpp_to_c_alloc(
            item_entry_cpp.get_pocket(),
            &temp_item_entry.pocket
        );
        string_cpp_to_c_alloc(
            item_entry_cpp.get_description(),
            &temp_item_entry.description
        );
        string_cpp_to_c_alloc(
            item_entry_cpp.get_fling_effect(),
            &temp_item_entry.fling_effect
        );

        temp_item_entry.cost = item_entry_cpp.get_cost();
        temp_item_entry.holdable = item_entry_cpp.holdable();
        temp_item_entry.fling_power = item_entry_cpp.get_fling_power();

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *item_entry_c_ptr = std::move(temp_item_entry);
    }

    void move_entry_cpp_to_c(
        const pkmn::database::move_entry& move_entry_cpp,
        struct pkmn_database_move_entry* move_entry_c_ptr
    )
    {
        BOOST_ASSERT(move_entry_c_ptr != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_database_move_entry temp_move_entry =
        {
            nullptr, // name
            nullptr, // game
            nullptr, // description
            nullptr, // target
            nullptr, // damage_class
            0,       // base_power
            {0},     // pp
            0.0f,    // accuracy
            nullptr, // effect
            nullptr, // contest_type
            nullptr, // contest_effect
            nullptr, // super_contest_effect
        };

        string_cpp_to_c_alloc(
            move_entry_cpp.get_name(),
            &temp_move_entry.name
        );
        string_cpp_to_c_alloc(
            move_entry_cpp.get_game(),
            &temp_move_entry.game
        );
        string_cpp_to_c_alloc(
            move_entry_cpp.get_description(),
            &temp_move_entry.description
        );
        string_cpp_to_c_alloc(
            move_entry_cpp.get_target(),
            &temp_move_entry.target
        );
        string_cpp_to_c_alloc(
            move_entry_cpp.get_damage_class(),
            &temp_move_entry.damage_class
        );
        string_cpp_to_c_alloc(
            move_entry_cpp.get_effect(),
            &temp_move_entry.effect
        );
        string_cpp_to_c_alloc(
            move_entry_cpp.get_contest_type(),
            &temp_move_entry.contest_type
        );
        string_cpp_to_c_alloc(
            move_entry_cpp.get_contest_effect(),
            &temp_move_entry.contest_effect
        );
        string_cpp_to_c_alloc(
            move_entry_cpp.get_super_contest_effect(),
            &temp_move_entry.super_contest_effect
        );

        temp_move_entry.base_power = move_entry_cpp.get_base_power();
        for(int num_pp = 0; num_pp < 4; ++num_pp)
        {
            temp_move_entry.pp[num_pp] = move_entry_cpp.get_pp(num_pp);
        }
        temp_move_entry.accuracy = move_entry_cpp.get_accuracy();

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *move_entry_c_ptr = std::move(temp_move_entry);
    }

    void pokemon_entry_cpp_to_c(
        const pkmn::database::pokemon_entry& pokemon_entry_cpp,
        struct pkmn_database_pokemon_entry* pokemon_entry_c_ptr
    )
    {
        BOOST_ASSERT(pokemon_entry_c_ptr != nullptr);

        struct pkmn_database_pokemon_entry temp_pokemon_entry =
        {
            nullptr, // name
            nullptr, // game
            nullptr, // species
            nullptr, // form
            nullptr, // pokedex_entry
            0.0f, // height
            0.0f, // weight
            0.0f, // chance_male
            0.0f, // chance_female
            false, // has_gender_differences
            0, // base_friendship
            //types
            {
                nullptr, // first
                nullptr  // second
            },
            //abilities
            {
                nullptr, // first
                nullptr  // second
            },
            nullptr, // hidden_ability
            //egg_groups
            {
                nullptr, // first
                nullptr  // second
            },
            {0}, // base_stats
            {0}, // EV_yields
            0, // experience_yield
            //levelup_moves
            {
                nullptr, // levelup_moves
                0ULL     // length
            },
            //tm_hm_moves
            {
                nullptr, // strings
                0ULL     // length
            },
            //egg_moves
            {
                nullptr, // strings
                0ULL     // length
            },
            //tutor_moves
            {
                nullptr, // strings
                0ULL     // length
            },
            //forms
            {
                nullptr, // strings
                0ULL     // length
            },
            //evolutions
            {
                nullptr, // strings
                0ULL     // length
            },
        };

        string_cpp_to_c_alloc(
            pokemon_entry_cpp.get_name(),
            &temp_pokemon_entry.name
        );
        string_cpp_to_c_alloc(
            pokemon_entry_cpp.get_game(),
            &temp_pokemon_entry.game
        );
        string_cpp_to_c_alloc(
            pokemon_entry_cpp.get_form(),
            &temp_pokemon_entry.form
        );
        string_cpp_to_c_alloc(
            pokemon_entry_cpp.get_species(),
            &temp_pokemon_entry.species
        );
        string_cpp_to_c_alloc(
            pokemon_entry_cpp.get_pokedex_entry(),
            &temp_pokemon_entry.pokedex_entry
        );
        string_pair_cpp_to_c(
            pokemon_entry_cpp.get_types(),
            &temp_pokemon_entry.types
        );
        string_cpp_to_c_alloc(
            pokemon_entry_cpp.get_hidden_ability(),
            &temp_pokemon_entry.hidden_ability
        );

        string_pair_cpp_to_c(
            pokemon_entry_cpp.get_abilities(),
            &temp_pokemon_entry.abilities
        );
        string_pair_cpp_to_c(
            pokemon_entry_cpp.get_egg_groups(),
            &temp_pokemon_entry.egg_groups
        );

        temp_pokemon_entry.height = pokemon_entry_cpp.get_height();
        temp_pokemon_entry.weight = pokemon_entry_cpp.get_weight();
        temp_pokemon_entry.chance_male = pokemon_entry_cpp.get_chance_male();
        temp_pokemon_entry.chance_female = pokemon_entry_cpp.get_chance_female();
        temp_pokemon_entry.has_gender_differences = pokemon_entry_cpp.has_gender_differences();
        temp_pokemon_entry.base_friendship = pokemon_entry_cpp.get_base_friendship();
        temp_pokemon_entry.experience_yield = pokemon_entry_cpp.get_experience_yield();

        std::map<std::string, int> base_stats = pokemon_entry_cpp.get_base_stats();
        temp_pokemon_entry.base_stats[PKMN_STAT_HP] = base_stats.at("HP");
        temp_pokemon_entry.base_stats[PKMN_STAT_ATTACK] = base_stats.at("Attack");
        temp_pokemon_entry.base_stats[PKMN_STAT_DEFENSE] = base_stats.at("Defense");
        temp_pokemon_entry.base_stats[PKMN_STAT_SPEED] = base_stats.at("Speed");
        if(base_stats.count("Special") > 0)
        {
            temp_pokemon_entry.base_stats[PKMN_STAT_SPECIAL] = base_stats.at("Special");
            temp_pokemon_entry.base_stats[PKMN_STAT_SPATK] = -1;
            temp_pokemon_entry.base_stats[PKMN_STAT_SPDEF] = -1;
        }
        else
        {
            temp_pokemon_entry.base_stats[PKMN_STAT_SPECIAL] = -1;
            temp_pokemon_entry.base_stats[PKMN_STAT_SPATK] = base_stats.at("Special Attack");
            temp_pokemon_entry.base_stats[PKMN_STAT_SPDEF] = base_stats.at("Special Defense");
        }

        std::map<std::string, int> EV_yields = pokemon_entry_cpp.get_EV_yields();
        temp_pokemon_entry.EV_yields[PKMN_STAT_HP] = EV_yields.at("HP");
        temp_pokemon_entry.EV_yields[PKMN_STAT_ATTACK] = EV_yields.at("Attack");
        temp_pokemon_entry.EV_yields[PKMN_STAT_DEFENSE] = EV_yields.at("Defense");
        temp_pokemon_entry.EV_yields[PKMN_STAT_SPEED] = EV_yields.at("Speed");
        if(EV_yields.count("Special") > 0)
        {
            temp_pokemon_entry.EV_yields[PKMN_STAT_SPECIAL] = EV_yields.at("Special");
            temp_pokemon_entry.EV_yields[PKMN_STAT_SPATK] = -1;
            temp_pokemon_entry.EV_yields[PKMN_STAT_SPDEF] = -1;
        }
        else
        {
            temp_pokemon_entry.EV_yields[PKMN_STAT_SPECIAL] = -1;
            temp_pokemon_entry.EV_yields[PKMN_STAT_SPATK] = EV_yields.at("Special Attack");
            temp_pokemon_entry.EV_yields[PKMN_STAT_SPDEF] = EV_yields.at("Special Defense");
        }

        levelup_moves_cpp_to_c(
            pokemon_entry_cpp.get_levelup_moves(),
            &temp_pokemon_entry.levelup_moves
        );

        move_list_to_string_list(
            pokemon_entry_cpp.get_tm_hm_moves(),
            &temp_pokemon_entry.tm_hm_moves
        );
        move_list_to_string_list(
            pokemon_entry_cpp.get_egg_moves(),
            &temp_pokemon_entry.egg_moves
        );
        move_list_to_string_list(
            pokemon_entry_cpp.get_tutor_moves(),
            &temp_pokemon_entry.tutor_moves
        );
        string_list_cpp_to_c(
            pokemon_entry_cpp.get_forms(),
            &temp_pokemon_entry.forms
        );
        pokemon_entries_to_string_list(
            pokemon_entry_cpp.get_evolutions(),
            &temp_pokemon_entry.evolutions
        );

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *pokemon_entry_c_ptr = std::move(temp_pokemon_entry);
    }

    void pokemon_list_cpp_to_c(
        const pkmn::pokemon_list_t& pokemon_list_cpp,
        struct pkmn_pokemon_list* pokemon_list_c_ptr
    )
    {
        BOOST_ASSERT(pokemon_list_c_ptr != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_pokemon_list temp_pokemon_list =
        {
            nullptr,                // pokemon_list
            pokemon_list_cpp.size() // length
        };

        if(temp_pokemon_list.length > 0)
        {
            temp_pokemon_list.pokemon = (struct pkmn_pokemon*)std::calloc(
                                                             pokemon_list_cpp.size(),
                                                             sizeof(struct pkmn_pokemon)
                                                         );
            for(size_t pokemon_index = 0;
                pokemon_index < temp_pokemon_list.length;
                ++pokemon_index)
            {
                init_pokemon(
                    pokemon_list_cpp[pokemon_index],
                    &temp_pokemon_list.pokemon[pokemon_index]
                );
            }
        }

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *pokemon_list_c_ptr = std::move(temp_pokemon_list);
    }

    void pokemon_box_list_cpp_to_c(
        const pkmn::pokemon_box_list_t& pokemon_box_list_cpp,
        struct pkmn_pokemon_box_list* pokemon_box_list_c_ptr
    )
    {
        BOOST_ASSERT(pokemon_box_list_c_ptr != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_pokemon_box_list temp_pokemon_box_list =
        {
            nullptr,                    // boxes
            pokemon_box_list_cpp.size() // length
        };

        if(temp_pokemon_box_list.length > 0)
        {
            temp_pokemon_box_list.boxes = (struct pkmn_pokemon_box*)std::calloc(
                                                                   pokemon_box_list_cpp.size(),
                                                                   sizeof(struct pkmn_pokemon_box)
                                                               );
            for(size_t pokemon_box_index = 0;
                pokemon_box_index < temp_pokemon_box_list.length;
                ++pokemon_box_index)
            {
                init_pokemon_box(
                    pokemon_box_list_cpp[pokemon_box_index],
                    &temp_pokemon_box_list.boxes[pokemon_box_index]
                );
            }
        }

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *pokemon_box_list_c_ptr = std::move(temp_pokemon_box_list);
    }
}}

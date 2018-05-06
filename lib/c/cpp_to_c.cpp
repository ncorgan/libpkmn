/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http:// opensource.org/licenses/MIT)
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

    template <typename libpkmn_cpp_type, typename libpkmn_c_type>
    static void init_c_internal_class(
        const std::shared_ptr<libpkmn_cpp_type>& libpkmn_sptr,
        libpkmn_c_type* libpkmn_c_struct_out
    )
    {
        BOOST_ASSERT(libpkmn_sptr.get() != nullptr);
        BOOST_ASSERT(libpkmn_c_struct_out != nullptr);

        libpkmn_c_struct_out->p_internal = new pkmn_c_internal_class_t<libpkmn_cpp_type>;
        pkmn_c_internal_class_t<libpkmn_cpp_type>* internal_ptr =
            reinterpret_cast<pkmn_c_internal_class_t<libpkmn_cpp_type>*>(
                libpkmn_c_struct_out->p_internal
            );

        internal_ptr->cpp = libpkmn_sptr;
        internal_ptr->last_error = "None";
        internal_ptr->generation = pkmn::priv::game_name_to_generation(
                                       libpkmn_sptr->get_game()
                                   );
    }

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
                nullptr, // pp_strings
                0        // length
            },
            nullptr // p_internal
        };

        init_c_internal_class(
            cpp_item_bag,
            &temp_item_bag
        );
        string_cpp_to_c_alloc(
            cpp_item_bag->get_game(),
            &temp_item_bag.p_game
        );
        string_list_cpp_to_c(
            cpp_item_bag->get_pocket_names(),
            &temp_item_bag.pocket_names
        );

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
            nullptr  // p_internal
        };

        init_c_internal_class(
            cpp_item_list,
            &temp_item_list
        );
        string_cpp_to_c_alloc(
            cpp_item_list->get_name(),
            &temp_item_list.p_name
        );
        string_cpp_to_c_alloc(
            cpp_item_list->get_game(),
            &temp_item_list.p_game
        );
        temp_item_list.capacity = cpp_item_list->get_capacity();

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
            nullptr // p_internal
        };

        init_c_internal_class(
            cpp_pokedex,
            &temp_pokedex
        );
        string_cpp_to_c_alloc(
            cpp_pokedex->get_game(),
            &temp_pokedex.p_game
        );

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
            nullptr // p_internal
        };

        init_c_internal_class(
            cpp_pokemon,
            &temp_pokemon
        );
        string_cpp_to_c_alloc(
            cpp_pokemon->get_species(),
            &temp_pokemon.p_species
        );
        string_cpp_to_c_alloc(
            cpp_pokemon->get_game(),
            &temp_pokemon.p_game
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
            nullptr  // p_internal
        };

        init_c_internal_class(
            cpp_pokemon_box,
            &temp_pokemon_box
        );
        string_cpp_to_c_alloc(
            cpp_pokemon_box->get_game(),
            &temp_pokemon_box.p_game
        );
        temp_pokemon_box.capacity = cpp_pokemon_box->get_capacity();

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
            nullptr  // p_internal
        };

        init_c_internal_class(
            cpp_pokemon_party,
            &temp_pokemon_party
        );
        string_cpp_to_c_alloc(
            cpp_pokemon_party->get_game(),
            &temp_pokemon_party.p_game
        );
        temp_pokemon_party.capacity = 6ULL;

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
            nullptr  // p_internal
        };

        init_c_internal_class(
            cpp_pokemon_pc,
            &temp_pokemon_pc
        );
        string_cpp_to_c_alloc(
            cpp_pokemon_pc->get_game(),
            &temp_pokemon_pc.p_game
        );
        temp_pokemon_pc.capacity = cpp_pokemon_pc->get_num_boxes();

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
            nullptr  // p_internal
        };

        init_c_internal_class(
            cpp_game_save,
            &temp_game_save
        );
        string_cpp_to_c_alloc(
            cpp_game_save->get_game(),
            &temp_game_save.p_game
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
        struct pkmn_string_list* p_string_list_c_out
    )
    {
        BOOST_ASSERT(p_string_list_c_out != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_string_list temp_string_list_c =
        {
            nullptr,               // pp_strings
            string_list_cpp.size() // length
        };

        if(temp_string_list_c.length > 0)
        {
            temp_string_list_c.pp_strings = (char**)std::calloc(
                                                     string_list_cpp.size(),
                                                     sizeof(char*)
                                                 );
            for(size_t string_index = 0;
                string_index < temp_string_list_c.length;
                ++string_index)
            {
                string_cpp_to_c_alloc(
                    string_list_cpp[string_index],
                    &temp_string_list_c.pp_strings[string_index]
                );
            }
        }

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *p_string_list_c_out = std::move(temp_string_list_c);
    }

    void item_slots_cpp_to_c(
        const pkmn::item_slots_t& item_slots_cpp,
        struct pkmn_item_slots* p_item_slots_c_out
    )
    {
        BOOST_ASSERT(p_item_slots_c_out != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_item_slots temp_item_slots_c =
        {
            nullptr,              // p_item_slots
            item_slots_cpp.size() // length
        };

        if(temp_item_slots_c.length > 0)
        {
            temp_item_slots_c.p_item_slots = (struct pkmn_item_slot*)std::calloc(
                                                                         item_slots_cpp.size(),
                                                                         sizeof(struct pkmn_item_slot)
                                                                     );
            for(size_t item_slot_index = 0;
                item_slot_index < temp_item_slots_c.length;
                ++item_slot_index)
            {
                item_slot_cpp_to_c(
                    item_slots_cpp[item_slot_index],
                    &temp_item_slots_c.p_item_slots[item_slot_index]
                );
            }
        }

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *p_item_slots_c_out = std::move(temp_item_slots_c);
    }

    void levelup_moves_cpp_to_c(
        const pkmn::database::levelup_moves_t& levelup_moves_cpp,
        struct pkmn_levelup_moves* p_levelup_moves_c_out
    )
    {
        BOOST_ASSERT(p_levelup_moves_c_out != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_levelup_moves temp_levelup_moves_c =
        {
            nullptr,                 // p_levelup_moves
            levelup_moves_cpp.size() // length
        };

        if(temp_levelup_moves_c.length > 0)
        {
            temp_levelup_moves_c.p_levelup_moves =
                (struct pkmn_levelup_move*)std::calloc(
                                               levelup_moves_cpp.size(),
                                               sizeof(struct pkmn_levelup_move)
                                           );

            for(size_t levelup_move_index = 0;
                levelup_move_index < temp_levelup_moves_c.length;
                ++levelup_move_index)
            {
                levelup_move_cpp_to_c(
                    levelup_moves_cpp[levelup_move_index],
                    &temp_levelup_moves_c.p_levelup_moves[levelup_move_index]
                );
            }
        }

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *p_levelup_moves_c_out = std::move(temp_levelup_moves_c);
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
            nullptr,         // pp_strings
            move_list.size() // length
        };

        if(temp_string_list.length > 0)
        {
            temp_string_list.pp_strings = (char**)std::calloc(
                                                      move_list.size(),
                                                      sizeof(char*)
                                                  );
            for(size_t move_index = 0;
                move_index < temp_string_list.length;
                ++move_index)
            {
                string_cpp_to_c_alloc(
                    move_list[move_index].get_name(),
                    &temp_string_list.pp_strings[move_index]
                );
            }
        }

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *string_list_ptr = std::move(temp_string_list);
    }

    void move_slots_cpp_to_c(
        const pkmn::move_slots_t& move_slots_cpp,
        struct pkmn_move_slots* p_move_slots_c_out
    )
    {
        BOOST_ASSERT(p_move_slots_c_out != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_move_slots temp_move_slots_c =
        {
            nullptr,              // p_move_slots
            move_slots_cpp.size() // length
        };

        if(temp_move_slots_c.length > 0)
        {
            temp_move_slots_c.p_move_slots =
                (struct pkmn_move_slot*)std::calloc(
                                            move_slots_cpp.size(),
                                            sizeof(struct pkmn_move_slot)
                                        );

            for(size_t move_slot_index = 0;
                move_slot_index < temp_move_slots_c.length;
                ++move_slot_index)
            {
                move_slot_cpp_to_c(
                    move_slots_cpp[move_slot_index],
                    &temp_move_slots_c.p_move_slots[move_slot_index]
                );
            }
        }

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *p_move_slots_c_out = std::move(temp_move_slots_c);
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
            nullptr,               // pp_strings
            pokemon_entries.size() // length
        };

        if(temp_string_list.length > 0)
        {
            temp_string_list.pp_strings = (char**)std::calloc(
                                                      pokemon_entries.size(),
                                                      sizeof(char*)
                                                  );
            for(size_t entry_index = 0;
                entry_index < temp_string_list.length;
                ++entry_index)
            {
                string_cpp_to_c_alloc(
                    pokemon_entries[entry_index].get_name(),
                    &temp_string_list.pp_strings[entry_index]
                );
            }
        }

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *string_list_ptr = std::move(temp_string_list);
    }

    void item_entry_cpp_to_c(
        const pkmn::database::item_entry& item_entry_cpp,
        struct pkmn_database_item_entry* p_item_entry_c_out
    )
    {
        BOOST_ASSERT(p_item_entry_c_out != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_database_item_entry temp_item_entry_c =
        {
            nullptr, // p_name
            nullptr, // p_game
            nullptr, // p_category
            nullptr, // p_pocket
            nullptr, // p_description
            0,       // cost
            false,   // holdable
            0,       // p_fling_power
            nullptr  // p_fling_effect
        };

        string_cpp_to_c_alloc(
            item_entry_cpp.get_name(),
            &temp_item_entry_c.p_name
        );
        string_cpp_to_c_alloc(
            item_entry_cpp.get_game(),
            &temp_item_entry_c.p_game
        );
        string_cpp_to_c_alloc(
            item_entry_cpp.get_category(),
            &temp_item_entry_c.p_category
        );
        string_cpp_to_c_alloc(
            item_entry_cpp.get_pocket(),
            &temp_item_entry_c.p_pocket
        );
        string_cpp_to_c_alloc(
            item_entry_cpp.get_description(),
            &temp_item_entry_c.p_description
        );
        string_cpp_to_c_alloc(
            item_entry_cpp.get_fling_effect(),
            &temp_item_entry_c.p_fling_effect
        );

        temp_item_entry_c.cost = item_entry_cpp.get_cost();
        temp_item_entry_c.holdable = item_entry_cpp.holdable();
        temp_item_entry_c.fling_power = item_entry_cpp.get_fling_power();

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *p_item_entry_c_out = std::move(temp_item_entry_c);
    }

    void move_entry_cpp_to_c(
        const pkmn::database::move_entry& move_entry_cpp,
        struct pkmn_database_move_entry* p_move_entry_c_out
    )
    {
        BOOST_ASSERT(p_move_entry_c_out != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_database_move_entry temp_move_entry_c =
        {
            nullptr, // p_name
            nullptr, // p_game
            nullptr, // p_description
            nullptr, // p_target
            nullptr, // p_damage_class
            0,       // base_power
            {0},     // pp
            0.0f,    // accuracy
            nullptr, // p_effect
            nullptr, // p_contest_type
            nullptr, // p_contest_effect
            nullptr, // p_super_contest_effect
        };

        string_cpp_to_c_alloc(
            move_entry_cpp.get_name(),
            &temp_move_entry_c.p_name
        );
        string_cpp_to_c_alloc(
            move_entry_cpp.get_game(),
            &temp_move_entry_c.p_game
        );
        string_cpp_to_c_alloc(
            move_entry_cpp.get_description(),
            &temp_move_entry_c.p_description
        );
        string_cpp_to_c_alloc(
            move_entry_cpp.get_target(),
            &temp_move_entry_c.p_target
        );
        string_cpp_to_c_alloc(
            move_entry_cpp.get_damage_class(),
            &temp_move_entry_c.p_damage_class
        );
        string_cpp_to_c_alloc(
            move_entry_cpp.get_effect(),
            &temp_move_entry_c.p_effect
        );
        string_cpp_to_c_alloc(
            move_entry_cpp.get_contest_type(),
            &temp_move_entry_c.p_contest_type
        );
        string_cpp_to_c_alloc(
            move_entry_cpp.get_contest_effect(),
            &temp_move_entry_c.p_contest_effect
        );
        string_cpp_to_c_alloc(
            move_entry_cpp.get_super_contest_effect(),
            &temp_move_entry_c.p_super_contest_effect
        );

        temp_move_entry_c.base_power = move_entry_cpp.get_base_power();
        for(int num_pp = 0; num_pp < 4; ++num_pp)
        {
            temp_move_entry_c.pp[num_pp] = move_entry_cpp.get_pp(num_pp);
        }
        temp_move_entry_c.accuracy = move_entry_cpp.get_accuracy();

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *p_move_entry_c_out = std::move(temp_move_entry_c);
    }

    void pokemon_entry_cpp_to_c(
        const pkmn::database::pokemon_entry& pokemon_entry_cpp,
        struct pkmn_database_pokemon_entry* p_pokemon_entry_c_out
    )
    {
        BOOST_ASSERT(p_pokemon_entry_c_out != nullptr);

        struct pkmn_database_pokemon_entry temp_pokemon_entry_c =
        {
            nullptr, // p_name
            nullptr, // p_game
            nullptr, // p_species
            nullptr, // p_form
            nullptr, // p_pokedex_entry
            0.0f, // height
            0.0f, // weight
            0.0f, // chance_male
            0.0f, // chance_female
            false, // has_gender_differences
            0, // base_friendship
            // types
            {
                nullptr, // p_first
                nullptr  // p_second
            },
            // abilities
            {
                nullptr, // p_first
                nullptr  // p_second
            },
            nullptr, // hidden_ability
            // egg_groups
            {
                nullptr, // p_first
                nullptr  // p_second
            },
            {0}, // base_stats
            {0}, // EV_yields
            0, // experience_yield
            // levelup_moves
            {
                nullptr, // levelup_moves
                0ULL     // length
            },
            // tm_hm_moves
            {
                nullptr, // pp_strings
                0ULL     // length
            },
            // egg_moves
            {
                nullptr, // pp_strings
                0ULL     // length
            },
            // tutor_moves
            {
                nullptr, // pp_strings
                0ULL     // length
            },
            // forms
            {
                nullptr, // pp_strings
                0ULL     // length
            },
            // evolutions
            {
                nullptr, // pp_strings
                0ULL     // length
            },
        };

        string_cpp_to_c_alloc(
            pokemon_entry_cpp.get_name(),
            &temp_pokemon_entry_c.p_name
        );
        string_cpp_to_c_alloc(
            pokemon_entry_cpp.get_game(),
            &temp_pokemon_entry_c.p_game
        );
        string_cpp_to_c_alloc(
            pokemon_entry_cpp.get_form(),
            &temp_pokemon_entry_c.p_form
        );
        string_cpp_to_c_alloc(
            pokemon_entry_cpp.get_species(),
            &temp_pokemon_entry_c.p_species
        );
        string_cpp_to_c_alloc(
            pokemon_entry_cpp.get_pokedex_entry(),
            &temp_pokemon_entry_c.p_pokedex_entry
        );
        string_pair_cpp_to_c(
            pokemon_entry_cpp.get_types(),
            &temp_pokemon_entry_c.types
        );
        string_cpp_to_c_alloc(
            pokemon_entry_cpp.get_hidden_ability(),
            &temp_pokemon_entry_c.p_hidden_ability
        );

        string_pair_cpp_to_c(
            pokemon_entry_cpp.get_abilities(),
            &temp_pokemon_entry_c.abilities
        );
        string_pair_cpp_to_c(
            pokemon_entry_cpp.get_egg_groups(),
            &temp_pokemon_entry_c.egg_groups
        );

        temp_pokemon_entry_c.height = pokemon_entry_cpp.get_height();
        temp_pokemon_entry_c.weight = pokemon_entry_cpp.get_weight();
        temp_pokemon_entry_c.chance_male = pokemon_entry_cpp.get_chance_male();
        temp_pokemon_entry_c.chance_female = pokemon_entry_cpp.get_chance_female();
        temp_pokemon_entry_c.has_gender_differences = pokemon_entry_cpp.has_gender_differences();
        temp_pokemon_entry_c.base_friendship = pokemon_entry_cpp.get_base_friendship();
        temp_pokemon_entry_c.experience_yield = pokemon_entry_cpp.get_experience_yield();

        std::map<std::string, int> base_stats = pokemon_entry_cpp.get_base_stats();
        temp_pokemon_entry_c.base_stats[PKMN_STAT_HP] = base_stats.at("HP");
        temp_pokemon_entry_c.base_stats[PKMN_STAT_ATTACK] = base_stats.at("Attack");
        temp_pokemon_entry_c.base_stats[PKMN_STAT_DEFENSE] = base_stats.at("Defense");
        temp_pokemon_entry_c.base_stats[PKMN_STAT_SPEED] = base_stats.at("Speed");
        if(base_stats.count("Special") > 0)
        {
            temp_pokemon_entry_c.base_stats[PKMN_STAT_SPECIAL] = base_stats.at("Special");
            temp_pokemon_entry_c.base_stats[PKMN_STAT_SPATK] = -1;
            temp_pokemon_entry_c.base_stats[PKMN_STAT_SPDEF] = -1;
        }
        else
        {
            temp_pokemon_entry_c.base_stats[PKMN_STAT_SPECIAL] = -1;
            temp_pokemon_entry_c.base_stats[PKMN_STAT_SPATK] = base_stats.at("Special Attack");
            temp_pokemon_entry_c.base_stats[PKMN_STAT_SPDEF] = base_stats.at("Special Defense");
        }

        std::map<std::string, int> EV_yields = pokemon_entry_cpp.get_EV_yields();
        temp_pokemon_entry_c.EV_yields[PKMN_STAT_HP] = EV_yields.at("HP");
        temp_pokemon_entry_c.EV_yields[PKMN_STAT_ATTACK] = EV_yields.at("Attack");
        temp_pokemon_entry_c.EV_yields[PKMN_STAT_DEFENSE] = EV_yields.at("Defense");
        temp_pokemon_entry_c.EV_yields[PKMN_STAT_SPEED] = EV_yields.at("Speed");
        if(EV_yields.count("Special") > 0)
        {
            temp_pokemon_entry_c.EV_yields[PKMN_STAT_SPECIAL] = EV_yields.at("Special");
            temp_pokemon_entry_c.EV_yields[PKMN_STAT_SPATK] = -1;
            temp_pokemon_entry_c.EV_yields[PKMN_STAT_SPDEF] = -1;
        }
        else
        {
            temp_pokemon_entry_c.EV_yields[PKMN_STAT_SPECIAL] = -1;
            temp_pokemon_entry_c.EV_yields[PKMN_STAT_SPATK] = EV_yields.at("Special Attack");
            temp_pokemon_entry_c.EV_yields[PKMN_STAT_SPDEF] = EV_yields.at("Special Defense");
        }

        levelup_moves_cpp_to_c(
            pokemon_entry_cpp.get_levelup_moves(),
            &temp_pokemon_entry_c.levelup_moves
        );

        move_list_to_string_list(
            pokemon_entry_cpp.get_tm_hm_moves(),
            &temp_pokemon_entry_c.tm_hm_moves
        );
        move_list_to_string_list(
            pokemon_entry_cpp.get_egg_moves(),
            &temp_pokemon_entry_c.egg_moves
        );
        move_list_to_string_list(
            pokemon_entry_cpp.get_tutor_moves(),
            &temp_pokemon_entry_c.tutor_moves
        );
        string_list_cpp_to_c(
            pokemon_entry_cpp.get_forms(),
            &temp_pokemon_entry_c.forms
        );
        pokemon_entries_to_string_list(
            pokemon_entry_cpp.get_evolutions(),
            &temp_pokemon_entry_c.evolutions
        );

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *p_pokemon_entry_c_out = std::move(temp_pokemon_entry_c);
    }

    void pokemon_list_cpp_to_c(
        const pkmn::pokemon_list_t& pokemon_list_cpp,
        struct pkmn_pokemon_list* p_pokemon_list_c_out
    )
    {
        BOOST_ASSERT(p_pokemon_list_c_out != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_pokemon_list temp_pokemon_list_c =
        {
            nullptr,                // p_pokemon_list
            pokemon_list_cpp.size() // length
        };

        if(temp_pokemon_list_c.length > 0)
        {
            temp_pokemon_list_c.p_pokemon =
                (struct pkmn_pokemon*)std::calloc(
                                          pokemon_list_cpp.size(),
                                          sizeof(struct pkmn_pokemon)
                                      );

            for(size_t pokemon_index = 0;
                pokemon_index < temp_pokemon_list_c.length;
                ++pokemon_index)
            {
                init_pokemon(
                    pokemon_list_cpp[pokemon_index],
                    &temp_pokemon_list_c.p_pokemon[pokemon_index]
                );
            }
        }

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *p_pokemon_list_c_out = std::move(temp_pokemon_list_c);
    }

    void pokemon_box_list_cpp_to_c(
        const pkmn::pokemon_box_list_t& pokemon_box_list_cpp,
        struct pkmn_pokemon_box_list* p_pokemon_box_list_c_out
    )
    {
        BOOST_ASSERT(p_pokemon_box_list_c_out != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_pokemon_box_list temp_pokemon_box_list_c =
        {
            nullptr,                    // p_boxes
            pokemon_box_list_cpp.size() // length
        };

        if(temp_pokemon_box_list_c.length > 0)
        {
            temp_pokemon_box_list_c.p_boxes =
                (struct pkmn_pokemon_box*)std::calloc(
                                              pokemon_box_list_cpp.size(),
                                              sizeof(struct pkmn_pokemon_box)
                                          );

            for(size_t pokemon_box_index = 0;
                pokemon_box_index < temp_pokemon_box_list_c.length;
                ++pokemon_box_index)
            {
                init_pokemon_box(
                    pokemon_box_list_cpp[pokemon_box_index],
                    &temp_pokemon_box_list_c.p_boxes[pokemon_box_index]
                );
            }
        }

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *p_pokemon_box_list_c_out = std::move(temp_pokemon_box_list_c);
    }
}}

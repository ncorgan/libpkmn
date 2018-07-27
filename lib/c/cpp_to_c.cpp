/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http:// opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "enum_maps.hpp"

#include "private_exports.hpp"

#include <boost/assert.hpp>

/*
 * Note that despite pkmn_*_t being C structs, we can't use designated
 * initializers in C++.
 */

namespace pkmn { namespace c {

    // LibPKMN wrapper class initialization.

    template <typename libpkmn_cpp_type, typename libpkmn_c_type>
    static void init_common(
        const std::shared_ptr<libpkmn_cpp_type>& libpkmn_sptr,
        libpkmn_c_type* p_libpkmn_c_struct_out
    )
    {
        BOOST_ASSERT(libpkmn_sptr.get() != nullptr);
        BOOST_ASSERT(p_libpkmn_c_struct_out != nullptr);

        p_libpkmn_c_struct_out->game = static_cast<enum pkmn_game>(
                                           libpkmn_sptr->get_game()
                                       );

        p_libpkmn_c_struct_out->p_internal = new pkmn_c_internal_class_t<libpkmn_cpp_type>;
        pkmn_c_internal_class_t<libpkmn_cpp_type>* p_internal =
            reinterpret_cast<pkmn_c_internal_class_t<libpkmn_cpp_type>*>(
                p_libpkmn_c_struct_out->p_internal
            );

        p_internal->cpp = libpkmn_sptr;
        p_internal->last_error = "None";
        p_internal->generation = pkmn::priv::game_enum_to_generation(
                                     libpkmn_sptr->get_game()
                                 );
    }

    void init_daycare(
        const pkmn::daycare::sptr& daycare_cpp,
        struct pkmn_daycare* p_daycare_c_out
    )
    {
        BOOST_ASSERT(p_daycare_c_out != nullptr);
        BOOST_ASSERT(daycare_cpp.get() != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_daycare temp_daycare_c =
        {
            PKMN_GAME_NONE, // game

            false, // can_breed_pokemon

            0, // p_levelup_pokemon_capacity
            0, // p_breeding_pokemon_capacity

            nullptr  // p_internal
        };

        init_common(
            daycare_cpp,
            &temp_daycare_c
        );

        temp_daycare_c.can_breed_pokemon = daycare_cpp->can_breed_pokemon();

        temp_daycare_c.levelup_pokemon_capacity = daycare_cpp->get_levelup_pokemon_capacity();
        temp_daycare_c.breeding_pokemon_capacity = daycare_cpp->get_breeding_pokemon_capacity();

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *p_daycare_c_out = std::move(temp_daycare_c);
    }

    void init_item_bag(
        const pkmn::item_bag::sptr& item_bag_cpp,
        struct pkmn_item_bag* p_item_bag_c_out
    )
    {
        BOOST_ASSERT(p_item_bag_c_out != nullptr);
        BOOST_ASSERT(item_bag_cpp.get() != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_item_bag temp_item_bag_c =
        {
            PKMN_GAME_NONE, // game

            // pocket_names
            {
                nullptr, // pp_strings
                0        // length
            },
            nullptr // p_internal
        };

        init_common(
            item_bag_cpp,
            &temp_item_bag_c
        );
        string_list_cpp_to_c(
            item_bag_cpp->get_pocket_names(),
            &temp_item_bag_c.pocket_names
        );

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *p_item_bag_c_out = std::move(temp_item_bag_c);
    }

    void init_item_list(
        const pkmn::item_list::sptr& item_list_cpp,
        struct pkmn_item_list* p_item_list_c_out
    )
    {
        BOOST_ASSERT(p_item_list_c_out != nullptr);
        BOOST_ASSERT(item_list_cpp.get() != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_item_list temp_item_list_c =
        {
            nullptr, // p_name
            PKMN_GAME_NONE, // game
            0ULL,    // capacity
            nullptr  // p_internal
        };

        init_common(
            item_list_cpp,
            &temp_item_list_c
        );
        string_cpp_to_c_alloc(
            item_list_cpp->get_name(),
            &temp_item_list_c.p_name
        );
        temp_item_list_c.capacity = item_list_cpp->get_capacity();

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *p_item_list_c_out = std::move(temp_item_list_c);
    }

    void init_pokedex(
        const pkmn::pokedex::sptr& pokedex_cpp,
        struct pkmn_pokedex* p_pokedex_c_out
    )
    {
        BOOST_ASSERT(p_pokedex_c_out != nullptr);
        BOOST_ASSERT(pokedex_cpp.get() != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_pokedex temp_pokedex_c =
        {
            PKMN_GAME_NONE, // game
            nullptr  // p_internal
        };

        init_common(
            pokedex_cpp,
            &temp_pokedex_c
        );

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *p_pokedex_c_out = std::move(temp_pokedex_c);
    }

    void init_pokemon(
        const pkmn::pokemon::sptr& pokemon_cpp,
        struct pkmn_pokemon* p_pokemon_c_out
    )
    {
        BOOST_ASSERT(p_pokemon_c_out != nullptr);
        BOOST_ASSERT(pokemon_cpp.get() != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_pokemon temp_pokemon_c =
        {
            PKMN_SPECIES_NONE, // species
            PKMN_GAME_NONE, // game
            nullptr  // p_internal
        };

        init_common(
            pokemon_cpp,
            &temp_pokemon_c
        );

        temp_pokemon_c.species = static_cast<enum pkmn_species>(
                                     pokemon_cpp->get_species()
                                 );

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *p_pokemon_c_out = std::move(temp_pokemon_c);
    }

    void init_pokemon_box(
        const pkmn::pokemon_box::sptr& pokemon_box_cpp,
        struct pkmn_pokemon_box* p_pokemon_box_c_out
    )
    {
        BOOST_ASSERT(p_pokemon_box_c_out != nullptr);
        BOOST_ASSERT(pokemon_box_cpp.get() != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_pokemon_box temp_pokemon_box_c =
        {
            PKMN_GAME_NONE, // game
            0ULL,    // capacity
            nullptr  // p_internal
        };

        init_common(
            pokemon_box_cpp,
            &temp_pokemon_box_c
        );
        temp_pokemon_box_c.capacity = pokemon_box_cpp->get_capacity();

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *p_pokemon_box_c_out = std::move(temp_pokemon_box_c);
    }

    void init_pokemon_party(
        const pkmn::pokemon_party::sptr& pokemon_party_cpp,
        struct pkmn_pokemon_party* p_pokemon_party_c_out
    )
    {
        BOOST_ASSERT(p_pokemon_party_c_out != nullptr);
        BOOST_ASSERT(pokemon_party_cpp.get() != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_pokemon_party temp_pokemon_party_c =
        {
            PKMN_GAME_NONE, // game
            0ULL,    // capacity
            nullptr  // p_internal
        };

        init_common(
            pokemon_party_cpp,
            &temp_pokemon_party_c
        );
        temp_pokemon_party_c.capacity = 6ULL;

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *p_pokemon_party_c_out = std::move(temp_pokemon_party_c);
    }

    void init_pokemon_pc(
        const pkmn::pokemon_pc::sptr& pokemon_pc_cpp,
        struct pkmn_pokemon_pc* p_pokemon_pc_c_out
    )
    {
        BOOST_ASSERT(p_pokemon_pc_c_out != nullptr);
        BOOST_ASSERT(pokemon_pc_cpp.get() != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_pokemon_pc temp_pokemon_pc_c =
        {
            PKMN_GAME_NONE, // game
            0ULL,    // capacity
            nullptr  // p_internal
        };

        init_common(
            pokemon_pc_cpp,
            &temp_pokemon_pc_c
        );
        temp_pokemon_pc_c.capacity = pokemon_pc_cpp->get_num_boxes();

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *p_pokemon_pc_c_out = std::move(temp_pokemon_pc_c);
    }

    void init_game_save(
        const pkmn::game_save::sptr& game_save_cpp,
        struct pkmn_game_save* p_game_save_c_out
    )
    {
        BOOST_ASSERT(p_game_save_c_out != nullptr);
        BOOST_ASSERT(game_save_cpp.get() != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_game_save temp_game_save_c =
        {
            PKMN_GAME_NONE, // game
            nullptr  // p_internal
        };

        init_common(
            game_save_cpp,
            &temp_game_save_c
        );

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *p_game_save_c_out = std::move(temp_game_save_c);
    }

    // Helper classes

    void string_cpp_to_c(
        const std::string& string_cpp,
        char* p_c_str_out,
        size_t buffer_len,
        size_t* p_string_length_out
    )
    {
        BOOST_ASSERT(p_c_str_out);

        size_t string_length = string_cpp.size();

        if(!string_cpp.empty())
        {
            std::strncpy(
                p_c_str_out,
                string_cpp.c_str(),
                buffer_len
            );

            // Null-terminate if we can
            if(string_length < buffer_len)
            {
                p_c_str_out[string_length] = '\0';
            }
        }

        if(p_string_length_out)
        {
            *p_string_length_out = string_length;
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
        const pkmn::database::move_list_t& move_list_cpp,
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
            nullptr,             // pp_strings
            move_list_cpp.size() // length
        };

        if(temp_string_list_c.length > 0)
        {
            temp_string_list_c.pp_strings = (char**)std::calloc(
                                                        move_list_cpp.size(),
                                                        sizeof(char*)
                                                    );
            for(size_t move_index = 0;
                move_index < temp_string_list_c.length;
                ++move_index)
            {
                string_cpp_to_c_alloc(
                    move_list_cpp[move_index].get_name(),
                    &temp_string_list_c.pp_strings[move_index]
                );
            }
        }

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *p_string_list_c_out = std::move(temp_string_list_c);
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
        const pkmn::database::pokemon_entries_t& pokemon_entries_cpp,
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
            nullptr,                   // pp_strings
            pokemon_entries_cpp.size() // length
        };

        if(temp_string_list_c.length > 0)
        {
            temp_string_list_c.pp_strings = (char**)std::calloc(
                                                        pokemon_entries_cpp.size(),
                                                        sizeof(char*)
                                                    );
            for(size_t entry_index = 0;
                entry_index < temp_string_list_c.length;
                ++entry_index)
            {
                string_cpp_to_c_alloc(
                    pokemon_entries_cpp[entry_index].get_species_name(),
                    &temp_string_list_c.pp_strings[entry_index]
                );
            }
        }

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *p_string_list_c_out = std::move(temp_string_list_c);
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
            PKMN_ITEM_NONE, // name
            nullptr, // p_name
            PKMN_GAME_NONE, // game
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

        temp_item_entry_c.item = static_cast<enum pkmn_item>(item_entry_cpp.get_item());
        temp_item_entry_c.game = static_cast<enum pkmn_game>(item_entry_cpp.get_game());

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
            PKMN_MOVE_NONE, // move
            nullptr, // p_name
            PKMN_GAME_NONE, // game
            PKMN_TYPE_NONE, // type
            nullptr, // p_description
            PKMN_MOVE_TARGET_NONE, // target
            PKMN_MOVE_DAMAGE_CLASS_NONE, // damage_class
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
            move_entry_cpp.get_description(),
            &temp_move_entry_c.p_description
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

        temp_move_entry_c.move = static_cast<enum pkmn_move>(move_entry_cpp.get_move());
        temp_move_entry_c.game = static_cast<enum pkmn_game>(move_entry_cpp.get_game());
        temp_move_entry_c.type = static_cast<enum pkmn_type>(move_entry_cpp.get_type());
        temp_move_entry_c.target = static_cast<enum pkmn_move_target>(move_entry_cpp.get_target());
        temp_move_entry_c.damage_class = static_cast<enum pkmn_move_damage_class>(move_entry_cpp.get_damage_class());

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
            PKMN_SPECIES_NONE, // species
            nullptr, // p_species_name
            PKMN_GAME_NONE, // game
            nullptr, // p_category
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
                PKMN_TYPE_NONE, // first
                PKMN_TYPE_NONE  // second
            },
            // abilities
            {
                PKMN_ABILITY_NONE, // first
                PKMN_ABILITY_NONE, // second
            },
            PKMN_ABILITY_NONE, // hidden_ability
            // egg_groups
            {
                PKMN_EGG_GROUP_NONE, // first
                PKMN_EGG_GROUP_NONE, // second
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
            pokemon_entry_cpp.get_species_name(),
            &temp_pokemon_entry_c.p_species_name
        );
        string_cpp_to_c_alloc(
            pokemon_entry_cpp.get_form(),
            &temp_pokemon_entry_c.p_form
        );
        string_cpp_to_c_alloc(
            pokemon_entry_cpp.get_category(),
            &temp_pokemon_entry_c.p_category
        );
        string_cpp_to_c_alloc(
            pokemon_entry_cpp.get_pokedex_entry(),
            &temp_pokemon_entry_c.p_pokedex_entry
        );

        ability_pair_cpp_to_c(
            pokemon_entry_cpp.get_abilities(),
            &temp_pokemon_entry_c.abilities
        );
        egg_group_pair_cpp_to_c(
            pokemon_entry_cpp.get_egg_groups(),
            &temp_pokemon_entry_c.egg_groups
        );
        type_pair_cpp_to_c(
            pokemon_entry_cpp.get_types(),
            &temp_pokemon_entry_c.types
        );

        temp_pokemon_entry_c.species = static_cast<enum pkmn_species>(pokemon_entry_cpp.get_species());
        temp_pokemon_entry_c.game = static_cast<enum pkmn_game>(pokemon_entry_cpp.get_game());
        temp_pokemon_entry_c.hidden_ability = static_cast<enum pkmn_ability>(pokemon_entry_cpp.get_hidden_ability());

        temp_pokemon_entry_c.height = pokemon_entry_cpp.get_height();
        temp_pokemon_entry_c.weight = pokemon_entry_cpp.get_weight();
        temp_pokemon_entry_c.chance_male = pokemon_entry_cpp.get_chance_male();
        temp_pokemon_entry_c.chance_female = pokemon_entry_cpp.get_chance_female();
        temp_pokemon_entry_c.has_gender_differences = pokemon_entry_cpp.has_gender_differences();
        temp_pokemon_entry_c.base_friendship = pokemon_entry_cpp.get_base_friendship();
        temp_pokemon_entry_c.experience_yield = pokemon_entry_cpp.get_experience_yield();

        std::map<pkmn::e_stat, int> base_stats = pokemon_entry_cpp.get_base_stats();
        temp_pokemon_entry_c.base_stats[PKMN_STAT_HP] = base_stats.at(pkmn::e_stat::HP);
        temp_pokemon_entry_c.base_stats[PKMN_STAT_ATTACK] = base_stats.at(pkmn::e_stat::ATTACK);
        temp_pokemon_entry_c.base_stats[PKMN_STAT_DEFENSE] = base_stats.at(pkmn::e_stat::DEFENSE);
        temp_pokemon_entry_c.base_stats[PKMN_STAT_SPEED] = base_stats.at(pkmn::e_stat::SPEED);
        if(base_stats.count(pkmn::e_stat::SPECIAL) > 0)
        {
            temp_pokemon_entry_c.base_stats[PKMN_STAT_SPECIAL] = base_stats.at(pkmn::e_stat::SPECIAL);
            temp_pokemon_entry_c.base_stats[PKMN_STAT_SPATK] = -1;
            temp_pokemon_entry_c.base_stats[PKMN_STAT_SPDEF] = -1;
        }
        else
        {
            temp_pokemon_entry_c.base_stats[PKMN_STAT_SPECIAL] = -1;
            temp_pokemon_entry_c.base_stats[PKMN_STAT_SPATK] = base_stats.at(pkmn::e_stat::SPECIAL_ATTACK);
            temp_pokemon_entry_c.base_stats[PKMN_STAT_SPDEF] = base_stats.at(pkmn::e_stat::SPECIAL_DEFENSE);
        }

        std::map<pkmn::e_stat, int> EV_yields = pokemon_entry_cpp.get_EV_yields();
        temp_pokemon_entry_c.EV_yields[PKMN_STAT_HP] = EV_yields.at(pkmn::e_stat::HP);
        temp_pokemon_entry_c.EV_yields[PKMN_STAT_ATTACK] = EV_yields.at(pkmn::e_stat::ATTACK);
        temp_pokemon_entry_c.EV_yields[PKMN_STAT_DEFENSE] = EV_yields.at(pkmn::e_stat::DEFENSE);
        temp_pokemon_entry_c.EV_yields[PKMN_STAT_SPEED] = EV_yields.at(pkmn::e_stat::SPEED);
        if(EV_yields.count(pkmn::e_stat::SPECIAL) > 0)
        {
            temp_pokemon_entry_c.EV_yields[PKMN_STAT_SPECIAL] = EV_yields.at(pkmn::e_stat::SPECIAL);
            temp_pokemon_entry_c.EV_yields[PKMN_STAT_SPATK] = -1;
            temp_pokemon_entry_c.EV_yields[PKMN_STAT_SPDEF] = -1;
        }
        else
        {
            temp_pokemon_entry_c.EV_yields[PKMN_STAT_SPECIAL] = -1;
            temp_pokemon_entry_c.EV_yields[PKMN_STAT_SPATK] = EV_yields.at(pkmn::e_stat::SPECIAL_ATTACK);
            temp_pokemon_entry_c.EV_yields[PKMN_STAT_SPDEF] = EV_yields.at(pkmn::e_stat::SPECIAL_DEFENSE);
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

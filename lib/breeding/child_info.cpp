/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "database/database_common.hpp"
#include "database/enum_conversions.hpp"
#include "database/id_to_string.hpp"
#include "pksav/pksav_call.hpp"
#include "types/rng.hpp"
#include "utils/floating_point_comparison.hpp"
#include "utils/misc.hpp"

#include <SQLiteCpp/SQLiteCpp.h>

#include <pkmn/breeding/child_info.hpp>
#include <pkmn/breeding/compatibility.hpp>
#include <pkmn/database/lists.hpp>
#include <pkmn/database/pokemon_entry.hpp>
#include <pkmn/enums/enum_to_string.hpp>

#include <pksav/common/stats.h>

#include <boost/assert.hpp>
#include <boost/config.hpp>

namespace pkmn { namespace breeding {

    // Needed to avoid using commas in BOOST_ASSERT
    #define IV_MAP_KEYS_TO_VECTOR(iv_map) (map_keys_to_vector<std::map<pkmn::e_stat, int>, pkmn::e_stat>(iv_map))

    struct incense_breeding_pokemon
    {
        std::vector<pkmn::e_species> species;
        pkmn::e_species evolution_to_add;
    };

    pkmn::e_species get_earliest_species_in_evolutionary_line(
        int species_id,
        int generation
    )
    {
        static BOOST_CONSTEXPR const char* evolution_query =
            "SELECT id FROM pokemon_species WHERE generation_id<=? AND id IN "
            "(SELECT evolves_from_species_id FROM pokemon_species WHERE id=?)";

        int query_species_id = species_id;
        SQLite::Statement stmt(pkmn::database::get_connection(), evolution_query);
        stmt.bind(1, generation);
        stmt.bind(2, query_species_id);
        while(stmt.executeStep())
        {
            // The final query will be valid but return 0, which we can't use
            if(int(stmt.getColumn(0)) == 0)
            {
                break;
            }
            query_species_id = stmt.getColumn(0);
            stmt.reset();
            stmt.clearBindings();
            stmt.bind(1, generation);
            stmt.bind(2, query_species_id);
        }

        return static_cast<pkmn::e_species>(query_species_id);
    }

    std::vector<pkmn::e_species> get_possible_child_species(
        pkmn::e_species mother_species,
        pkmn::e_species father_species,
        pkmn::e_game game
    )
    {
        if(!are_pokemon_species_compatible(mother_species, father_species))
        {
            throw std::invalid_argument(
                      "The two given Pokémon are not compatible for breeding."
                  );
        }

        pkmn::database::pokemon_entry mother_entry(mother_species, game, "");
        if(fp_compare_equal(mother_entry.get_chance_male(), 1.0f))
        {
            std::string error_message(pkmn::species_to_string(mother_species));
            error_message += " is male-only and cannot be a mother.";
            throw std::invalid_argument(error_message);
        }

        pkmn::database::pokemon_entry father_entry(father_species, game, "");
        if(fp_compare_equal(father_entry.get_chance_female(), 1.0f))
        {
            std::string error_message(pkmn::species_to_string(father_species));
            error_message += " is female-only and cannot be a father.";
            throw std::invalid_argument(error_message);
        }

        std::vector<pkmn::e_species> possible_child_species;

        bool is_mother_nidoran_f = (mother_species == pkmn::e_species::NIDORAN_F);
        bool is_mother_ditto     = (mother_species == pkmn::e_species::DITTO);
        bool is_mother_illumise  = (mother_species == pkmn::e_species::ILLUMISE);
        bool is_mother_manaphy   = (mother_species == pkmn::e_species::MANAPHY);

        bool is_father_nidoran_m = (father_species == pkmn::e_species::NIDORAN_M);
        bool is_father_ditto     = (father_species == pkmn::e_species::DITTO);
        bool is_father_volbeat   = (father_species == pkmn::e_species::VOLBEAT);
        bool is_father_manaphy   = (father_species == pkmn::e_species::MANAPHY);

        if(is_mother_nidoran_f)
        {
            possible_child_species =
            {
                pkmn::e_species::NIDORAN_F,
                pkmn::e_species::NIDORAN_M
            };
        }
        else if(is_mother_ditto && is_father_nidoran_m)
        {
            possible_child_species = {pkmn::e_species::NIDORAN_M};

            if(pkmn::database::game_enum_to_generation(game) >= 5)
            {
                // Output should be sorted by species ID
                possible_child_species.insert(
                    possible_child_species.begin(),
                    pkmn::e_species::NIDORAN_F
                );
            }
        }
        else if(is_mother_ditto && is_father_volbeat)
        {
            possible_child_species = {pkmn::e_species::VOLBEAT};

            if(pkmn::database::game_enum_to_generation(game) >= 5)
            {
                possible_child_species.emplace_back(
                    pkmn::e_species::ILLUMISE
                );
            }
        }
        else if(is_mother_illumise)
        {
            possible_child_species =
            {
                pkmn::e_species::VOLBEAT,
                pkmn::e_species::ILLUMISE
            };
        }
        else if((is_mother_manaphy && is_father_ditto) ||
                (is_father_manaphy && is_mother_ditto))
        {
            possible_child_species = {pkmn::e_species::PHIONE};
        }
        else if(is_mother_ditto)
        {
            possible_child_species.emplace_back(
                get_earliest_species_in_evolutionary_line(
                    father_entry.get_species_id(),
                    pkmn::database::game_id_to_generation(
                        father_entry.get_game_id()
                    )
                )
            );
        }
        else
        {
            possible_child_species.emplace_back(
                get_earliest_species_in_evolutionary_line(
                    mother_entry.get_species_id(),
                    pkmn::database::game_id_to_generation(
                        mother_entry.get_game_id()
                    )
                )
            );
        }

        // Pokémon with pre-evolutions added after Generation II can breed
        // either itself or the pre-evolution. There's no elegant way to
        // do this automatically, so we have to automatically check.
        static const std::vector<incense_breeding_pokemon> INCENSE_BREEDING_POKEMON =
        {
            {
                {pkmn::e_species::MARILL, pkmn::e_species::AZUMARILL},
                pkmn::e_species::MARILL
            },
            {
                {pkmn::e_species::WOBBUFFET},
                pkmn::e_species::WOBBUFFET
            },
            {
                {pkmn::e_species::ROSELIA, pkmn::e_species::ROSERADE},
                pkmn::e_species::ROSELIA
            },
            {
                {pkmn::e_species::CHIMECHO},
                pkmn::e_species::CHIMECHO
            },
            {
                {pkmn::e_species::SUDOWOODO},
                pkmn::e_species::SUDOWOODO
            },
            {
                {pkmn::e_species::MR_MIME},
                pkmn::e_species::MR_MIME
            },
            {
                {pkmn::e_species::CHANSEY, pkmn::e_species::BLISSEY},
                pkmn::e_species::CHANSEY
            },
            {
                {pkmn::e_species::MANTINE},
                pkmn::e_species::MANTINE
            },
            {
                {pkmn::e_species::SNORLAX},
                pkmn::e_species::SNORLAX
            },
        };
        auto incense_breeding_pokemon_iter =
            std::find_if(
                INCENSE_BREEDING_POKEMON.begin(),
                INCENSE_BREEDING_POKEMON.end(),
                [&mother_species, &father_species](
                    const incense_breeding_pokemon& breeding_pokemon
                )
                {
                    return (does_vector_contain_value(breeding_pokemon.species, mother_species)) ||
                           ((mother_species == pkmn::e_species::DITTO) && does_vector_contain_value(breeding_pokemon.species, father_species));
                });
        if((incense_breeding_pokemon_iter != INCENSE_BREEDING_POKEMON.end()) &&
           !does_vector_contain_value(
                possible_child_species,
                incense_breeding_pokemon_iter->evolution_to_add
            )
          )
        {
            possible_child_species.insert(
                possible_child_species.begin(),
                incense_breeding_pokemon_iter->evolution_to_add
            );
        }

        BOOST_ASSERT(!possible_child_species.empty());
        return possible_child_species;
    }

    std::vector<std::string> get_child_moves(
        const pkmn::pokemon::sptr& mother,
        const pkmn::pokemon::sptr& father,
        pkmn::e_species child_species
    )
    {
        if(mother->get_game() != father->get_game())
        {
            throw std::invalid_argument(
                      "Both parent Pokémon must come from the same game."
                  );
        }

        std::vector<pkmn::e_species> possible_child_species = get_possible_child_species(
                                                                  mother->get_species(),
                                                                  father->get_species(),
                                                                  mother->get_game()
                                                              );
        if(!does_vector_contain_value(possible_child_species, child_species))
        {
            std::string error_message = pkmn::species_to_string(child_species);
            error_message += " is not a possible child for ";
            error_message += pkmn::species_to_string(mother->get_species());
            error_message += " and ";
            error_message += pkmn::species_to_string(father->get_species());
            error_message += ".";

            throw std::invalid_argument(error_message);
        }

        const pkmn::e_game game = mother->get_game();
        const int generation = pkmn::database::game_enum_to_generation(game);

        BOOST_STATIC_CONSTEXPR size_t MAX_NUM_MOVES = 4;
        std::vector<std::string> child_moves;

        const pkmn::move_slots_t& mother_moves = mother->get_moves();
        const pkmn::move_slots_t& father_moves = father->get_moves();
        BOOST_ASSERT(mother_moves.size() == MAX_NUM_MOVES);
        BOOST_ASSERT(father_moves.size() == MAX_NUM_MOVES);

        pkmn::database::pokemon_entry child_entry(child_species, game, "");

        /*
         * From Emerald onward, if either parent is holding a Light Ball, any
         * child Pichu will know Volt Tackle. This takes priority over any
         * other policy.
         */
        if(child_species == pkmn::e_species::PICHU)
        {
            bool has_volt_tackle_policy = (generation >= 4) ||
                                          (game == pkmn::e_game::EMERALD);

            if(has_volt_tackle_policy)
            {
                if((mother->get_held_item() == pkmn::e_item::LIGHT_BALL) ||
                   (father->get_held_item() == pkmn::e_item::LIGHT_BALL))
                {
                    child_moves.emplace_back("Volt Tackle");
                }
            }
        }

        pkmn::database::move_list_t child_egg_moves = child_entry.get_egg_moves();
        if(!child_egg_moves.empty())
        {
            /*
             * If the mother knows any of the child's egg moves, the child
             * will learn them.
             */
            for(size_t mother_move_index = 0;
                (mother_move_index < mother_moves.size()) && (child_moves.size() < MAX_NUM_MOVES);
                ++mother_move_index)
            {
                auto egg_move_iter =
                    std::find_if(
                        child_egg_moves.begin(),
                        child_egg_moves.end(),
                        [&mother_moves, &mother_move_index](const pkmn::database::move_entry& egg_move)
                        {
                            return (egg_move.get_name() == mother_moves[mother_move_index].move);
                        });
                if(egg_move_iter != child_egg_moves.end())
                {
                    child_moves.emplace_back(
                        mother_moves[mother_move_index].move
                    );
                }
            }

            /*
             * If the father knows any of the child's egg moves, the child
             * will learn them.
             */
            for(size_t father_move_index = 0;
                (father_move_index < father_moves.size()) && (child_moves.size() < MAX_NUM_MOVES);
                ++father_move_index)
            {
                auto egg_move_iter =
                    std::find_if(
                        child_egg_moves.begin(),
                        child_egg_moves.end(),
                        [&father_moves, &father_move_index](const pkmn::database::move_entry& egg_move)
                        {
                            return (egg_move.get_name() == father_moves[father_move_index].move);
                        });
                if(egg_move_iter != child_egg_moves.end())
                {
                    child_moves.emplace_back(
                        father_moves[father_move_index].move
                    );
                }
            }
        }

        /*
         * Before Generation VI, if the father (or non-Ditto parent) knows any
         * moves that are TMs/HMs the child is compatible with, then the child
         * will learn these moves.
         *
         */
        if((child_moves.size() < MAX_NUM_MOVES) && (generation <= 5))
        {
            pkmn::database::move_list_t child_tm_hm_moves = child_entry.get_tm_hm_moves();
            std::vector<std::string> child_tm_hm_move_names;
            for(const auto& child_tm_hm_move: child_tm_hm_moves)
            {
                child_tm_hm_move_names.emplace_back(child_tm_hm_move.get_name());
            }

            for(size_t move_index = 0;
                (move_index < father_moves.size()) && (child_moves.size() < MAX_NUM_MOVES);
                ++move_index)
            {
                auto tm_hm_move_name_iter = std::find(
                                                child_tm_hm_move_names.begin(),
                                                child_tm_hm_move_names.end(),
                                                father_moves[move_index].move
                                            );
                if(tm_hm_move_name_iter != child_tm_hm_move_names.end())
                {
                    child_moves.emplace_back(*tm_hm_move_name_iter);
                }
            }
        }

        /*
         * In Crystal, if the father knows any moves that the child can learn via
         * a Move Tutor, the child will hatch knowing these move(s).
         */
        if((child_moves.size() < MAX_NUM_MOVES) && (game == pkmn::e_game::CRYSTAL))
        {
            pkmn::database::move_list_t child_tutor_moves = child_entry.get_tutor_moves();

            for(size_t father_move_index = 0;
                (father_move_index < father_moves.size()) && (child_moves.size() < MAX_NUM_MOVES);
                ++father_move_index)
            {
                auto tutor_move_iter =
                    std::find_if(
                        child_tutor_moves.begin(),
                        child_tutor_moves.end(),
                        [&father_moves, &father_move_index](const pkmn::database::move_entry& tutor_move)
                        {
                            return (tutor_move.get_name() == father_moves[father_move_index].move);
                        });
                if(tutor_move_iter != child_tutor_moves.end())
                {
                    child_moves.emplace_back(
                        father_moves[father_move_index].move
                    );
                }
            }
        }

        if((child_moves.size() < MAX_NUM_MOVES))
        {
            /*
             * If both the mother and father know a move the child would learn via
             * level-up by the level at which the child will hatch, the child will
             * hatch knowing this move.
             */
            pkmn::database::levelup_moves_t child_levelup_moves = child_entry.get_levelup_moves();
            for(size_t levelup_move_index = 0;
                (levelup_move_index < child_levelup_moves.size()) && (child_moves.size() < MAX_NUM_MOVES);
                ++levelup_move_index)
            {
                std::string move_name = child_levelup_moves[levelup_move_index].move.get_name();

                auto mother_move_iter =
                    std::find_if(
                        mother_moves.begin(),
                        mother_moves.end(),
                        [&move_name](const pkmn::move_slot& move_slot)
                        {
                            return (move_slot.move == move_name);
                        });
                if(mother_move_iter != mother_moves.end())
                {
                    auto father_move_iter =
                        std::find_if(
                            father_moves.begin(),
                            father_moves.end(),
                            [&move_name](const pkmn::move_slot& move_slot)
                            {
                                return (move_slot.move == move_name);
                            });
                    if(father_move_iter != father_moves.end())
                    {
                        child_moves.emplace_back(move_name);
                    }
                }
            }

            /*
             * For any remaining moves, use any moves the child would naturally
             * learn by the level at which it will hatch.
             */
            const int child_level = (generation >= 4) ? 1 : 5;

            for(size_t levelup_move_index = 0;
                (levelup_move_index < child_levelup_moves.size()) && (child_moves.size() < MAX_NUM_MOVES);
                ++levelup_move_index)
            {
                if(child_levelup_moves[levelup_move_index].level <= child_level)
                {
                    child_moves.emplace_back(
                        child_levelup_moves[levelup_move_index].move.get_name()
                    );
                }
                else
                {
                    break;
                }
            }
        }

        child_moves.resize(MAX_NUM_MOVES, "None");
        return child_moves;
    }

    static bool can_species_be_male(
        pkmn::e_species species
    )
    {
        static const pkmn::e_game ENTRY_GAME = pkmn::e_game::X;

        return fp_compare_not_equal(
                   pkmn::database::pokemon_entry(species, ENTRY_GAME, "").get_chance_male(),
                   0.0f
               );
    }

    static bool can_species_be_female(
        pkmn::e_species species
    )
    {
        static const pkmn::e_game ENTRY_GAME = pkmn::e_game::X;

        return fp_compare_not_equal(
                   pkmn::database::pokemon_entry(species, ENTRY_GAME, "").get_chance_female(),
                   0.0f
               );
    }

    static bool is_species_genderless(
        pkmn::e_species species
    )
    {
        static const pkmn::e_game ENTRY_GAME = pkmn::e_game::X;

        pkmn::database::pokemon_entry species_entry(species, ENTRY_GAME, "");

        return fp_compare_equal(
                   species_entry.get_chance_male(),
                   0.0f
               ) &&
               fp_compare_equal(
                   species_entry.get_chance_female(),
                   0.0f
               );
    }

    std::map<pkmn::e_stat, int> get_gen2_ideal_child_IVs(
        const pkmn::pokemon::sptr& mother,
        const pkmn::pokemon::sptr& father,
        pkmn::e_gender child_gender
    )
    {
        bool is_mother_ditto = (mother->get_species() == pkmn::e_species::DITTO);
        bool is_father_ditto = (father->get_species() == pkmn::e_species::DITTO);

        std::map<pkmn::e_stat, int> parent_IVs;
        if(is_mother_ditto)
        {
            parent_IVs = mother->get_IVs();
        }
        else if(is_father_ditto)
        {
            parent_IVs = father->get_IVs();
        }
        else
        {
            // Genderless Pokémon can only breed with Ditto, so we know the parents
            // are male and female at this point.
            parent_IVs = (child_gender == pkmn::e_gender::MALE) ? mother->get_IVs()
                                                                : father->get_IVs();
        }

        std::map<pkmn::e_stat, int> ideal_child_IVs;

        // Use PKSav to set the IVs, as the HP IV is derived from the others.
        uint16_t pksav_IV = 0;

        // The Defense IV comes straight from the parent.
        BOOST_ASSERT(parent_IVs.count(pkmn::e_stat::DEFENSE) > 0);

        ideal_child_IVs[pkmn::e_stat::DEFENSE] = parent_IVs[pkmn::e_stat::DEFENSE];
        PKSAV_CALL(
            pksav_set_gb_IV(
                PKSAV_GB_IV_DEFENSE,
                static_cast<uint8_t>(parent_IVs[pkmn::e_stat::DEFENSE]),
                &pksav_IV
            );
        )

        // The Special stat is inherited from the parent. However, there is
        // a 50% chance it will be offset by 8, meaning if the IV was below 8,
        // it would be increased by 8, and if it was above it, it would be
        // decreased by 8. As this function returns ideal valid IVs, we will
        // add a low IV and leave a high IV as is.
        BOOST_ASSERT(parent_IVs.count(pkmn::e_stat::SPECIAL) > 0);

        int IV_special = parent_IVs[pkmn::e_stat::SPECIAL];
        if(IV_special < 8)
        {
            IV_special += 8;
        }
        ideal_child_IVs[pkmn::e_stat::SPECIAL] = IV_special;
        PKSAV_CALL(
            pksav_set_gb_IV(
                PKSAV_GB_IV_SPECIAL,
                static_cast<uint8_t>(IV_special),
                &pksav_IV
            );
        )

        // Attack and Speed IVs are generated randomly, but as this function
        // returns ideal valid IVs, we'll just use the highest valid values.
        ideal_child_IVs[pkmn::e_stat::ATTACK] = PKSAV_MAX_GB_IV;
        PKSAV_CALL(
            pksav_set_gb_IV(
                PKSAV_GB_IV_ATTACK,
                PKSAV_MAX_GB_IV,
                &pksav_IV
            );
        )

        ideal_child_IVs[pkmn::e_stat::SPEED]  = PKSAV_MAX_GB_IV;
        PKSAV_CALL(
            pksav_set_gb_IV(
                PKSAV_GB_IV_SPEED,
                PKSAV_MAX_GB_IV,
                &pksav_IV
            );
        )

        // The HP IV is derived from the rest, so query PKSav for it.
        uint8_t pksav_IVs[PKSAV_NUM_GB_IVS] = {0};
        PKSAV_CALL(
            pksav_get_gb_IVs(
                &pksav_IV,
                pksav_IVs,
                sizeof(pksav_IVs)
            );
        )
        ideal_child_IVs[pkmn::e_stat::HP] = pksav_IVs[PKSAV_GB_IV_HP];

        BOOST_ASSERT(
            IV_MAP_KEYS_TO_VECTOR(ideal_child_IVs) ==
            IV_MAP_KEYS_TO_VECTOR(parent_IVs)
        );
        return ideal_child_IVs;
    }

    static std::map<pkmn::e_stat, int> combine_maps_with_higher_values(
        const std::map<pkmn::e_stat, int>& map1,
        const std::map<pkmn::e_stat, int>& map2
    )
    {
        BOOST_ASSERT(
            IV_MAP_KEYS_TO_VECTOR(map1) ==
            IV_MAP_KEYS_TO_VECTOR(map2)
        );

        std::map<pkmn::e_stat, int> combined_map;
        for(const auto& map1_pair: map1)
        {
            pkmn::e_stat key = map1_pair.first;
            int value = map1_pair.second;

            combined_map[key] = std::max(value, map2.at(key));
        }

        BOOST_ASSERT(
            IV_MAP_KEYS_TO_VECTOR(combined_map) ==
            IV_MAP_KEYS_TO_VECTOR(map1)
        );
        return combined_map;
    }

    template <typename key_type, typename val_type>
    inline typename std::map<key_type, val_type>::iterator map_max_value_iter(
        std::map<key_type, val_type>& r_input_map
    )
    {
        auto iter = std::max_element(
                        r_input_map.begin(),
                        r_input_map.end(),
                        [](const std::pair<key_type, val_type>& map_pair1,
                           const std::pair<key_type, val_type>& map_pair2)
                        {
                            return map_pair1.second < map_pair2.second;
                        });
        BOOST_ASSERT(iter != r_input_map.end());

        return iter;
    }

    void copy_highest_IV_and_remove_from_input_map(
        std::map<pkmn::e_stat, int>& r_input_map,
        std::map<pkmn::e_stat, int>& r_output_map
    )
    {
        auto max_IV_iter = map_max_value_iter(r_input_map);
        pkmn::e_stat stat = max_IV_iter->first;
        int IV = max_IV_iter->second;

        r_output_map[stat] = IV;

        // We've used this IV, so remove it from the intermediate map.
        r_input_map.erase(max_IV_iter);
    }

    static std::map<pkmn::e_stat, int> get_rs_frlg_ideal_child_IVs(
        const pkmn::pokemon::sptr& mother,
        const pkmn::pokemon::sptr& father
    )
    {
        /*
         * A child will inherit three random IVs between the parents,
         * and the rest are randomly generated. We will return the
         * highest unique IVs from each parent and return the maximum
         * valid value for the rest.
         */
        std::map<pkmn::e_stat, int> mother_IVs = mother->get_IVs();
        std::map<pkmn::e_stat, int> father_IVs = father->get_IVs();
        BOOST_ASSERT(
            IV_MAP_KEYS_TO_VECTOR(mother_IVs) ==
            IV_MAP_KEYS_TO_VECTOR(father_IVs)
        );

        std::map<pkmn::e_stat, int> max_parent_IVs = combine_maps_with_higher_values(
                                                        mother_IVs,
                                                        father_IVs
                                                    );

        std::map<pkmn::e_stat, int> ideal_child_IVs;

        BOOST_STATIC_CONSTEXPR size_t num_inherited_IVs = 3;
        for(size_t iteration = 0; iteration < num_inherited_IVs; ++iteration)
        {
            copy_highest_IV_and_remove_from_input_map(
                max_parent_IVs,
                ideal_child_IVs
            );
        }

        // Use the stats remaining in the intermediate map to determine which
        // IVs should have the max value.
        for(const auto& IV_map_pair: max_parent_IVs)
        {
            pkmn::e_stat stat = IV_map_pair.first;

            ideal_child_IVs[stat] = PKSAV_MAX_IV;
        }

        BOOST_ASSERT(
            IV_MAP_KEYS_TO_VECTOR(ideal_child_IVs) ==
            IV_MAP_KEYS_TO_VECTOR(mother_IVs)
        );
        return ideal_child_IVs;
    }

    // https://stackoverflow.com/a/180772/2425605
    std::map<pkmn::e_stat, int> get_filtered_map(
        const std::map<pkmn::e_stat, int>& input_map,
        const std::vector<pkmn::e_stat>& keys_to_remove
    )
    {
        std::map<pkmn::e_stat, int> output_map(input_map);

        for(auto map_iter = input_map.begin(); map_iter != input_map.end();)
        {
            pkmn::e_stat key = map_iter->first;

            auto key_iter = std::find(
                                keys_to_remove.begin(),
                                keys_to_remove.end(),
                                key
                            );
            if(key_iter != keys_to_remove.end())
            {
                output_map.erase(map_iter++);
            }
            else
            {
                ++map_iter;
            }
        }

        return output_map;
    }

    std::map<pkmn::e_stat, int> get_emerald_dp_ideal_child_IVs(
        const pkmn::pokemon::sptr& mother,
        const pkmn::pokemon::sptr& father
    )
    {
        /*
         * First, a random IV in inherited. Second, a non-HP random IV will be
         * inherited. Third, an IV (not HP or Defense) will be inherited. The
         * rest will be randomly generated.
         */
        std::map<pkmn::e_stat, int> mother_IVs = mother->get_IVs();
        std::map<pkmn::e_stat, int> father_IVs = father->get_IVs();
        BOOST_ASSERT(
            IV_MAP_KEYS_TO_VECTOR(mother_IVs) ==
            IV_MAP_KEYS_TO_VECTOR(father_IVs)
        );

        std::map<pkmn::e_stat, int> ideal_child_IVs;

        std::map<pkmn::e_stat, int> max_parent_IVs = combine_maps_with_higher_values(
                                                         mother_IVs,
                                                         father_IVs
                                                     );
        copy_highest_IV_and_remove_from_input_map(
            max_parent_IVs,
            ideal_child_IVs
        );

        std::map<pkmn::e_stat, int> max_parent_IVs_minus_hp = get_filtered_map(
                                                                  max_parent_IVs,
                                                                  {pkmn::e_stat::HP}
                                                              );
        copy_highest_IV_and_remove_from_input_map(
            max_parent_IVs_minus_hp,
            ideal_child_IVs
        );

        std::map<pkmn::e_stat, int> max_parent_IVs_minus_hp_and_def = get_filtered_map(
                                                                         max_parent_IVs,
                                                                         {
                                                                             pkmn::e_stat::HP,
                                                                             pkmn::e_stat::DEFENSE
                                                                         }
                                                                     );
        copy_highest_IV_and_remove_from_input_map(
            max_parent_IVs_minus_hp_and_def,
            ideal_child_IVs
        );

        // Use the highest possible value for the remaining IVs.
        for(const auto& max_parent_IV_pair: max_parent_IVs)
        {
            pkmn::e_stat stat = max_parent_IV_pair.first;
            int IV = max_parent_IV_pair.second;

            if(ideal_child_IVs.count(stat) == 0)
            {
                ideal_child_IVs[stat] = IV;
            }
        }

        BOOST_ASSERT(
            IV_MAP_KEYS_TO_VECTOR(ideal_child_IVs) ==
            IV_MAP_KEYS_TO_VECTOR(mother_IVs)
        );
        return ideal_child_IVs;
    }

    std::map<pkmn::e_stat, int> get_ideal_child_IVs(
        const pkmn::pokemon::sptr& mother,
        const pkmn::pokemon::sptr& father,
        pkmn::e_gender child_gender
    )
    {
        pkmn::enforce_value_in_vector(
            "Child gender",
            child_gender,
            {pkmn::e_gender::MALE, pkmn::e_gender::FEMALE, pkmn::e_gender::GENDERLESS}
        );

        if(mother->get_game() != father->get_game())
        {
            throw std::invalid_argument(
                      "Both parent Pokémon must come from the same game."
                  );
        }

        // Validate given child gender against possible child species.
        std::vector<pkmn::e_species> possible_child_species = get_possible_child_species(
                                                                  mother->get_species(),
                                                                  father->get_species(),
                                                                  mother->get_game()
                                                              );

        auto species_iter = possible_child_species.end();
        switch(child_gender)
        {
            case pkmn::e_gender::MALE:
                species_iter = std::find_if(
                                   possible_child_species.begin(),
                                   possible_child_species.end(),
                                   can_species_be_male
                               );
                break;

            case pkmn::e_gender::FEMALE:
                species_iter = std::find_if(
                                   possible_child_species.begin(),
                                   possible_child_species.end(),
                                   can_species_be_female
                               );
                break;

            case pkmn::e_gender::GENDERLESS:
                species_iter = std::find_if(
                                   possible_child_species.begin(),
                                   possible_child_species.end(),
                                   is_species_genderless
                               );
                break;

            default:
                BOOST_ASSERT_MSG(false, "Invalid child gender");
        }
        if(species_iter == possible_child_species.end())
        {
            std::string error_message = "Invalid gender for any valid child species.";
            throw std::invalid_argument(error_message);
        }

        const pkmn::e_game game = mother->get_game();
        const int generation = pkmn::database::game_enum_to_generation(game);
        BOOST_ASSERT(generation >= 2);

        std::map<pkmn::e_stat, int> ideal_child_IVs;

        switch(generation)
        {
            case 2:
                ideal_child_IVs = get_gen2_ideal_child_IVs(
                                      mother,
                                      father,
                                      child_gender
                                  );
                break;

            case 3:
                if(game == pkmn::e_game::EMERALD)
                {
                    ideal_child_IVs = get_emerald_dp_ideal_child_IVs(
                                          mother,
                                          father
                                      );
                }
                else
                {
                    ideal_child_IVs = get_rs_frlg_ideal_child_IVs(
                                          mother,
                                          father
                                      );
                }
                break;

            case 4:
            case 5:
            case 6:
            case 7:
                throw pkmn::unimplemented_error();

            default:
                throw std::invalid_argument("Invalid game");
        }

        return ideal_child_IVs;
    }

}}

/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "database/database_common.hpp"
#include "database/id_to_string.hpp"
#include "utils/floating_point_comparison.hpp"
#include "utils/misc.hpp"

#include <SQLiteCpp/SQLiteCpp.h>

#include <pkmn/breeding/child_info.hpp>
#include <pkmn/breeding/compatibility.hpp>
#include <pkmn/database/pokemon_entry.hpp>

#include <boost/assert.hpp>
#include <boost/config.hpp>

namespace pkmn { namespace breeding {

    BOOST_STATIC_CONSTEXPR int NIDORAN_F_SPECIES_ID = 29;
    BOOST_STATIC_CONSTEXPR int NIDORAN_M_SPECIES_ID = 32;
    BOOST_STATIC_CONSTEXPR int DITTO_SPECIES_ID     = 132;
    BOOST_STATIC_CONSTEXPR int VOLBEAT_SPECIES_ID   = 313;
    BOOST_STATIC_CONSTEXPR int ILLUMISE_SPECIES_ID  = 314;
    BOOST_STATIC_CONSTEXPR int MANAPHY_SPECIES_ID   = 490;

    struct incense_breeding_pokemon
    {
        std::vector<std::string> species;
        std::string evolution_to_add;
    };

    std::string get_earliest_species_in_evolutionary_line(
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

        return pkmn::database::species_id_to_name(query_species_id);
    }

    std::vector<std::string> get_possible_child_species(
        const std::string& mother_species,
        const std::string& father_species,
        const std::string& game
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
            std::string error_message(mother_species);
            error_message += " is male-only and cannot be a mother.";
            throw std::invalid_argument(error_message);
        }

        pkmn::database::pokemon_entry father_entry(father_species, game, "");
        if(fp_compare_equal(father_entry.get_chance_female(), 1.0f))
        {
            std::string error_message(father_species);
            error_message += " is female-only and cannot be a father.";
            throw std::invalid_argument(error_message);
        }

        std::vector<std::string> possible_child_species;

        bool is_mother_nidoran_f = (mother_entry.get_species_id() == NIDORAN_F_SPECIES_ID);
        bool is_mother_ditto     = (mother_entry.get_species_id() == DITTO_SPECIES_ID);
        bool is_mother_illumise  = (mother_entry.get_species_id() == ILLUMISE_SPECIES_ID);
        bool is_mother_manaphy   = (mother_entry.get_species_id() == MANAPHY_SPECIES_ID);

        bool is_father_nidoran_m = (father_entry.get_species_id() == NIDORAN_M_SPECIES_ID);
        bool is_father_ditto     = (father_entry.get_species_id() == DITTO_SPECIES_ID);
        bool is_father_volbeat   = (father_entry.get_species_id() == VOLBEAT_SPECIES_ID);
        bool is_father_manaphy   = (father_entry.get_species_id() == MANAPHY_SPECIES_ID);

        if(is_mother_nidoran_f)
        {
            // Trust the library to properly return the symbols.
            static const std::vector<std::string> NIDORAN_F_POSSIBLE_SPECIES =
            {
                pkmn::database::species_id_to_name(NIDORAN_F_SPECIES_ID),
                pkmn::database::species_id_to_name(NIDORAN_M_SPECIES_ID),
            };

            possible_child_species = NIDORAN_F_POSSIBLE_SPECIES;
        }
        else if(is_mother_ditto && is_father_nidoran_m)
        {
            possible_child_species.emplace_back(
                pkmn::database::species_id_to_name(NIDORAN_M_SPECIES_ID)
            );

            if(pkmn::database::game_name_to_generation(game) >= 5)
            {
                // Output should be sorted by species ID
                possible_child_species.insert(
                    possible_child_species.begin(),
                    pkmn::database::species_id_to_name(NIDORAN_F_SPECIES_ID)
                );
            }
        }
        else if(is_mother_ditto && is_father_volbeat)
        {
            possible_child_species.emplace_back("Volbeat");
            if(pkmn::database::game_name_to_generation(game) >= 5)
            {
                possible_child_species.emplace_back("Illumise");
            }
        }
        else if(is_mother_illumise)
        {
            possible_child_species = {"Volbeat", "Illumise"};
        }
        else if((is_mother_manaphy && is_father_ditto) ||
                (is_father_manaphy && is_mother_ditto))
        {
            possible_child_species = {"Phione"};
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
            {{"Marill", "Azumarill"}, "Marill"},
            {{"Wobbuffet"}, "Wobbuffet"},
            {{"Roselia", "Roserade"}, "Roselia"},
            {{"Chimecho"}, "Chimecho"},
            {{"Sudowoodo"}, "Sudowoodo"},
            {{"Mr. Mime"}, "Mr. Mime"},
            {{"Chansey", "Blissey"}, "Chansey"},
            {{"Mantine"}, "Mantine"},
            {{"Snorlax"}, "Snorlax"}
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
                           ((mother_species == "Ditto") && does_vector_contain_value(breeding_pokemon.species, father_species));
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
        const std::string& child_species
    )
    {
        if(mother->get_game() != father->get_game())
        {
            throw std::invalid_argument(
                      "Both parent Pokémon must come from the same game."
                  );
        }

        std::vector<std::string> possible_child_species = get_possible_child_species(
                                                              mother->get_species(),
                                                              father->get_species(),
                                                              mother->get_game()
                                                          );
        if(!does_vector_contain_value(possible_child_species, child_species))
        {
            std::string error_message = child_species;
            error_message += " is not a possible child for ";
            error_message += mother->get_species();
            error_message += " and ";
            error_message += father->get_species();
            error_message += ".";

            throw std::invalid_argument(error_message);
        }

        const std::string game = mother->get_game();
        const int generation = pkmn::database::game_name_to_generation(game);

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
        if(child_species == "Pichu")
        {
            bool has_volt_tackle_policy = (generation >= 4) ||
                                          (game == "Emerald");

            if(has_volt_tackle_policy)
            {
                if((mother->get_held_item() == "Light Ball") || (father->get_held_item() == "Light Ball"))
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

        // TODO: add functions to get lists of TMs, HMs, check for father knowing
        // TM or HM moves before Generation VI

        /*
         * In Crystal, if the father knows any moves that the child can learn via
         * a Move Tutor, the child will hatch knowing these move(s).
         */
        if((child_moves.size() < MAX_NUM_MOVES) && (game == "Crystal"))
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

}}

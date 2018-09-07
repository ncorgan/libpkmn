/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "private_exports.hpp"
#include "types/rng.hpp"

#include <pkmn/database/lists.hpp>

#include <pkmntest/misc_comparison.hpp>
#include <pkmntest/pokemon_comparison.hpp>
#include <pkmntest/util.hpp>

#include <gtest/gtest.h>

namespace pkmntest
{
    pkmn::pokemon::sptr get_random_pokemon(
        pkmn::e_species species,
        pkmn::e_game game,
        const std::vector<pkmn::e_item>& item_list,
        const std::vector<pkmn::e_move>& move_list
    )
    {
        pkmn::rng<int> int_rng;
        pkmn::rng<size_t> size_rng;
        int generation = pkmn::priv::game_enum_to_generation(game);

        pkmn::pokemon::sptr ret = pkmn::pokemon::make(
                                      species,
                                      game,
                                      "",
                                      int_rng.rand(2,63)
                                  );
        for(int move_index = 0; move_index < 4; ++move_index)
        {
            pkmn::e_move move = pkmn::e_move::NONE;
            do
            {
                move = move_list[size_rng.rand() % move_list.size()];
            }
            while(move >= pkmn::e_move::SHADOW_RUSH);
            ret->set_move(move, move_index);
        }

        const std::map<pkmn::e_stat, int>& EVs = ret->get_EVs();
        for(auto iter = EVs.begin(); iter != EVs.end(); ++iter)
        {
            ret->set_EV(iter->first, int_rng.rand(0,255));
        }

        const std::map<pkmn::e_stat, int>& IVs = ret->get_IVs();
        for(auto iter = IVs.begin(); iter != IVs.end(); ++iter)
        {
            ret->set_IV(iter->first, int_rng.rand(0,15));
        }

        if(generation >= 2)
        {
            // Keep going until one is holdable
            while(ret->get_held_item() == pkmn::e_item::NONE)
            {
                try
                {
                    ret->set_held_item(
                        item_list[size_rng.rand() % item_list.size()]
                    );
                } catch(std::invalid_argument&) {}
            }

            ret->set_shininess(get_random_bool());
        }

        if(generation >= 3)
        {
            const std::map<pkmn::e_marking, bool>& markings = ret->get_markings();
            for(auto iter = markings.begin(); iter != markings.end(); ++iter)
            {
                ret->set_marking(iter->first, get_random_bool());
            }

            const std::map<std::string, bool>& ribbons = ret->get_ribbons();
            for(auto iter = ribbons.begin(); iter != ribbons.end(); ++iter)
            {
                ret->set_ribbon(iter->first, get_random_bool());
            }

            const std::map<pkmn::e_contest_stat, int>& contest_stats = ret->get_contest_stats();
            for(auto iter = contest_stats.begin(); iter != contest_stats.end(); ++iter)
            {
                ret->set_contest_stat(iter->first, int_rng.rand(0,255));
            }
        }

        return ret;
    }

    pkmn::pokemon::sptr get_random_pokemon(
        pkmn::e_game game,
        const std::vector<pkmn::e_species>& pokemon_list,
        const std::vector<pkmn::e_item>& item_list,
        const std::vector<pkmn::e_move>& move_list
    )
    {
        pkmn::rng<size_t> rng;
        int generation = pkmn::priv::game_enum_to_generation(game);

        // Don't deal with Deoxys or Unown issues here.
        pkmn::e_species species = pkmn::e_species::NONE;
        if(generation == 3)
        {
            do
            {
                species = pokemon_list[rng.rand() % pokemon_list.size()];
            }
            while((species == pkmn::e_species::UNOWN) ||
                  (species == pkmn::e_species::DEOXYS));
        }
        else
        {
            species = pokemon_list[rng.rand() % pokemon_list.size()];
        }

        return get_random_pokemon(
                   species,
                   game,
                   item_list,
                   move_list
               );
    }

    pkmn::pokemon::sptr get_random_pokemon(pkmn::e_game game)
    {
        int generation = pkmn::priv::game_enum_to_generation(game);

        std::vector<pkmn::e_item> item_list = pkmn::database::get_item_list(game);
        std::vector<pkmn::e_move> move_list = pkmn::database::get_move_list(game);
        std::vector<pkmn::e_species> pokemon_list = pkmn::database::get_pokemon_list(
                                                        generation,
                                                        true
                                                    );

        return get_random_pokemon(
                   game,
                   pokemon_list,
                   item_list,
                   move_list
               );
    }

    void compare_pokemon(
        const pkmn::pokemon::sptr& pokemon1,
        const pkmn::pokemon::sptr& pokemon2
    )
    {
        pkmn::e_game game = pokemon1->get_game();
        int generation = pkmn::priv::game_enum_to_generation(game);

        // There is no way to determine what game an imported Generation I-II
        // Pokémon comes from, so LibPKMN defaults to a default valid game.
        if(generation >= 3)
        {
            ASSERT_EQ(game, pokemon2->get_game());
        }

        EXPECT_EQ(pokemon1->get_species(), pokemon2->get_species());
        EXPECT_EQ(pokemon1->get_form(), pokemon2->get_form());
        EXPECT_EQ(pokemon1->get_original_trainer_id(), pokemon2->get_original_trainer_id());
        EXPECT_EQ(pokemon1->get_experience(), pokemon2->get_experience());
        EXPECT_EQ(pokemon1->get_level(), pokemon2->get_level());
        EXPECT_EQ(pokemon1->get_nickname(), pokemon2->get_nickname());
        EXPECT_EQ(pokemon1->get_original_trainer_name(), pokemon2->get_original_trainer_name());

        const std::map<pkmn::e_stat, int>& EVs1 = pokemon1->get_EVs();
        const std::map<pkmn::e_stat, int>& EVs2 = pokemon2->get_EVs();
        for(auto iter = EVs1.begin(); iter != EVs1.end(); ++iter)
        {
            EXPECT_EQ(iter->second, EVs2.at(iter->first));
        }

        const std::map<pkmn::e_stat, int>& IVs1 = pokemon1->get_IVs();
        const std::map<pkmn::e_stat, int>& IVs2 = pokemon2->get_IVs();
        for(auto iter = IVs1.begin(); iter != IVs1.end(); ++iter)
        {
            EXPECT_EQ(iter->second, IVs2.at(iter->first));
        }

        const std::map<pkmn::e_stat, int>& stats1 = pokemon1->get_stats();
        const std::map<pkmn::e_stat, int>& stats2 = pokemon2->get_stats();
        for(auto iter = stats1.begin(); iter != stats1.end(); ++iter)
        {
            EXPECT_EQ(iter->second, stats2.at(iter->first));
        }

        if(pokemon2->get_game() == game)
        {
            EXPECT_EQ(pokemon1->get_icon_filepath(), pokemon2->get_icon_filepath());
            EXPECT_EQ(pokemon1->get_sprite_filepath(), pokemon2->get_sprite_filepath());
        }

        if(generation >= 2)
        {
            EXPECT_EQ(pokemon1->get_original_trainer_gender(), pokemon2->get_original_trainer_gender());
            EXPECT_EQ(pokemon1->get_current_trainer_friendship(), pokemon2->get_current_trainer_friendship());
            EXPECT_EQ(pokemon1->get_gender(), pokemon2->get_gender());
            EXPECT_EQ(pokemon1->is_shiny(), pokemon2->is_shiny());
            EXPECT_EQ(pokemon1->get_held_item(), pokemon2->get_held_item());
            EXPECT_EQ(pokemon1->get_level_met(), pokemon2->get_level_met());
            EXPECT_EQ(pokemon1->get_location_met(false), pokemon2->get_location_met(false));
        }
        if(generation >= 3)
        {
            EXPECT_EQ(pokemon1->get_ability(), pokemon2->get_ability());
            EXPECT_EQ(pokemon1->get_ball(), pokemon2->get_ball());
            EXPECT_EQ(pokemon1->get_personality(), pokemon2->get_personality());
            EXPECT_EQ(pokemon1->get_ball(), pokemon2->get_ball());

            const std::map<pkmn::e_marking, bool>& markings1 = pokemon1->get_markings();
            const std::map<pkmn::e_marking, bool>& markings2 = pokemon2->get_markings();
            for(auto iter = markings1.begin(); iter != markings1.end(); ++iter)
            {
                EXPECT_EQ(iter->second, markings2.at(iter->first));
            }

            const std::map<std::string, bool>& ribbons1 = pokemon1->get_ribbons();
            const std::map<std::string, bool>& ribbons2 = pokemon2->get_ribbons();
            for(auto iter = ribbons1.begin(); iter != ribbons1.end(); ++iter)
            {
                EXPECT_EQ(iter->second, ribbons2.at(iter->first));
            }

            const std::map<pkmn::e_contest_stat, int>& contest_stats1 = pokemon1->get_contest_stats();
            const std::map<pkmn::e_contest_stat, int>& contest_stats2 = pokemon2->get_contest_stats();
            for(auto iter = contest_stats1.begin(); iter != contest_stats1.end(); ++iter)
            {
                EXPECT_EQ(iter->second, contest_stats2.at(iter->first));
            }
        }
        if(generation >= 4)
        {
            EXPECT_EQ(pokemon1->get_location_met(true), pokemon2->get_location_met(true));
        }

        compare_attributes(
            pokemon1,
            pokemon2
        );
    }
}

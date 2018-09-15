/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/pokemon_subclass.hpp>
#include <pkmntest/util.hpp>

#include <pkmn/daycare.hpp>
#include <pkmn/pokemon_box.hpp>
#include <pkmn/pokemon_party.hpp>
#include <pkmn/enums/enum_to_string.hpp>

#include "utils/misc.hpp"

#include <gtest/gtest.h>

class pokemon_polymorphism_test: public ::testing::TestWithParam<pkmn::e_game>
{
    public:
        template <typename libpkmn_container_type>
        void test_setting_sptr_in_pokemon_container(
            pkmn::e_species species
        )
        {
            pkmn::e_game game = GetParam();

            std::shared_ptr<pkmntest::pokemon_subclass> outside_pokemon_sptr =
                std::make_shared<pkmntest::pokemon_subclass>(
                    species,
                    game
                );
            ASSERT_EQ(
                species,
                outside_pokemon_sptr->get_species()
            ) << pkmn::species_to_string(outside_pokemon_sptr->get_species());
            ASSERT_EQ(
                game,
                outside_pokemon_sptr->get_game()
            ) << pkmn::game_to_string(outside_pokemon_sptr->get_game());

            typename libpkmn_container_type::sptr libpkmn_container = libpkmn_container_type::make(game);
            libpkmn_container->set_pokemon(0, outside_pokemon_sptr);
        }

        template <typename libpkmn_container_type>
        void test_setting_reference_in_pokemon_container(
            pkmn::e_species species
        )
        {
            pkmn::e_game game = GetParam();

            pkmntest::pokemon_subclass outside_pokemon(species, game);

            ASSERT_EQ(
                species,
                outside_pokemon.get_species()
            ) << pkmn::species_to_string(outside_pokemon.get_species());
            ASSERT_EQ(
                game,
                outside_pokemon.get_game()
            ) << pkmn::game_to_string(outside_pokemon.get_game());

            typename libpkmn_container_type::sptr libpkmn_container = libpkmn_container_type::make(game);
            libpkmn_container->set_pokemon(0, outside_pokemon);
        }
};

// Adding outside Pokémon to box

TEST_P(pokemon_polymorphism_test, setting_sptr_in_libpkmn_pokemon_box)
{
    ASSERT_NO_FATAL_FAILURE(test_setting_sptr_in_pokemon_container<pkmn::pokemon_box>(
        pkmn::e_species::BULBASAUR
    )) << pkmn::game_to_string(GetParam());
}

TEST_P(pokemon_polymorphism_test, setting_reference_in_libpkmn_pokemon_box)
{
    ASSERT_NO_FATAL_FAILURE(test_setting_reference_in_pokemon_container<pkmn::pokemon_box>(
        pkmn::e_species::BULBASAUR
    )) << pkmn::game_to_string(GetParam());
}

// Adding outside Pokémon to party

TEST_P(pokemon_polymorphism_test, setting_sptr_in_libpkmn_pokemon_party)
{
    ASSERT_NO_FATAL_FAILURE(test_setting_sptr_in_pokemon_container<pkmn::pokemon_party>(
        pkmn::e_species::BULBASAUR
    )) << pkmn::game_to_string(GetParam());
}

TEST_P(pokemon_polymorphism_test, setting_reference_in_libpkmn_pokemon_party)
{
    ASSERT_NO_FATAL_FAILURE(test_setting_reference_in_pokemon_container<pkmn::pokemon_party>(
        pkmn::e_species::BULBASAUR
    )) << pkmn::game_to_string(GetParam());
}

// Adding outside Pokémon to daycare

TEST_P(pokemon_polymorphism_test, setting_sptr_in_libpkmn_daycare)
{
    const pkmn::e_game game = GetParam();
    const pkmn::e_species species = pkmn::e_species::BULBASAUR;

    if(pkmn::does_vector_contain_value(pkmntest::BREEDING_SUPPORTED_GAMES,game))
    {
        std::shared_ptr<pkmntest::pokemon_subclass> outside_pokemon_sptr =
            std::make_shared<pkmntest::pokemon_subclass>(
                species,
                game
            );
        ASSERT_EQ(
            species,
            outside_pokemon_sptr->get_species()
        ) << pkmn::species_to_string(outside_pokemon_sptr->get_species());
        ASSERT_EQ(
            game,
            outside_pokemon_sptr->get_game()
        ) << pkmn::game_to_string(outside_pokemon_sptr->get_game());

        pkmn::daycare::sptr daycare = pkmn::daycare::make(game);
        daycare->set_levelup_pokemon(0, outside_pokemon_sptr);

        if(daycare->can_breed_pokemon())
        {
            daycare->set_breeding_pokemon(0, outside_pokemon_sptr);
            daycare->set_breeding_pokemon(1, outside_pokemon_sptr);
        }
    }
    else
    {
        std::cout << "Currently unimplemented" << std::endl;
    }
}

TEST_P(pokemon_polymorphism_test, setting_reference_in_libpkmn_daycare)
{
    const pkmn::e_game game = GetParam();
    const pkmn::e_species species = pkmn::e_species::BULBASAUR;

    if(pkmn::does_vector_contain_value(pkmntest::BREEDING_SUPPORTED_GAMES,game))
    {
        pkmntest::pokemon_subclass outside_pokemon(
                                       pkmn::e_species::BULBASAUR,
                                       game
                                   );
        ASSERT_EQ(
            species,
            outside_pokemon.get_species()
        ) << pkmn::species_to_string(outside_pokemon.get_species());
        ASSERT_EQ(
            game,
            outside_pokemon.get_game()
        ) << pkmn::game_to_string(outside_pokemon.get_game());

        pkmn::daycare::sptr daycare = pkmn::daycare::make(game);
        daycare->set_levelup_pokemon(0, outside_pokemon);

        if(daycare->can_breed_pokemon())
        {
            daycare->set_breeding_pokemon(0, outside_pokemon);
        }
    }
    else
    {
        std::cout << "Currently unimplemented" << std::endl;
    }
}

INSTANTIATE_TEST_CASE_P(
    cpp_pokemon_polymorphism_test,
    pokemon_polymorphism_test,
    ::testing::ValuesIn(pkmntest::SUPPORTED_GAMES)
);

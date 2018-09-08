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

#include "polymorphism/pokemon.hpp"

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
                pkmn::e_species::BULBASAUR,
                outside_pokemon_sptr->get_species()
            ) << pkmn::species_to_string(outside_pokemon_sptr->get_species());
            ASSERT_EQ(
                game,
                outside_pokemon_sptr->get_game()
            ) << pkmn::game_to_string(outside_pokemon_sptr->get_game());

            typename libpkmn_container_type::sptr libpkmn_container = libpkmn_container_type::make(game);
            libpkmn_container->set_pokemon(0, outside_pokemon_sptr);
        }
};

TEST_P(pokemon_polymorphism_test, setting_sptr_in_libpkmn_pokemon_box)
{
    test_setting_sptr_in_pokemon_container<pkmn::pokemon_box>(
        pkmn::e_species::BULBASAUR
    );
}

TEST_P(pokemon_polymorphism_test, setting_sptr_in_libpkmn_pokemon_party)
{
    test_setting_sptr_in_pokemon_container<pkmn::pokemon_party>(
        pkmn::e_species::BULBASAUR
    );
}

TEST_P(pokemon_polymorphism_test, setting_sptr_in_libpkmn_daycare)
{
    pkmn::e_game game = GetParam();

    std::shared_ptr<pkmntest::pokemon_subclass> outside_pokemon_sptr =
        std::make_shared<pkmntest::pokemon_subclass>(
            pkmn::e_species::BULBASAUR,
            game
        );
    ASSERT_EQ(
        pkmn::e_species::BULBASAUR,
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
    }
}

INSTANTIATE_TEST_CASE_P(
    cpp_pokemon_polymorphism_test,
    pokemon_polymorphism_test,
    ::testing::ValuesIn(pkmntest::SUPPORTED_GAMES)
);

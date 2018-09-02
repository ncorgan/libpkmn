/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/breeding/child_info.hpp>
#include <pkmn/breeding/compatibility.hpp>

#include <pkmn/enums/enum_to_string.hpp>
#include <pkmn/enums/species.hpp>

#include <gtest/gtest.h>

#include <tuple>
#include <vector>

/*
 * Compatibility
 */

typedef std::tuple<
            pkmn::e_species, // species1
            pkmn::e_species, // species2
            bool             // should_be_compatible
        > breeding_compatibility_test_params_t;

class breeding_compatibility_test: public ::testing::TestWithParam<breeding_compatibility_test_params_t> {};

TEST_P(breeding_compatibility_test, test_breeding_compatibility)
{
    breeding_compatibility_test_params_t test_params = GetParam();

    // Extract input tuple into readable variable names.
    pkmn::e_species species1  = std::get<0>(test_params);
    pkmn::e_species species2  = std::get<1>(test_params);
    bool should_be_compatible = std::get<2>(test_params);

    EXPECT_EQ(
        should_be_compatible,
        pkmn::breeding::are_pokemon_species_compatible(species1, species2)
    ) << pkmn::species_to_string(species1) << " " << pkmn::species_to_string(species2);
}

static const std::vector<breeding_compatibility_test_params_t> BREEDING_COMPATIBILITY_TEST_PARAMS =
{
    // Same evolutionary line, should be compatible
    breeding_compatibility_test_params_t(
        pkmn::e_species::BULBASAUR,
        pkmn::e_species::BULBASAUR,
        true
    ),
    breeding_compatibility_test_params_t(
        pkmn::e_species::CHARMANDER,
        pkmn::e_species::CHARMELEON,
        true
    ),
    breeding_compatibility_test_params_t(
        pkmn::e_species::SQUIRTLE,
        pkmn::e_species::BLASTOISE,
        true
    ),
    breeding_compatibility_test_params_t(
        pkmn::e_species::VAPOREON,
        pkmn::e_species::SYLVEON,
        true
    ),

    // Common egg group, should be compatible
    breeding_compatibility_test_params_t(
        pkmn::e_species::PIKACHU,
        pkmn::e_species::TOGEKISS,
        true
    ),
    breeding_compatibility_test_params_t(
        pkmn::e_species::SKITTY,
        pkmn::e_species::WAILORD,
        true
    ),

    // "Undiscovered" egg group, cannot breed
    breeding_compatibility_test_params_t(
        pkmn::e_species::IGGLYBUFF,
        pkmn::e_species::DITTO,
        false
    ),
    breeding_compatibility_test_params_t(
        pkmn::e_species::AZURILL,
        pkmn::e_species::DITTO,
        false
    ),
    breeding_compatibility_test_params_t(
        pkmn::e_species::MEW,
        pkmn::e_species::DITTO,
        false
    ),

    // Two Dittos cannot breed
    breeding_compatibility_test_params_t(
        pkmn::e_species::DITTO,
        pkmn::e_species::DITTO,
        false
    ),

    // Two Pokémon with a 100% chance to be the same gender are incompatible,
    // even with common egg groups.
    breeding_compatibility_test_params_t(
        pkmn::e_species::JYNX,
        pkmn::e_species::ILLUMISE,
        false
    ),
    breeding_compatibility_test_params_t(
        pkmn::e_species::HITMONCHAN,
        pkmn::e_species::VOLBEAT,
        false
    ),
};

INSTANTIATE_TEST_CASE_P(
    cpp_breeding_compatibility_test,
    breeding_compatibility_test,
    ::testing::ValuesIn(BREEDING_COMPATIBILITY_TEST_PARAMS)
);

/*
 * Expected child species
 */

typedef std::tuple<
            pkmn::e_species,             // Mother species
            pkmn::e_species,             // Father species
            pkmn::e_game,                // Game
            bool,                        // Are parents interchangeable?
            std::vector<pkmn::e_species> // Possible child species
        > possible_child_species_test_params_t;

class possible_child_species_test: public ::testing::TestWithParam<possible_child_species_test_params_t> {};

TEST_P(possible_child_species_test, test_possible_child_species)
{
    possible_child_species_test_params_t test_params = GetParam();

    // Extract input tuple into readable variable names.
    pkmn::e_species mother_species                             = std::get<0>(test_params);
    pkmn::e_species father_species                             = std::get<1>(test_params);
    const pkmn::e_game& game                                   = std::get<2>(test_params);
    bool are_parents_interchangeable                           = std::get<3>(test_params);
    const std::vector<pkmn::e_species>& possible_child_species = std::get<4>(test_params);

    EXPECT_EQ(
        possible_child_species,
        pkmn::breeding::get_possible_child_species(
            mother_species,
            father_species,
            game
        )
    ) << pkmn::species_to_string(mother_species) << " "
      << pkmn::species_to_string(father_species) << " "
      << pkmn::game_to_string(game);
    if(are_parents_interchangeable)
    {
        EXPECT_EQ(
            possible_child_species,
            pkmn::breeding::get_possible_child_species(
                father_species,
                mother_species,
                game
            )
        ) << pkmn::species_to_string(father_species) << " "
          << pkmn::species_to_string(mother_species) << " "
          << pkmn::game_to_string(game);
    }
}

static const std::vector<possible_child_species_test_params_t> POSSIBLE_CHILD_SPECIES_TEST_PARAMS =
{
    // Any Pokémon, when bred with Ditto, will determine the child's species, no
    // matter the gender.
    possible_child_species_test_params_t(
        pkmn::e_species::JYNX,
        pkmn::e_species::DITTO,
        pkmn::e_game::GOLD,
        false,
        {pkmn::e_species::SMOOCHUM}
    ),
    possible_child_species_test_params_t(
        pkmn::e_species::DITTO,
        pkmn::e_species::HITMONCHAN,
        pkmn::e_game::GOLD,
        false,
        {pkmn::e_species::TYROGUE}
    ),

    // Both Ditto and genderless species should be able to be given as either the
    // mother or father.
    possible_child_species_test_params_t(
        pkmn::e_species::MAGNETON,
        pkmn::e_species::DITTO,
        pkmn::e_game::GOLD,
        true,
        {pkmn::e_species::MAGNEMITE}
    ),
    possible_child_species_test_params_t(
        pkmn::e_species::DITTO,
        pkmn::e_species::MAGNETON,
        pkmn::e_game::GOLD,
        true,
        {pkmn::e_species::MAGNEMITE}
    ),

    // Some species had baby Pokémon added to their evolutionary lines in later games.
    // In the older games, they should only be able to breed to their existing earliest
    // evolution, and later, they should be able to breed to both this species and the
    // new baby evolution.

    possible_child_species_test_params_t(
        pkmn::e_species::MARILL,
        pkmn::e_species::DITTO,
        pkmn::e_game::GOLD,
        true,
        {pkmn::e_species::MARILL}
    ),
    possible_child_species_test_params_t(
        pkmn::e_species::AZUMARILL,
        pkmn::e_species::DITTO,
        pkmn::e_game::GOLD,
        true,
        {pkmn::e_species::MARILL}
    ),
    possible_child_species_test_params_t(
        pkmn::e_species::MARILL,
        pkmn::e_species::DITTO,
        pkmn::e_game::RUBY,
        true,
        {pkmn::e_species::MARILL, pkmn::e_species::AZURILL}
    ),
    possible_child_species_test_params_t(
        pkmn::e_species::AZUMARILL,
        pkmn::e_species::DITTO,
        pkmn::e_game::RUBY,
        true,
        {pkmn::e_species::MARILL, pkmn::e_species::AZURILL}
    ),

    possible_child_species_test_params_t(
        pkmn::e_species::WOBBUFFET,
        pkmn::e_species::DITTO,
        pkmn::e_game::GOLD,
        true,
        {pkmn::e_species::WOBBUFFET}
    ),
    possible_child_species_test_params_t(
        pkmn::e_species::WOBBUFFET,
        pkmn::e_species::DITTO,
        pkmn::e_game::RUBY,
        true,
        {pkmn::e_species::WOBBUFFET, pkmn::e_species::WYNAUT}
    ),

    possible_child_species_test_params_t(
        pkmn::e_species::ROSELIA,
        pkmn::e_species::DITTO,
        pkmn::e_game::RUBY,
        true,
        {pkmn::e_species::ROSELIA}
    ),
    possible_child_species_test_params_t(
        pkmn::e_species::ROSELIA,
        pkmn::e_species::DITTO,
        pkmn::e_game::DIAMOND,
        true,
        {pkmn::e_species::ROSELIA, pkmn::e_species::BUDEW}
    ),
    possible_child_species_test_params_t(
        pkmn::e_species::ROSERADE,
        pkmn::e_species::DITTO,
        pkmn::e_game::DIAMOND,
        true,
        {pkmn::e_species::ROSELIA, pkmn::e_species::BUDEW}
    ),

    possible_child_species_test_params_t(
        pkmn::e_species::CHIMECHO,
        pkmn::e_species::DITTO,
        pkmn::e_game::RUBY,
        true,
        {pkmn::e_species::CHIMECHO}
    ),
    possible_child_species_test_params_t(
        pkmn::e_species::CHIMECHO,
        pkmn::e_species::DITTO,
        pkmn::e_game::DIAMOND,
        true,
        {pkmn::e_species::CHIMECHO, pkmn::e_species::CHINGLING}
    ),

    possible_child_species_test_params_t(
        pkmn::e_species::SUDOWOODO,
        pkmn::e_species::DITTO,
        pkmn::e_game::GOLD,
        true,
        {pkmn::e_species::SUDOWOODO}
    ),
    possible_child_species_test_params_t(
        pkmn::e_species::SUDOWOODO,
        pkmn::e_species::DITTO,
        pkmn::e_game::DIAMOND,
        true,
        {pkmn::e_species::SUDOWOODO, pkmn::e_species::BONSLY}
    ),

    possible_child_species_test_params_t(
        pkmn::e_species::MR_MIME,
        pkmn::e_species::DITTO,
        pkmn::e_game::GOLD,
        true,
        {pkmn::e_species::MR_MIME}
    ),
    possible_child_species_test_params_t(
        pkmn::e_species::MR_MIME,
        pkmn::e_species::DITTO,
        pkmn::e_game::DIAMOND,
        true,
        {pkmn::e_species::MR_MIME, pkmn::e_species::MIME_JR}
    ),

    possible_child_species_test_params_t(
        pkmn::e_species::CHANSEY,
        pkmn::e_species::DITTO,
        pkmn::e_game::GOLD,
        false,
        {pkmn::e_species::CHANSEY}
    ),
    possible_child_species_test_params_t(
        pkmn::e_species::BLISSEY,
        pkmn::e_species::DITTO,
        pkmn::e_game::GOLD,
        false,
        {pkmn::e_species::CHANSEY}
    ),
    possible_child_species_test_params_t(
        pkmn::e_species::CHANSEY,
        pkmn::e_species::DITTO,
        pkmn::e_game::DIAMOND,
        false,
        {pkmn::e_species::CHANSEY, pkmn::e_species::HAPPINY}
    ),
    possible_child_species_test_params_t(
        pkmn::e_species::BLISSEY,
        pkmn::e_species::DITTO,
        pkmn::e_game::DIAMOND,
        false,
        {pkmn::e_species::CHANSEY, pkmn::e_species::HAPPINY}
    ),

    possible_child_species_test_params_t(
        pkmn::e_species::MANTINE,
        pkmn::e_species::DITTO,
        pkmn::e_game::GOLD,
        true,
        {pkmn::e_species::MANTINE}
    ),
    possible_child_species_test_params_t(
        pkmn::e_species::MANTINE,
        pkmn::e_species::DITTO,
        pkmn::e_game::DIAMOND,
        true,
        {pkmn::e_species::MANTINE, pkmn::e_species::MANTYKE}
    ),

    possible_child_species_test_params_t(
        pkmn::e_species::SNORLAX,
        pkmn::e_species::DITTO,
        pkmn::e_game::GOLD,
        true,
        {pkmn::e_species::SNORLAX}
    ),
    possible_child_species_test_params_t(
        pkmn::e_species::SNORLAX,
        pkmn::e_species::DITTO,
        pkmn::e_game::DIAMOND,
        true,
        {pkmn::e_species::SNORLAX, pkmn::e_species::MUNCHLAX}
    ),

    // In Generation V+, Nidoran♂ and Volbeat has a chance to produce one of two different species.
    // Their female counterparts have always had this chance.

    possible_child_species_test_params_t(
        pkmn::e_species::NIDORAN_F,
        pkmn::e_species::DITTO,
        pkmn::e_game::GOLD,
        false,
        {pkmn::e_species::NIDORAN_F, pkmn::e_species::NIDORAN_M}
    ),
    possible_child_species_test_params_t(
        pkmn::e_species::NIDORAN_F,
        pkmn::e_species::DITTO,
        pkmn::e_game::BLACK,
        false,
        {pkmn::e_species::NIDORAN_F, pkmn::e_species::NIDORAN_M}
    ),
    possible_child_species_test_params_t(
        pkmn::e_species::DITTO,
        pkmn::e_species::NIDORAN_M,
        pkmn::e_game::GOLD,
        false,
        {pkmn::e_species::NIDORAN_M}
    ),
    possible_child_species_test_params_t(
        pkmn::e_species::DITTO,
        pkmn::e_species::NIDORAN_M,
        pkmn::e_game::BLACK,
        false,
        {pkmn::e_species::NIDORAN_F, pkmn::e_species::NIDORAN_M}
    ),

    possible_child_species_test_params_t(
        pkmn::e_species::ILLUMISE,
        pkmn::e_species::DITTO,
        pkmn::e_game::RUBY,
        false,
        {pkmn::e_species::VOLBEAT, pkmn::e_species::ILLUMISE}
    ),
    possible_child_species_test_params_t(
        pkmn::e_species::ILLUMISE,
        pkmn::e_species::DITTO,
        pkmn::e_game::BLACK,
        false,
        {pkmn::e_species::VOLBEAT, pkmn::e_species::ILLUMISE}
    ),
    possible_child_species_test_params_t(
        pkmn::e_species::DITTO,
        pkmn::e_species::VOLBEAT,
        pkmn::e_game::RUBY,
        false,
        {pkmn::e_species::VOLBEAT}
    ),
    possible_child_species_test_params_t(
        pkmn::e_species::DITTO,
        pkmn::e_species::VOLBEAT,
        pkmn::e_game::BLACK,
        false,
        {pkmn::e_species::VOLBEAT, pkmn::e_species::ILLUMISE}
    ),

    // Manaphy is an odd case in that it breeds to a Pokémon that doesn't evolve into it.
    possible_child_species_test_params_t(
        pkmn::e_species::MANAPHY,
        pkmn::e_species::DITTO,
        pkmn::e_game::DIAMOND,
        true,
        {pkmn::e_species::PHIONE}
    ),
    possible_child_species_test_params_t(
        pkmn::e_species::PHIONE,
        pkmn::e_species::DITTO,
        pkmn::e_game::DIAMOND,
        true,
        {pkmn::e_species::PHIONE}
    ),
};

INSTANTIATE_TEST_CASE_P(
    cpp_possible_child_species_test,
    possible_child_species_test,
    ::testing::ValuesIn(POSSIBLE_CHILD_SPECIES_TEST_PARAMS)
);

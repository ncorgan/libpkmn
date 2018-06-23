/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/breeding/child_info.hpp>
#include <pkmn/breeding/compatibility.hpp>

#include <gtest/gtest.h>

#include <tuple>
#include <vector>

/*
 * Compatibility
 */

typedef std::tuple<std::string, std::string, bool> breeding_compatibility_test_params_t;

class breeding_compatibility_test: public ::testing::TestWithParam<breeding_compatibility_test_params_t> {};

TEST_P(breeding_compatibility_test, test_breeding_compatibility)
{
    breeding_compatibility_test_params_t test_params = GetParam();

    // Extract input tuple into readable variable names.
    const std::string& species1 = std::get<0>(test_params);
    const std::string& species2 = std::get<1>(test_params);
    bool should_be_compatible   = std::get<2>(test_params);

    EXPECT_EQ(
        should_be_compatible,
        pkmn::breeding::are_pokemon_species_compatible(species1, species2)
    );
}

static const std::vector<breeding_compatibility_test_params_t> BREEDING_COMPATIBILITY_TEST_PARAMS =
{
    // Same evolutionary line, should be compatible
    breeding_compatibility_test_params_t("Bulbasaur", "Bulbasaur", true),
    breeding_compatibility_test_params_t("Charmander", "Charmeleon", true),
    breeding_compatibility_test_params_t("Squirtle", "Blastoise", true),
    breeding_compatibility_test_params_t("Vaporeon", "Sylveon", true),

    // Common egg group, should be compatible
    breeding_compatibility_test_params_t("Pikachu", "Togekiss", true),
    breeding_compatibility_test_params_t("Skitty", "Wailord", true),

    // "Undiscovered" egg group, cannot breed
    breeding_compatibility_test_params_t("Igglybuff", "Ditto", false),
    breeding_compatibility_test_params_t("Azurill", "Ditto", false),
    breeding_compatibility_test_params_t("Mew", "Ditto", false),

    // Two Dittos cannot breed
    breeding_compatibility_test_params_t("Ditto", "Ditto", false),

    // Two Pokémon with a 100% chance to be the same gender are incompatible,
    // even with common egg groups.
    breeding_compatibility_test_params_t("Jynx", "Illumise", false),
    breeding_compatibility_test_params_t("Hitmonchan", "Volbeat", false)
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
            std::string,             // Mother species
            std::string,             // Father species
            pkmn::e_game,            // Game
            bool,                    // Are parents interchangeable?
            std::vector<std::string> // Possible child species
        > possible_child_species_test_params_t;

class possible_child_species_test: public ::testing::TestWithParam<possible_child_species_test_params_t> {};

TEST_P(possible_child_species_test, test_possible_child_species)
{
    possible_child_species_test_params_t test_params = GetParam();

    // Extract input tuple into readable variable names.
    const std::string& mother_species                      = std::get<0>(test_params);
    const std::string& father_species                      = std::get<1>(test_params);
    const pkmn::e_game& game                               = std::get<2>(test_params);
    bool are_parents_interchangeable                       = std::get<3>(test_params);
    const std::vector<std::string>& possible_child_species = std::get<4>(test_params);

    EXPECT_EQ(
        possible_child_species,
        pkmn::breeding::get_possible_child_species(
            mother_species,
            father_species,
            game
        )
    );
    if(are_parents_interchangeable)
    {
        EXPECT_EQ(
            possible_child_species,
            pkmn::breeding::get_possible_child_species(
                father_species,
                mother_species,
                game
            )
        );
    }
}

static const std::vector<possible_child_species_test_params_t> POSSIBLE_CHILD_SPECIES_TEST_PARAMS =
{
    // Any Pokémon, when bred with Ditto, will determine the child's species, no
    // matter the gender.
    possible_child_species_test_params_t("Jynx", "Ditto", pkmn::e_game::GOLD, false, {"Smoochum"}),
    possible_child_species_test_params_t("Ditto", "Hitmonchan", pkmn::e_game::GOLD, false, {"Tyrogue"}),

    // Both Ditto and genderless species should be able to be given as either the
    // mother or father.
    possible_child_species_test_params_t("Magneton", "Ditto", pkmn::e_game::GOLD, true, {"Magnemite"}),
    possible_child_species_test_params_t("Ditto", "Magneton", pkmn::e_game::GOLD, true, {"Magnemite"}),

    // Some species had baby Pokémon added to their evolutionary lines in later games.
    // In the older games, they should only be able to breed to their existing earliest
    // evolution, and later, they should be able to breed to both this species and the
    // new baby evolution.

    possible_child_species_test_params_t("Marill", "Ditto", pkmn::e_game::GOLD, true, {"Marill"}),
    possible_child_species_test_params_t("Azumarill", "Ditto", pkmn::e_game::GOLD, true, {"Marill"}),
    possible_child_species_test_params_t("Marill", "Ditto", pkmn::e_game::RUBY, true, {"Marill", "Azurill"}),
    possible_child_species_test_params_t("Azumarill", "Ditto", pkmn::e_game::RUBY, true, {"Marill", "Azurill"}),

    possible_child_species_test_params_t("Wobbuffet", "Ditto", pkmn::e_game::GOLD, true, {"Wobbuffet"}),
    possible_child_species_test_params_t("Wobbuffet", "Ditto", pkmn::e_game::RUBY, true, {"Wobbuffet", "Wynaut"}),

    possible_child_species_test_params_t("Roselia", "Ditto", pkmn::e_game::RUBY, true, {"Roselia"}),
    possible_child_species_test_params_t("Roselia", "Ditto", pkmn::e_game::DIAMOND, true, {"Roselia", "Budew"}),
    possible_child_species_test_params_t("Roserade", "Ditto", pkmn::e_game::DIAMOND, true, {"Roselia", "Budew"}),

    possible_child_species_test_params_t("Chimecho", "Ditto", pkmn::e_game::RUBY, true, {"Chimecho"}),
    possible_child_species_test_params_t("Chimecho", "Ditto", pkmn::e_game::DIAMOND, true, {"Chimecho", "Chingling"}),

    possible_child_species_test_params_t("Sudowoodo", "Ditto", pkmn::e_game::GOLD, true, {"Sudowoodo"}),
    possible_child_species_test_params_t("Sudowoodo", "Ditto", pkmn::e_game::DIAMOND, true, {"Sudowoodo", "Bonsly"}),

    possible_child_species_test_params_t("Mr. Mime", "Ditto", pkmn::e_game::GOLD, true, {"Mr. Mime"}),
    possible_child_species_test_params_t("Mr. Mime", "Ditto", pkmn::e_game::DIAMOND, true, {"Mr. Mime", "Mime Jr."}),

    possible_child_species_test_params_t("Chansey", "Ditto", pkmn::e_game::GOLD, false, {"Chansey"}),
    possible_child_species_test_params_t("Blissey", "Ditto", pkmn::e_game::GOLD, false, {"Chansey"}),
    possible_child_species_test_params_t("Chansey", "Ditto", pkmn::e_game::DIAMOND, false, {"Chansey", "Happiny"}),
    possible_child_species_test_params_t("Blissey", "Ditto", pkmn::e_game::DIAMOND, false, {"Chansey", "Happiny"}),

    possible_child_species_test_params_t("Mantine", "Ditto", pkmn::e_game::GOLD, true, {"Mantine"}),
    possible_child_species_test_params_t("Mantine", "Ditto", pkmn::e_game::DIAMOND, true, {"Mantine", "Mantyke"}),

    possible_child_species_test_params_t("Snorlax", "Ditto", pkmn::e_game::GOLD, true, {"Snorlax"}),
    possible_child_species_test_params_t("Snorlax", "Ditto", pkmn::e_game::DIAMOND, true, {"Snorlax", "Munchlax"}),

    // In Generation V+, Nidoran♂ and Volbeat has a chance to produce one of two different species.
    // Their female counterparts have always had this chance.

    possible_child_species_test_params_t("Nidoran♀", "Ditto", pkmn::e_game::GOLD, false, {"Nidoran♀", "Nidoran♂"}),
    possible_child_species_test_params_t("Nidoran♀", "Ditto", pkmn::e_game::BLACK, false, {"Nidoran♀", "Nidoran♂"}),
    possible_child_species_test_params_t("Ditto", "Nidoran♂", pkmn::e_game::GOLD, false, {"Nidoran♂"}),
    possible_child_species_test_params_t("Ditto", "Nidoran♂", pkmn::e_game::BLACK, false, {"Nidoran♀", "Nidoran♂"}),

    possible_child_species_test_params_t("Illumise", "Ditto", pkmn::e_game::RUBY, false, {"Volbeat", "Illumise"}),
    possible_child_species_test_params_t("Illumise", "Ditto", pkmn::e_game::BLACK, false, {"Volbeat", "Illumise"}),
    possible_child_species_test_params_t("Ditto", "Volbeat", pkmn::e_game::RUBY, false, {"Volbeat"}),
    possible_child_species_test_params_t("Ditto", "Volbeat", pkmn::e_game::BLACK, false, {"Volbeat", "Illumise"}),

    // Manaphy is an odd case in that it breeds to a Pokémon that doesn't evolve into it.
    possible_child_species_test_params_t("Manaphy", "Ditto", pkmn::e_game::DIAMOND, true, {"Phione"}),
    possible_child_species_test_params_t("Phione", "Ditto", pkmn::e_game::DIAMOND, true, {"Phione"}),
};

INSTANTIATE_TEST_CASE_P(
    cpp_possible_child_species_test,
    possible_child_species_test,
    ::testing::ValuesIn(POSSIBLE_CHILD_SPECIES_TEST_PARAMS)
);

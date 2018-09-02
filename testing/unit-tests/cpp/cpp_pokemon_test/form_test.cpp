/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "private_exports.hpp"

#include <pkmntest/util.hpp>

#include <pkmn/pokemon.hpp>

#include <boost/assign.hpp>
#include <boost/filesystem.hpp>

#include <gtest/gtest.h>

#include <iostream>

namespace fs = boost::filesystem;

static const std::vector<pkmn::e_game> gen1_test_games =
{
    pkmn::e_game::RED, pkmn::e_game::BLUE, pkmn::e_game::YELLOW,

    pkmn::e_game::GOLD, pkmn::e_game::SILVER, pkmn::e_game::CRYSTAL,

    pkmn::e_game::RUBY, pkmn::e_game::SAPPHIRE, pkmn::e_game::EMERALD,
    pkmn::e_game::FIRERED, pkmn::e_game::LEAFGREEN,
    pkmn::e_game::COLOSSEUM, pkmn::e_game::XD
};

static const std::vector<pkmn::e_game> gen2_test_games =
{
    pkmn::e_game::GOLD, pkmn::e_game::SILVER, pkmn::e_game::CRYSTAL,

    pkmn::e_game::RUBY, pkmn::e_game::SAPPHIRE, pkmn::e_game::EMERALD,
    pkmn::e_game::FIRERED, pkmn::e_game::LEAFGREEN,
    pkmn::e_game::COLOSSEUM, pkmn::e_game::XD
};

static const std::vector<pkmn::e_game> gen3_test_games =
{
    pkmn::e_game::RUBY, pkmn::e_game::SAPPHIRE, pkmn::e_game::EMERALD,
    pkmn::e_game::FIRERED, pkmn::e_game::LEAFGREEN,
    pkmn::e_game::COLOSSEUM, pkmn::e_game::XD
};

class gen1_form_test: public ::testing::TestWithParam<pkmn::e_game> {};
class gen2_form_test: public ::testing::TestWithParam<pkmn::e_game> {};
class gen3_form_test: public ::testing::TestWithParam<pkmn::e_game> {};

static const std::vector<pkmn::e_species> gen1_pokemon_with_xy_mega_forms =
{
    pkmn::e_species::VENUSAUR,
    pkmn::e_species::BLASTOISE,
    pkmn::e_species::ALAKAZAM,
    pkmn::e_species::GENGAR,
    pkmn::e_species::KANGASKHAN,
    pkmn::e_species::PINSIR,
    pkmn::e_species::GYARADOS,
    pkmn::e_species::AERODACTYL,
};

static const std::vector<pkmn::e_species> gen1_pokemon_with_oras_mega_forms =
{
    pkmn::e_species::BEEDRILL,
    pkmn::e_species::PIDGEOT,
    pkmn::e_species::SLOWBRO,
};

static const std::vector<pkmn::e_species> gen2_pokemon_with_xy_mega_forms =
{
    pkmn::e_species::AMPHAROS,
    pkmn::e_species::SCIZOR,
    pkmn::e_species::HERACROSS,
    pkmn::e_species::HOUNDOOM,
    pkmn::e_species::TYRANITAR,
};

static const std::vector<pkmn::e_species> gen3_pokemon_with_xy_mega_forms =
{
    pkmn::e_species::BLAZIKEN,
    pkmn::e_species::GARDEVOIR,
    pkmn::e_species::MAWILE,
    pkmn::e_species::AGGRON,
    pkmn::e_species::MEDICHAM,
    pkmn::e_species::MANECTRIC,
    pkmn::e_species::BANETTE,
    pkmn::e_species::ABSOL,
    pkmn::e_species::LATIAS,
    pkmn::e_species::LATIOS,
    pkmn::e_species::GARCHOMP,
};

static const std::vector<pkmn::e_species> gen3_pokemon_with_oras_mega_forms =
{
    pkmn::e_species::SCEPTILE,
    pkmn::e_species::SWAMPERT,
    pkmn::e_species::SABLEYE,
    pkmn::e_species::SHARPEDO,
    pkmn::e_species::CAMERUPT,
    pkmn::e_species::ALTARIA,
    pkmn::e_species::GLALIE,
    pkmn::e_species::SALAMENCE,
    pkmn::e_species::METAGROSS,
    pkmn::e_species::RAYQUAZA,
};

TEST_P(gen1_form_test, test_gen1_pokemon_forms)
{
    pkmn::e_game game = GetParam();
    int generation = pkmn::priv::game_enum_to_generation(game);

    bool is_oras = (game == pkmn::e_game::OMEGA_RUBY) ||
                   (game == pkmn::e_game::ALPHA_SAPPHIRE);

    // Check that Mega forms only work in their given games
    for(auto iter = gen1_pokemon_with_xy_mega_forms.begin();
             iter != gen1_pokemon_with_xy_mega_forms.end();
             ++iter)
    {
        if(generation >= 6)
        {
            (void)pkmn::pokemon::make(
                      *iter,
                      game,
                      "Mega",
                      100
                  );
        }
        else
        {
            EXPECT_THROW(
                (void)pkmn::pokemon::make(
                          *iter,
                          game,
                          "Mega",
                          100
                      );
            , std::invalid_argument);
        }
    }
    for(auto iter = gen1_pokemon_with_oras_mega_forms.begin();
             iter != gen1_pokemon_with_oras_mega_forms.end();
             ++iter)
    {
        if(is_oras)
        {
            (void)pkmn::pokemon::make(
                      *iter,
                      game,
                      "Mega",
                      100
                  );
        }
        else
        {
            EXPECT_THROW(
                (void)pkmn::pokemon::make(
                          *iter,
                          game,
                          "Mega",
                          100
                      );
            , std::invalid_argument);
        }
    }

    // Cosplay Pikachu should only work for OR/AS
    static const pkmn::database::pokemon_entry oras_pikachu(
        pkmn::e_species::PIKACHU,
        pkmn::e_game::OMEGA_RUBY,
        ""
    );
    const std::vector<std::string>& cosplay_forms = oras_pikachu.get_forms();
    for(auto iter = cosplay_forms.begin()+1;
             iter != cosplay_forms.end();
             ++iter)
    {
        if(is_oras)
        {
            (void)pkmn::pokemon::make(
                      pkmn::e_species::PIKACHU,
                      game,
                      *iter,
                      5
                  );
        }
        else
        {
            EXPECT_THROW(
                (void)pkmn::pokemon::make(
                          pkmn::e_species::PIKACHU,
                          game,
                          *iter,
                          5
                      );
            , std::invalid_argument);
        }
    }

    // Hardcode Mega X/Y cases
    if(generation >= 6)
    {
        (void)pkmn::pokemon::make(
                  pkmn::e_species::CHARIZARD,
                  game,
                  "Mega X",
                  50
              );
        (void)pkmn::pokemon::make(
                  pkmn::e_species::CHARIZARD,
                  game,
                  "Mega Y",
                  50
              );
        (void)pkmn::pokemon::make(
                  pkmn::e_species::MEWTWO,
                  game,
                  "Mega X",
                  50
              );
        (void)pkmn::pokemon::make(
                  pkmn::e_species::MEWTWO,
                  game,
                  "Mega Y",
                  50
              );
    }
    else
    {
        EXPECT_THROW(
            (void)pkmn::pokemon::make(
                      pkmn::e_species::CHARIZARD,
                      game,
                      "Mega X",
                      50
                  );
        , std::invalid_argument);
        EXPECT_THROW(
            (void)pkmn::pokemon::make(
                      pkmn::e_species::CHARIZARD,
                      game,
                      "Mega Y",
                      50
                  );
        , std::invalid_argument);
        EXPECT_THROW(
            (void)pkmn::pokemon::make(
                      pkmn::e_species::MEWTWO,
                      game,
                      "Mega X",
                      50
                  );
        , std::invalid_argument);
        EXPECT_THROW(
            (void)pkmn::pokemon::make(
                      pkmn::e_species::MEWTWO,
                      game,
                      "Mega Y",
                      50
                  );
        , std::invalid_argument);
    }
}

TEST_P(gen2_form_test, test_gen2_pokemon_forms)
{
    pkmn::e_game game = GetParam();
    int generation = pkmn::priv::game_enum_to_generation(game);

    bool is_oras = (game == pkmn::e_game::OMEGA_RUBY) ||
                   (game == pkmn::e_game::ALPHA_SAPPHIRE);

    // Check that Mega forms only work in their given games
    for(auto iter = gen2_pokemon_with_xy_mega_forms.begin();
             iter != gen2_pokemon_with_xy_mega_forms.end();
             ++iter)
    {
        if(generation >= 6)
        {
            (void)pkmn::pokemon::make(
                      *iter,
                      game,
                      "Mega",
                      100
                  );
        }
        else
        {
            EXPECT_THROW(
                (void)pkmn::pokemon::make(
                          *iter,
                          game,
                          "Mega",
                          100
                      );
            , std::invalid_argument);
        }
    }

    if(is_oras)
    {
        (void)pkmn::pokemon::make(
                  pkmn::e_species::STEELIX,
                  game,
                  "Mega",
                  100
              );
    }
    else
    {
        EXPECT_THROW(
            (void)pkmn::pokemon::make(
                      pkmn::e_species::STEELIX,
                      game,
                      "Mega",
                      100
                  );
        , std::invalid_argument);
    }

    // Spiky-eared Pichu should only work in HG/SS
    if((game == pkmn::e_game::HEARTGOLD) || (game == pkmn::e_game::SOULSILVER))
    {
        pkmn::pokemon::sptr pichu = pkmn::pokemon::make(
                                        pkmn::e_species::PICHU,
                                        game,
                                        "Spiky-eared",
                                       100
                                    );
        EXPECT_TRUE(pichu->is_shiny());
        EXPECT_EQ(pkmn::e_gender::FEMALE, pichu->get_gender());
    }
    else
    {
        EXPECT_THROW(
            (void)pkmn::pokemon::make(
                      pkmn::e_species::PICHU,
                      game,
                      "Spiky-eared",
                      100
                  );
        , std::invalid_argument);
    }

    // Unown's "!" and "?" forms aren't in Generation II
    static const pkmn::database::pokemon_entry oras_unown(pkmn::e_species::UNOWN, pkmn::e_game::OMEGA_RUBY, "");
    const std::vector<std::string>& unown_forms = oras_unown.get_forms();
    for(size_t i = 0; i < 26; ++i)
    {
        pkmn::pokemon::sptr unown = pkmn::pokemon::make(
                                        pkmn::e_species::UNOWN,
                                        game,
                                        unown_forms.at(i),
                                        10
                                    );
        EXPECT_TRUE(fs::exists(unown->get_icon_filepath()));
        EXPECT_TRUE(fs::exists(unown->get_sprite_filepath()));
    }
    if(generation == 2)
    {
        EXPECT_THROW(
            (void)pkmn::pokemon::make(
                      pkmn::e_species::UNOWN,
                      game,
                      "!",
                      10
                  );
        , std::invalid_argument);
        EXPECT_THROW(
            (void)pkmn::pokemon::make(
                      pkmn::e_species::UNOWN,
                      game,
                      "?",
                      10
                  );
        , std::invalid_argument);
    }
    else
    {
        pkmn::pokemon::sptr unown = pkmn::pokemon::make(
                                        pkmn::e_species::UNOWN,
                                        game,
                                        "!",
                                        10
                                    );
        EXPECT_TRUE(fs::exists(unown->get_icon_filepath()));
        EXPECT_TRUE(fs::exists(unown->get_sprite_filepath()));

        unown = pkmn::pokemon::make(
                    pkmn::e_species::UNOWN,
                    game,
                    "?",
                    10
                );
        EXPECT_TRUE(fs::exists(unown->get_icon_filepath()));
        EXPECT_TRUE(fs::exists(unown->get_sprite_filepath()));
    }
}

TEST_P(gen3_form_test, test_gba_pokemon_forms)
{
    pkmn::e_game game = GetParam();
    int generation = pkmn::priv::game_enum_to_generation(game);

    bool is_oras = (game == pkmn::e_game::OMEGA_RUBY) ||
                   (game == pkmn::e_game::ALPHA_SAPPHIRE);

    // Check that Mega forms only work in their given games
    for(auto iter = gen3_pokemon_with_xy_mega_forms.begin();
             iter != gen3_pokemon_with_xy_mega_forms.end();
             ++iter)
    {
        if(generation >= 6)
        {
            (void)pkmn::pokemon::make(
                      *iter,
                      game,
                      "Mega",
                      100
                  );
        }
        else
        {
            EXPECT_THROW(
                (void)pkmn::pokemon::make(
                          *iter,
                          game,
                          "Mega",
                          100
                      );
            , std::invalid_argument);
        }
    }
    for(auto iter = gen3_pokemon_with_oras_mega_forms.begin();
             iter != gen3_pokemon_with_oras_mega_forms.end();
             ++iter)
    {
        if(is_oras)
        {
            (void)pkmn::pokemon::make(
                      *iter,
                      game,
                      "Mega",
                      100
                  );
        } else {
            EXPECT_THROW(
                (void)pkmn::pokemon::make(
                          *iter,
                          game,
                          "Mega",
                          100
                      );
            , std::invalid_argument);
        }
    }

    // Castform should always work.
    std::vector<std::string> castform_forms = pkmn::database::pokemon_entry(
                                                  pkmn::e_species::CASTFORM,
                                                  pkmn::e_game::OMEGA_RUBY,
                                                  ""
                                              ).get_forms();
    for(auto iter = castform_forms.begin(); iter != castform_forms.end(); ++iter)
    {
        pkmn::pokemon::sptr castform = pkmn::pokemon::make(
                                           pkmn::e_species::CASTFORM,
                                           game,
                                           *iter,
                                           30
                                       );
        EXPECT_TRUE(fs::exists(castform->get_icon_filepath()));
        EXPECT_TRUE(fs::exists(castform->get_sprite_filepath()));
    }

    // Primal Reversion forms should only work in OR/AS.
    pkmn::pokemon::sptr groudon = pkmn::pokemon::make(
                                      pkmn::e_species::GROUDON,
                                      game,
                                      "",
                                      70
                                  );
    EXPECT_TRUE(fs::exists(groudon->get_icon_filepath()));
    EXPECT_TRUE(fs::exists(groudon->get_sprite_filepath()));

    pkmn::pokemon::sptr kyogre = pkmn::pokemon::make(
                                     pkmn::e_species::KYOGRE,
                                     game,
                                     "",
                                     70
                                 );
    EXPECT_TRUE(fs::exists(kyogre->get_icon_filepath()));
    EXPECT_TRUE(fs::exists(kyogre->get_sprite_filepath()));

    if(is_oras)
    {
        (void)pkmn::pokemon::make(
                  pkmn::e_species::GROUDON,
                  game,
                  "Primal Reversion",
                  70
              );
        (void)pkmn::pokemon::make(
                  pkmn::e_species::KYOGRE,
                  game,
                  "Primal Reversion",
                  70
              );
    }
    else
    {
        EXPECT_THROW(
            (void)pkmn::pokemon::make(
                      pkmn::e_species::GROUDON,
                      game,
                      "Primal Reversion",
                      70
                  );
        , std::invalid_argument);
        EXPECT_THROW(
            (void)pkmn::pokemon::make(
                      pkmn::e_species::KYOGRE,
                      game,
                      "Primal Reversion",
                      70
                  );
        , std::invalid_argument);
    }

    // In Generation III, Deoxys's form is game-specific.
    pkmn::pokemon::sptr deoxys;
    if(generation == 3)
    {
        if((game == pkmn::e_game::RUBY) || (game == pkmn::e_game::SAPPHIRE) ||
           (game == pkmn::e_game::COLOSSEUM) || (game == pkmn::e_game::XD))
        {
            deoxys = pkmn::pokemon::make(
                         pkmn::e_species::DEOXYS,
                         game,
                         "Normal",
                         70
                     );
            EXPECT_TRUE(fs::exists(deoxys->get_icon_filepath()));
            EXPECT_TRUE(fs::exists(deoxys->get_sprite_filepath()));
        }
        else
        {
            EXPECT_THROW(
                (void)pkmn::pokemon::make(
                          pkmn::e_species::DEOXYS,
                          game,
                          "Normal",
                          70
                      );
            , std::invalid_argument);
        }

        if(game == pkmn::e_game::FIRERED)
        {
            deoxys = pkmn::pokemon::make(
                         pkmn::e_species::DEOXYS,
                         game,
                         "Attack",
                         70
                     );
            EXPECT_TRUE(fs::exists(deoxys->get_icon_filepath()));
            EXPECT_TRUE(fs::exists(deoxys->get_sprite_filepath()));
        }
        else
        {
            EXPECT_THROW(
                (void)pkmn::pokemon::make(
                          pkmn::e_species::DEOXYS,
                          game,
                          "Attack",
                          70
                      );
            , std::invalid_argument);
        }

        if(game == pkmn::e_game::LEAFGREEN)
        {
            deoxys = pkmn::pokemon::make(
                         pkmn::e_species::DEOXYS,
                         game,
                         "Defense",
                         70
                     );
            EXPECT_TRUE(fs::exists(deoxys->get_icon_filepath()));
            EXPECT_TRUE(fs::exists(deoxys->get_sprite_filepath()));
        }
        else
        {
            EXPECT_THROW(
                (void)pkmn::pokemon::make(
                          pkmn::e_species::DEOXYS,
                          game,
                          "Defense",
                          70
                      );
            , std::invalid_argument);
        }

        if(game == pkmn::e_game::EMERALD)
        {
            deoxys = pkmn::pokemon::make(
                         pkmn::e_species::DEOXYS,
                         game,
                         "Speed",
                         70
                     );
            EXPECT_TRUE(fs::exists(deoxys->get_icon_filepath()));
            EXPECT_TRUE(fs::exists(deoxys->get_sprite_filepath()));
        }
        else
        {
            EXPECT_THROW(
                (void)pkmn::pokemon::make(
                          pkmn::e_species::DEOXYS,
                          game,
                          "Speed",
                          70
                      );
            , std::invalid_argument);
        }
    }
    else
    {
        // Past Generation III, Deoxys's form can be switched.
        std::vector<std::string> deoxys_forms = pkmn::database::pokemon_entry(
                                                    pkmn::e_species::DEOXYS,
                                                    pkmn::e_game::OMEGA_RUBY,
                                                    ""
                                                ).get_forms();
        for(auto iter = deoxys_forms.begin(); iter != deoxys_forms.end(); ++iter)
        {
            deoxys = pkmn::pokemon::make(
                         pkmn::e_species::DEOXYS,
                         game,
                         *iter,
                         30
                     );
            EXPECT_TRUE(fs::exists(deoxys->get_icon_filepath()));
            EXPECT_TRUE(fs::exists(deoxys->get_sprite_filepath()));
        }
    }
}

INSTANTIATE_TEST_CASE_P(
    cpp_gen1_form_test,
    gen1_form_test,
    ::testing::ValuesIn(gen1_test_games)
);

INSTANTIATE_TEST_CASE_P(
    cpp_gen2_form_test,
    gen2_form_test,
    ::testing::ValuesIn(gen2_test_games)
);

INSTANTIATE_TEST_CASE_P(
    cpp_gen3_form_test,
    gen3_form_test,
    ::testing::ValuesIn(gen3_test_games)
);

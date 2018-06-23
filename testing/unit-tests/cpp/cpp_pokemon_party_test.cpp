/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "private_exports.hpp"

#include <pkmntest/config.hpp>
#include <pkmntest/util.hpp>

#include <pkmn/exception.hpp>
#include <pkmn/pokemon_party.hpp>

#include <pksav/gen1.h>
#include <pksav/gen2.h>
#include <pksav/gba.h>

#include "pksav/pksav_call.hpp"

#include "libpkmgc_includes.hpp"

#include <gtest/gtest.h>

#include <cstring>
#include <string>

// From pokemon_party_gcnimpl.hpp
typedef struct {
    LibPkmGC::GC::Pokemon* pokemon[6];
} gcn_pokemon_party_t;

struct test_params_t
{
    pkmn::e_game party_game;
    std::vector<pkmn::e_game> valid_other_games;
    pkmn::e_game invalid_other_game;
};

class pokemon_party_test: public ::testing::TestWithParam<test_params_t> {
    public:
        inline pkmn::pokemon_party::sptr get_party() {
            return _party;
        }

        inline pkmn::e_game get_game() {
            return test_params.party_game;
        }

        inline void reset_party() {
            _party = pkmn::pokemon_party::make(test_params.party_game);

            ASSERT_NE(nullptr, _party.get());
            ASSERT_EQ(test_params.party_game, _party->get_game());
            ASSERT_EQ(0, _party->get_num_pokemon());
            ASSERT_EQ(6, _party->as_vector().size());
        }

        test_params_t test_params;

    protected:
        void SetUp() {
            test_params = GetParam();
            reset_party();
        }

    private:

        pkmn::pokemon_party::sptr _party;
};

TEST_P(pokemon_party_test, empty_party_test) {
    pkmn::pokemon_party::sptr party = get_party();
    const pkmn::pokemon_list_t& list = party->as_vector();

    for(auto party_iter = list.begin(); party_iter != list.end(); ++party_iter) {
        ASSERT_NE(nullptr, party_iter->get());
        EXPECT_EQ("None", (*party_iter)->get_species());
        EXPECT_EQ(get_game(), (*party_iter)->get_game());

        const pkmn::move_slots_t& move_slots = (*party_iter)->get_moves();
        ASSERT_EQ(4, move_slots.size());
        for(auto move_iter = move_slots.begin(); move_iter != move_slots.end(); ++move_iter) {
            EXPECT_EQ("None", move_iter->move);
            EXPECT_EQ(0, move_iter->pp);
        }
    }
}

TEST_P(pokemon_party_test, setting_pokemon_test) {
    pkmn::pokemon_party::sptr party = get_party();
    ASSERT_NE(nullptr, party.get());

    pkmn::e_game game = party->get_game();
    int generation = pkmn::priv::game_enum_to_generation(game);

    pkmn::pokemon::sptr original_first = party->get_pokemon(0);
    pkmn::pokemon::sptr original_second = party->get_pokemon(1);

    /*
     * Create new Pokémon and place in party. The original variables should
     * have the same underlying Pokémon.
     */
    pkmn::pokemon::sptr bulbasaur = pkmn::pokemon::make(
                                        "Bulbasaur",
                                        game,
                                        "",
                                        5
                                    );
    pkmn::pokemon::sptr charmander = pkmn::pokemon::make(
                                         "Charmander",
                                         game,
                                         "",
                                         5
                                     );
    pkmn::pokemon::sptr squirtle = pkmn::pokemon::make(
                                       "Squirtle",
                                       game,
                                       "",
                                       5
                                   );

    // Make sure we can't set them at invalid indices.
    EXPECT_THROW(
        party->set_pokemon(-1, bulbasaur);
    , std::out_of_range);
    EXPECT_THROW(
        party->set_pokemon(6, bulbasaur);
    , std::out_of_range);

    party->set_pokemon(0, bulbasaur);
    EXPECT_EQ(1, party->get_num_pokemon());
    EXPECT_EQ("Bulbasaur", party->get_pokemon(0)->get_species());
    party->set_pokemon(1, charmander);
    EXPECT_EQ(2, party->get_num_pokemon());
    EXPECT_EQ("Charmander", party->get_pokemon(1)->get_species());

    // Replace one of the new ones.
    party->set_pokemon(0, squirtle);
    EXPECT_EQ(2, party->get_num_pokemon());
    EXPECT_EQ("Squirtle", party->get_pokemon(0)->get_species());

    // Make sure we can't copy a Pokémon to itself.
    EXPECT_THROW(
        party->set_pokemon(1, party->get_pokemon(1));
    , std::invalid_argument);
    EXPECT_EQ(2, party->get_num_pokemon());

    // Copy a Pokémon whose memory is already part of the party.
    party->set_pokemon(2, party->get_pokemon(1));
    EXPECT_EQ(3, party->get_num_pokemon());

    // We should always be able to clear the last contiguous Pokémon.
    party->set_pokemon(2, original_first);
    EXPECT_EQ(2, party->get_num_pokemon());
    EXPECT_EQ("None", party->get_pokemon(2)->get_species());

    // Put it back.
    party->set_pokemon(2, party->get_pokemon(1));
    EXPECT_EQ(3, party->get_num_pokemon());

    // Check that Pokémon cannot be placed non-contiguously.
    EXPECT_THROW(
        party->set_pokemon(1, original_first);
    , std::invalid_argument);
    EXPECT_EQ(3, party->get_num_pokemon());
    EXPECT_EQ("Charmander", party->get_pokemon(1)->get_species());

    EXPECT_THROW(
        party->set_pokemon(4, bulbasaur);
    , std::out_of_range);
    EXPECT_EQ(3, party->get_num_pokemon());
    EXPECT_EQ("None", party->get_pokemon(4)->get_species());

    /*
     * Now check everything we've created. Each variable should have the
     * same Pokémon underneath, even if the pointer has changed.
     */
    EXPECT_EQ("Squirtle", party->get_pokemon(0)->get_species());
    EXPECT_EQ("Charmander", party->get_pokemon(1)->get_species());
    EXPECT_EQ("Charmander", party->get_pokemon(2)->get_species());
    EXPECT_EQ("None", original_first->get_species());
    EXPECT_EQ("None", original_second->get_species());
    EXPECT_EQ("Bulbasaur", bulbasaur->get_species());
    EXPECT_EQ("Charmander", charmander->get_species());
    EXPECT_EQ("Squirtle", squirtle->get_species());

    // On the C++ level, make sure the expected equal pointers are equal.
    EXPECT_NE(original_first->get_native_pc_data(), party->get_pokemon(0)->get_native_pc_data());
    EXPECT_NE(original_second->get_native_pc_data(), party->get_pokemon(1)->get_native_pc_data());

    // On the C++ level, check the underlying representation.
    switch(generation) {
        case 1: {
            struct pksav_gen1_pokemon_party* native = reinterpret_cast<pksav_gen1_pokemon_party*>(party->get_native());
            EXPECT_EQ(3, native->count);
            EXPECT_EQ(squirtle->get_database_entry().get_pokemon_index(), native->species[0]);
            EXPECT_EQ(charmander->get_database_entry().get_pokemon_index(), native->species[1]);
            EXPECT_EQ(charmander->get_database_entry().get_pokemon_index(), native->species[2]);
            EXPECT_EQ(0, native->species[3]);
            EXPECT_EQ(0, native->species[4]);
            EXPECT_EQ(0, native->species[5]);
            for(int i = 0; i < 3; ++i) {
                EXPECT_EQ(party->get_pokemon(i)->get_native_pc_data(), &native->party[i]);
                EXPECT_EQ(party->get_pokemon(i)->get_database_entry().get_pokemon_index(), int(native->species[i]));
                EXPECT_EQ(party->get_pokemon(i)->get_database_entry().get_pokemon_index(), int(native->party[i].pc_data.species));

                char nickname[11] = {};
                PKSAV_CALL(
                    pksav_gen1_import_text(
                        native->nicknames[i],
                        nickname,
                        10
                    );
                )
                EXPECT_TRUE(std::strlen(nickname) > 0);
                EXPECT_EQ(
                    party->get_pokemon(i)->get_nickname(),
                    nickname
                );

                char otname[11] = {};
                PKSAV_CALL(
                    pksav_gen1_import_text(
                        native->otnames[i],
                        otname,
                        10
                    );
                )
                EXPECT_TRUE(std::strlen(otname) > 0);
                EXPECT_EQ(
                    party->get_pokemon(i)->get_original_trainer_name(),
                    otname
                );
            }
            break;
        }

        case 2: {
            struct pksav_gen2_pokemon_party* native = reinterpret_cast<pksav_gen2_pokemon_party*>(party->get_native());
            EXPECT_EQ(3, native->count);
            EXPECT_EQ(squirtle->get_database_entry().get_pokemon_index(), native->species[0]);
            EXPECT_EQ(charmander->get_database_entry().get_pokemon_index(), native->species[1]);
            EXPECT_EQ(charmander->get_database_entry().get_pokemon_index(), native->species[2]);
            EXPECT_EQ(0, native->species[3]);
            EXPECT_EQ(0, native->species[4]);
            EXPECT_EQ(0, native->species[5]);
            for(int i = 0; i < 3; ++i) {
                EXPECT_EQ(party->get_pokemon(i)->get_native_pc_data(), &native->party[i]);
                EXPECT_EQ(party->get_pokemon(i)->get_database_entry().get_pokemon_index(), int(native->species[i]));
                EXPECT_EQ(party->get_pokemon(i)->get_database_entry().get_pokemon_index(), int(native->party[i].pc_data.species));

                char nickname[11] = {};
                PKSAV_CALL(
                    pksav_gen2_import_text(
                        native->nicknames[i],
                        nickname,
                        10
                    );
                )
                EXPECT_TRUE(std::strlen(nickname) > 0);
                EXPECT_EQ(
                    party->get_pokemon(i)->get_nickname(),
                    nickname
                );

                char otname[11] = {};
                PKSAV_CALL(
                    pksav_gen2_import_text(
                        native->otnames[i],
                        otname,
                        10
                    );
                )
                EXPECT_TRUE(std::strlen(otname) > 0);
                EXPECT_EQ(
                    party->get_pokemon(i)->get_original_trainer_name(),
                    otname
                );
            }
            break;
        }

        case 3:
            if((game == pkmn::e_game::COLOSSEUM) || (game == pkmn::e_game::XD))
            {
                gcn_pokemon_party_t* native = reinterpret_cast<gcn_pokemon_party_t*>(party->get_native());
                EXPECT_EQ(squirtle->get_database_entry().get_pokemon_index(), int(native->pokemon[0]->species));
                EXPECT_EQ(charmander->get_database_entry().get_pokemon_index(), int(native->pokemon[1]->species));
                EXPECT_EQ(charmander->get_database_entry().get_pokemon_index(), int(native->pokemon[2]->species));
                EXPECT_EQ(0, int(native->pokemon[3]->species));
                EXPECT_EQ(0, int(native->pokemon[4]->species));
                EXPECT_EQ(0, int(native->pokemon[5]->species));
                for(int i = 0; i < 6; ++i)
                {
                    EXPECT_EQ(party->get_pokemon(i)->get_native_pc_data(), native->pokemon[i]);
                }
            }
            else
            {
                struct pksav_gba_pokemon_party* native = reinterpret_cast<pksav_gba_pokemon_party*>(party->get_native());
                EXPECT_EQ(3, native->count);
                EXPECT_EQ(squirtle->get_database_entry().get_pokemon_index(), native->party[0].pc_data.blocks.growth.species);
                EXPECT_EQ(charmander->get_database_entry().get_pokemon_index(), native->party[1].pc_data.blocks.growth.species);
                EXPECT_EQ(charmander->get_database_entry().get_pokemon_index(), native->party[2].pc_data.blocks.growth.species);
                EXPECT_EQ(0, native->party[3].pc_data.blocks.growth.species);
                EXPECT_EQ(0, native->party[4].pc_data.blocks.growth.species);
                EXPECT_EQ(0, native->party[5].pc_data.blocks.growth.species);
                for(int i = 0; i < 6; ++i)
                {
                    EXPECT_EQ(party->get_pokemon(i)->get_native_pc_data(), &native->party[i]);
                }
            }

        default:
            break;
    }

    // Make sure converting Pokémon before putting into the party works (or doesn't work) as expected.

    for(pkmn::e_game valid_game: test_params.valid_other_games)
    {
        pkmn::pokemon::sptr pikachu = pkmn::pokemon::make(
                                          "Pikachu",
                                          valid_game,
                                          "",
                                          50
                                      );
        party->set_pokemon(3, pikachu);

        pkmn::pokemon::sptr party_pokemon = party->get_pokemon(3);
        EXPECT_EQ("Pikachu", party_pokemon->get_species());
        EXPECT_EQ(test_params.party_game, party_pokemon->get_game());
        EXPECT_EQ(50, party_pokemon->get_level());
    }

    pkmn::pokemon::sptr invalid_pikachu = pkmn::pokemon::make(
                                              "Pikachu",
                                              test_params.invalid_other_game,
                                              "",
                                              50
                                          );
    EXPECT_THROW(
        party->set_pokemon(3, invalid_pikachu);
    , std::invalid_argument);
}

static const test_params_t PARAMS[] =
{
    {
        pkmn::e_game::RED,
        {
            pkmn::e_game::BLUE,
            pkmn::e_game::YELLOW,
            pkmn::e_game::GOLD,
            pkmn::e_game::SILVER,
            pkmn::e_game::CRYSTAL
        },
        pkmn::e_game::RUBY
    },
    {
        pkmn::e_game::BLUE,
        {
            pkmn::e_game::RED,
            pkmn::e_game::YELLOW,
            pkmn::e_game::GOLD,
            pkmn::e_game::SILVER,
            pkmn::e_game::CRYSTAL
        },
        pkmn::e_game::SAPPHIRE
    },
    {
        pkmn::e_game::YELLOW,
        {
            pkmn::e_game::RED,
            pkmn::e_game::BLUE,
            pkmn::e_game::GOLD,
            pkmn::e_game::SILVER,
            pkmn::e_game::CRYSTAL
        },
        pkmn::e_game::EMERALD
    },
    {
        pkmn::e_game::GOLD,
        {
            pkmn::e_game::RED,
            pkmn::e_game::BLUE,
            pkmn::e_game::YELLOW,
            pkmn::e_game::SILVER,
            pkmn::e_game::CRYSTAL
        },
        pkmn::e_game::FIRERED
    },
    {
        pkmn::e_game::SILVER,
        {
            pkmn::e_game::RED,
            pkmn::e_game::BLUE,
            pkmn::e_game::YELLOW,
            pkmn::e_game::GOLD,
            pkmn::e_game::CRYSTAL
        },
        pkmn::e_game::LEAFGREEN
    },
    {
        pkmn::e_game::CRYSTAL,
        {
            pkmn::e_game::RED,
            pkmn::e_game::BLUE,
            pkmn::e_game::YELLOW,
            pkmn::e_game::GOLD,
            pkmn::e_game::SILVER
        },
        pkmn::e_game::COLOSSEUM
    },
    {
        pkmn::e_game::RUBY,
        {
            pkmn::e_game::SAPPHIRE,
            pkmn::e_game::EMERALD,
            pkmn::e_game::FIRERED,
            pkmn::e_game::LEAFGREEN,
            pkmn::e_game::COLOSSEUM,
            pkmn::e_game::XD,
        },
        pkmn::e_game::RED
    },
    {
        pkmn::e_game::SAPPHIRE,
        {
            pkmn::e_game::RUBY,
            pkmn::e_game::EMERALD,
            pkmn::e_game::FIRERED,
            pkmn::e_game::LEAFGREEN,
            pkmn::e_game::COLOSSEUM,
            pkmn::e_game::XD,
        },
        pkmn::e_game::BLUE
    },
    {
        pkmn::e_game::EMERALD,
        {
            pkmn::e_game::RUBY,
            pkmn::e_game::SAPPHIRE,
            pkmn::e_game::FIRERED,
            pkmn::e_game::LEAFGREEN,
            pkmn::e_game::COLOSSEUM,
            pkmn::e_game::XD,
        },
        pkmn::e_game::YELLOW
    },
    {
        pkmn::e_game::FIRERED,
        {
            pkmn::e_game::RUBY,
            pkmn::e_game::SAPPHIRE,
            pkmn::e_game::EMERALD,
            pkmn::e_game::LEAFGREEN,
            pkmn::e_game::COLOSSEUM,
            pkmn::e_game::XD,
        },
        pkmn::e_game::GOLD
    },
    {
        pkmn::e_game::LEAFGREEN,
        {
            pkmn::e_game::RUBY,
            pkmn::e_game::SAPPHIRE,
            pkmn::e_game::EMERALD,
            pkmn::e_game::FIRERED,
            pkmn::e_game::COLOSSEUM,
            pkmn::e_game::XD,
        },
        pkmn::e_game::SILVER
    },
    {
        pkmn::e_game::COLOSSEUM,
        {
            pkmn::e_game::RUBY,
            pkmn::e_game::SAPPHIRE,
            pkmn::e_game::EMERALD,
            pkmn::e_game::FIRERED,
            pkmn::e_game::LEAFGREEN,
            pkmn::e_game::XD,
        },
        pkmn::e_game::CRYSTAL
    },
    {
        pkmn::e_game::XD,
        {
            pkmn::e_game::RUBY,
            pkmn::e_game::SAPPHIRE,
            pkmn::e_game::EMERALD,
            pkmn::e_game::FIRERED,
            pkmn::e_game::LEAFGREEN,
            pkmn::e_game::COLOSSEUM,
        },
        pkmn::e_game::RED
    },
};

INSTANTIATE_TEST_CASE_P(
    cpp_pokemon_party_test,
    pokemon_party_test,
    ::testing::ValuesIn(PARAMS)
);

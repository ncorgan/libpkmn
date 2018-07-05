/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/config.hpp>
#include <pkmntest/util.hpp>

#include <pkmn/pokemon_box.hpp>
#include <pkmn/pokemon_pc.hpp>

#include "private_exports.hpp"

#include "pksav/pksav_call.hpp"

#include "libpkmgc_includes.hpp"

#include <pksav/gen1.h>
#include <pksav/gen2.h>
#include <pksav/gba.h>

#include <pkmn/exception.hpp>

#include <gtest/gtest.h>

struct test_params_t
{
    pkmn::e_game box_game;
    std::vector<pkmn::e_game> valid_other_games;
    pkmn::e_game invalid_other_game;
};

class pokemon_box_test: public ::testing::TestWithParam<test_params_t>
{
    public:
        inline const pkmn::pokemon_box::sptr& get_pokemon_box()
        {
            return _pokemon_box;
        }

        inline pkmn::e_game get_game()
        {
            return test_params.box_game;
        }

        inline void reset()
        {
            _pokemon_box = pkmn::pokemon_box::make(test_params.box_game);

            ASSERT_NE(nullptr, _pokemon_box.get());
            ASSERT_EQ(test_params.box_game, _pokemon_box->get_game());
        }

        test_params_t test_params;

    protected:
        void SetUp()
        {
            test_params = GetParam();
            reset();
        }

    private:

        pkmn::pokemon_box::sptr _pokemon_box;
};

class pokemon_pc_test: public ::testing::TestWithParam<test_params_t>
{
    public:
        inline const pkmn::pokemon_pc::sptr& get_pokemon_pc()
        {
            return _pokemon_pc;
        }

        inline pkmn::e_game get_game()
        {
            return test_params.box_game;
        }

        inline void reset()
        {
            _pokemon_pc = pkmn::pokemon_pc::make(test_params.box_game);

            ASSERT_NE(nullptr, _pokemon_pc.get());
            ASSERT_EQ(test_params.box_game, _pokemon_pc->get_game());
        }

        test_params_t test_params;

    protected:
        void SetUp()
        {
            test_params = GetParam();
            reset();
        }

    private:

        pkmn::pokemon_pc::sptr _pokemon_pc;
};

void pokemon_box_test_common(
    pkmn::pokemon_box::sptr box,
    const test_params_t& test_params
)
{
    pkmn::e_game game = box->get_game();
    int generation = pkmn::priv::game_enum_to_generation(game);

    if(generation >= 2)
    {
        EXPECT_THROW(
            box->set_name("ABCDEFGHI");
        , std::invalid_argument);

        box->set_name("ABCDEFGH");
        EXPECT_EQ("ABCDEFGH", box->get_name());
    }
    else
    {
        EXPECT_THROW(
            box->get_name();
        , pkmn::feature_not_in_game_error);
        EXPECT_THROW(
            box->set_name("ABCDEFGH");
        , pkmn::feature_not_in_game_error);
    }

    pkmn::pokemon::sptr original_first = box->get_pokemon(0);
    pkmn::pokemon::sptr original_second = box->get_pokemon(1);

    /*
     * Create new Pokémon and place in box. The original variables should
     * have the same underlying Pokémon.
     */
    pkmn::pokemon::sptr bulbasaur = pkmn::pokemon::make(
                                        pkmn::e_species::BULBASAUR,
                                        game,
                                        "",
                                        5
                                    );
    pkmn::pokemon::sptr charmander = pkmn::pokemon::make(
                                         pkmn::e_species::CHARMANDER,
                                         game,
                                         "",
                                         5
                                     );
    pkmn::pokemon::sptr squirtle = pkmn::pokemon::make(
                                       pkmn::e_species::SQUIRTLE,
                                       game,
                                       "",
                                       5
                                   );

    // Make sure we can't set them at invalid indices.
    EXPECT_THROW(
        box->set_pokemon(-1, bulbasaur);
    , std::out_of_range);
    EXPECT_THROW(
        box->set_pokemon(box->get_capacity(), bulbasaur);
    , std::out_of_range);

    box->set_pokemon(0, bulbasaur);
    ASSERT_EQ(1, box->get_num_pokemon());
    box->set_pokemon(1, charmander);
    EXPECT_EQ(2, box->get_num_pokemon());

    // Replace one of the new ones.
    box->set_pokemon(0, squirtle);
    EXPECT_EQ(2, box->get_num_pokemon());

    // Make sure we can't copy a Pokémon to itself.
    EXPECT_THROW(
        box->set_pokemon(1, box->get_pokemon(1));
    , std::invalid_argument);
    EXPECT_EQ(2, box->get_num_pokemon());

    // Copy a Pokémon whose memory is already part of the box.
    box->set_pokemon(2, box->get_pokemon(1));
    EXPECT_EQ(3, box->get_num_pokemon());

    // We should always be able to clear the last contiguous Pokémon.
    box->set_pokemon(2, original_first);
    EXPECT_EQ(2, box->get_num_pokemon());
    EXPECT_EQ(pkmn::e_species::NONE, box->get_pokemon(2)->get_species());

    // Put it back.
    box->set_pokemon(2, box->get_pokemon(1));
    EXPECT_EQ(3, box->get_num_pokemon());

    // Check that Pokémon can be placed non-contiguously in the correct games.
    if(generation <= 2) {
        EXPECT_THROW(
            box->set_pokemon(1, original_first);
        , std::invalid_argument);
        EXPECT_EQ(3, box->get_num_pokemon());
        EXPECT_EQ(pkmn::e_species::CHARMANDER, box->get_pokemon(1)->get_species());

        EXPECT_THROW(
            box->set_pokemon(4, bulbasaur);
        , std::out_of_range);
        EXPECT_EQ(3, box->get_num_pokemon());
        EXPECT_EQ(pkmn::e_species::NONE, box->get_pokemon(4)->get_species());
    } else {
        box->set_pokemon(1, original_first);
        EXPECT_EQ(2, box->get_num_pokemon());
        EXPECT_EQ(pkmn::e_species::NONE, box->get_pokemon(1)->get_species());

        box->set_pokemon(4, bulbasaur);
        EXPECT_EQ(3, box->get_num_pokemon());
        EXPECT_EQ(pkmn::e_species::BULBASAUR, box->get_pokemon(4)->get_species());

        // Restore it to how it was.
        box->set_pokemon(1, charmander);
        box->set_pokemon(4, original_first);
        EXPECT_EQ(3, box->get_num_pokemon());
        EXPECT_EQ(pkmn::e_species::CHARMANDER, box->get_pokemon(1)->get_species());
        EXPECT_EQ(pkmn::e_species::NONE, box->get_pokemon(4)->get_species());
    }

    /*
     * Now check everything we've created. Each variable should have the
     * same Pokémon underneath, even if the pointer has changed.
     */
    EXPECT_EQ(pkmn::e_species::SQUIRTLE, box->get_pokemon(0)->get_species());
    EXPECT_EQ(pkmn::e_species::CHARMANDER, box->get_pokemon(1)->get_species());
    EXPECT_EQ(pkmn::e_species::CHARMANDER, box->get_pokemon(2)->get_species());
    EXPECT_EQ(pkmn::e_species::NONE, original_first->get_species());
    EXPECT_EQ(pkmn::e_species::NONE, original_second->get_species());
    EXPECT_EQ(pkmn::e_species::BULBASAUR, bulbasaur->get_species());
    EXPECT_EQ(pkmn::e_species::CHARMANDER, charmander->get_species());
    EXPECT_EQ(pkmn::e_species::SQUIRTLE, squirtle->get_species());

    // On the C++ level, make sure the expected equal pointers are equal.
    EXPECT_NE(original_first->get_native_pc_data(), box->get_pokemon(0)->get_native_pc_data());
    EXPECT_NE(original_second->get_native_pc_data(), box->get_pokemon(1)->get_native_pc_data());

    // On the C++ level, check the underlying representation.
    switch(generation) {
        case 1: {
            const struct pksav_gen1_pokemon_box* native_box = reinterpret_cast<const struct pksav_gen1_pokemon_box*>(box->get_native());
            const pkmn::pokemon_list_t& pokemon_list = box->as_vector();
            for(size_t i = 0; i < 3; ++i) {
                EXPECT_EQ(pokemon_list.at(i)->get_database_entry().get_pokemon_index(), int(native_box->species[i]));
                EXPECT_EQ(pokemon_list.at(i)->get_database_entry().get_pokemon_index(), int(native_box->entries[i].species));
                EXPECT_EQ(pokemon_list.at(i)->get_native_pc_data(), &native_box->entries[i]);

                char nickname[11] = {0};
                PKSAV_CALL(
                    pksav_gen2_import_text(
                        native_box->nicknames[i],
                        nickname,
                        10
                    );
                )
                EXPECT_EQ(std::string(nickname), pokemon_list.at(i)->get_nickname());

                char otname[8] = {0};
                PKSAV_CALL(
                    pksav_gen2_import_text(
                        native_box->otnames[i],
                        otname,
                        7
                    );
                )
                EXPECT_EQ(std::string(otname), pokemon_list.at(i)->get_original_trainer_name());
            }
            EXPECT_EQ(3, native_box->count);
            break;
        }

        case 2: {
            const struct pksav_gen2_pokemon_box* native_box = reinterpret_cast<const struct pksav_gen2_pokemon_box*>(box->get_native());
            const pkmn::pokemon_list_t& pokemon_list = box->as_vector();
            for(size_t i = 0; i < 3; ++i) {
                EXPECT_EQ(pokemon_list.at(i)->get_database_entry().get_pokemon_index(), int(native_box->species[i]));
                EXPECT_EQ(pokemon_list.at(i)->get_database_entry().get_pokemon_index(), int(native_box->entries[i].species));
                EXPECT_EQ(pokemon_list.at(i)->get_native_pc_data(), &native_box->entries[i]);

                char nickname[11] = {0};
                PKSAV_CALL(
                    pksav_gen2_import_text(
                        native_box->nicknames[i],
                        nickname,
                        10
                    );
                )
                EXPECT_EQ(std::string(nickname), pokemon_list.at(i)->get_nickname());

                char otname[8] = {0};
                PKSAV_CALL(
                    pksav_gen2_import_text(
                        native_box->otnames[i],
                        otname,
                        7
                    );
                )
                EXPECT_EQ(std::string(otname), pokemon_list.at(i)->get_original_trainer_name());
            }
            EXPECT_EQ(3, native_box->count);
            break;
        }

        case 3:
        {
            if((game == pkmn::e_game::COLOSSEUM) || (game == pkmn::e_game::XD))
            {
                const LibPkmGC::GC::PokemonBox* native_box = reinterpret_cast<const LibPkmGC::GC::PokemonBox*>(box->get_native());
                const pkmn::pokemon_list_t& pokemon_list = box->as_vector();
                for(size_t i = 0; i < pokemon_list.size(); ++i)
                {
                    EXPECT_EQ(pokemon_list.at(i)->get_native_pc_data(), native_box->pkm[i]);
                }

                EXPECT_EQ(box->get_name(), std::string(native_box->name->toUTF8()));
            }
            else
            {
                const struct pksav_gba_pokemon_box* native_box = reinterpret_cast<const struct pksav_gba_pokemon_box*>(box->get_native());
                const pkmn::pokemon_list_t& pokemon_list = box->as_vector();
                for(size_t i = 0; i < pokemon_list.size(); ++i)
                {
                    EXPECT_EQ(pokemon_list.at(i)->get_native_pc_data(), &native_box->entries[i]);
                }
            }
            break;
        }

        default:
            break;
    }

    // Make sure converting Pokémon before putting into the box works (or doesn't work) as expected.
    for(pkmn::e_game valid_game: test_params.valid_other_games)
    {
        pkmn::pokemon::sptr pikachu = pkmn::pokemon::make(
                                          pkmn::e_species::PIKACHU,
                                          valid_game,
                                          "",
                                          50
                                      );
        box->set_pokemon(3, pikachu);

        pkmn::pokemon::sptr box_pokemon = box->get_pokemon(3);
        EXPECT_EQ(pkmn::e_species::PIKACHU, box_pokemon->get_species());
        EXPECT_EQ(test_params.box_game, box_pokemon->get_game());
        EXPECT_EQ(50, box_pokemon->get_level());
    }

    pkmn::pokemon::sptr invalid_pikachu = pkmn::pokemon::make(
                                              pkmn::e_species::PIKACHU,
                                              test_params.invalid_other_game,
                                              "",
                                              50
                                          );
    EXPECT_THROW(
        box->set_pokemon(3, invalid_pikachu);
    , std::invalid_argument);
}

void pokemon_pc_test_common(
    pkmn::pokemon_pc::sptr pc,
    const test_params_t& test_params
)
{
    pkmn::e_game game = pc->get_game();
    int generation = pkmn::priv::game_enum_to_generation(pc->get_game());

    const pkmn::pokemon_box_list_t& box_list = pc->as_vector();
    ASSERT_EQ(box_list.size(), size_t(pc->get_num_boxes()));
    for(auto box_iter = box_list.begin(); box_iter != box_list.end(); ++box_iter) {
        ASSERT_EQ(game, (*box_iter)->get_game());
        pokemon_box_test_common(*box_iter, test_params);
    }

    if(generation >= 2) {
        for(int i = 0; i < pc->get_num_boxes(); ++i) {
            std::string box_name = str(boost::format("BOX%d") % (i+1));
            pc->get_box(i)->set_name(box_name);
        }

        const std::vector<std::string>& box_names = pc->get_box_names();
        for(int i = 0; i < pc->get_num_boxes(); ++i) {
            std::string box_name = str(boost::format("BOX%d") % (i+1));
            EXPECT_EQ(box_name, box_names[i]);
        }
    } else {
        EXPECT_THROW(
            pc->get_box_names();
        , pkmn::feature_not_in_game_error);
    }

    for(int i = 0; i < pc->get_num_boxes(); ++i) {
        EXPECT_EQ(pkmn::e_species::SQUIRTLE, pc->get_box(i)->get_pokemon(0)->get_species());
        EXPECT_EQ(pkmn::e_species::CHARMANDER, pc->get_box(i)->get_pokemon(1)->get_species());
        EXPECT_EQ(pkmn::e_species::CHARMANDER, pc->get_box(i)->get_pokemon(2)->get_species());
    }

    // On the C++ level, just check the values we've set to confirm the pointers worked.
    switch(generation)
    {
        case 1:
        {
            const struct pksav_gen1_pokemon_storage* p_native_storage =
                reinterpret_cast<const struct pksav_gen1_pokemon_storage*>(pc->get_native());

            uint8_t current_box_num = *p_native_storage->p_current_box_num;
            current_box_num &= PKSAV_GEN1_CURRENT_POKEMON_BOX_NUM_MASK;

            const pkmn::pokemon_box_list_t& pokemon_box_list = pc->as_vector();
            for(size_t box_index = 0;
                box_index < pokemon_box_list.size();
                ++box_index)
            {
                const pkmn::pokemon_list_t& pokemon_list = pokemon_box_list.at(box_index)->as_vector();

                const struct pksav_gen1_pokemon_box* p_box = nullptr;
                if(box_index == current_box_num)
                {
                    p_box = p_native_storage->p_current_box;
                }
                else
                {
                    p_box = p_native_storage->pp_boxes[box_index];
                }
                ASSERT_NE(nullptr, p_box);

                EXPECT_EQ(
                    pokemon_list.at(0)->get_database_entry().get_pokemon_index(),
                    int(p_box->species[0])
                );
                EXPECT_EQ(
                    pokemon_list.at(0)->get_database_entry().get_pokemon_index(),
                    int(p_box->entries[0].species)
                );
                EXPECT_EQ(
                    pokemon_list.at(1)->get_database_entry().get_pokemon_index(),
                    int(p_box->species[1])
                );
                EXPECT_EQ(
                    pokemon_list.at(1)->get_database_entry().get_pokemon_index(),
                    int(p_box->entries[1].species)
                );
                EXPECT_EQ(
                    pokemon_list.at(2)->get_database_entry().get_pokemon_index(),
                    int(p_box->species[2])
                );
                EXPECT_EQ(
                    pokemon_list.at(2)->get_database_entry().get_pokemon_index(),
                    int(p_box->entries[2].species)
                );
                EXPECT_EQ(
                    pokemon_box_list.at(box_index)->get_native(),
                    p_box
                );
            }
            break;
        }

        case 2:
        {
            const struct pksav_gen2_pokemon_storage* p_native_storage =
                reinterpret_cast<const struct pksav_gen2_pokemon_storage*>(pc->get_native());

            const pkmn::pokemon_box_list_t& pokemon_box_list = pc->as_vector();
            const std::vector<std::string>& box_names = pc->get_box_names();
            for(size_t box_index = 0;
                box_index < pokemon_box_list.size();
                ++box_index)
            {
                const pkmn::pokemon_list_t& pokemon_list = pokemon_box_list.at(box_index)->as_vector();

                const struct pksav_gen2_pokemon_box* p_box = nullptr;
                if(box_index == *p_native_storage->p_current_box_num)
                {
                    p_box = p_native_storage->p_current_box;
                }
                else
                {
                    p_box = p_native_storage->pp_boxes[box_index];
                }
                ASSERT_NE(nullptr, p_box);

                EXPECT_EQ(
                    pokemon_list.at(0)->get_database_entry().get_pokemon_index(),
                    int(p_box->species[0])
                );
                EXPECT_EQ(
                    pokemon_list.at(0)->get_database_entry().get_pokemon_index(),
                    int(p_box->entries[0].species)
                );
                EXPECT_EQ(
                    pokemon_list.at(1)->get_database_entry().get_pokemon_index(),
                    int(p_box->species[1])
                );
                EXPECT_EQ(
                    pokemon_list.at(1)->get_database_entry().get_pokemon_index(),
                    int(p_box->entries[1].species)
                );
                EXPECT_EQ(
                    pokemon_list.at(2)->get_database_entry().get_pokemon_index(),
                    int(p_box->species[2])
                );
                EXPECT_EQ(
                    pokemon_list.at(2)->get_database_entry().get_pokemon_index(),
                    int(p_box->entries[2].species)
                );
                EXPECT_EQ(
                    pokemon_box_list.at(box_index)->get_native(),
                    p_box
                );

                char box_name[PKSAV_GEN2_POKEMON_BOX_NAME_LENGTH + 1] = {0};
                PKSAV_CALL(
                    pksav_gen2_import_text(
                        p_native_storage->p_box_names->names[box_index],
                        box_name,
                        PKSAV_GEN2_POKEMON_BOX_NAME_LENGTH
                    );
                )
                EXPECT_EQ(std::string(box_name), box_names.at(box_index));
            }
            break;
        }

        case 3: {
            if((game == pkmn::e_game::COLOSSEUM) || (game == pkmn::e_game::XD))
            {
                const LibPkmGC::GC::PokemonBox** native_boxes = reinterpret_cast<const LibPkmGC::GC::PokemonBox**>(pc->get_native());
                const pkmn::pokemon_box_list_t& pokemon_box_list = pc->as_vector();
                const std::vector<std::string>& box_names = pc->get_box_names();
                for(size_t i = 0; i < pokemon_box_list.size(); ++i) {
                    const pkmn::pokemon_list_t& pokemon_list = pokemon_box_list.at(i)->as_vector();
                    EXPECT_EQ(pokemon_list.at(0)->get_database_entry().get_pokemon_index(), int(native_boxes[i]->pkm[0]->species));
                    EXPECT_EQ(pokemon_list.at(1)->get_database_entry().get_pokemon_index(), int(native_boxes[i]->pkm[1]->species));
                    EXPECT_EQ(pokemon_list.at(0)->get_database_entry().get_pokemon_index(), int(native_boxes[i]->pkm[0]->species));
                    EXPECT_EQ(pokemon_list.at(2)->get_database_entry().get_pokemon_index(), int(native_boxes[i]->pkm[2]->species));
                    EXPECT_EQ(pokemon_box_list.at(i)->get_native(), native_boxes[i]);
                    EXPECT_EQ(std::string(native_boxes[i]->name->toUTF8()), box_names.at(i));
                }
            }
            else
            {
                const struct pksav_gba_pokemon_pc* native_pc = reinterpret_cast<const struct pksav_gba_pokemon_pc*>(pc->get_native());
                const pkmn::pokemon_box_list_t& pokemon_box_list = pc->as_vector();
                const std::vector<std::string>& box_names = pc->get_box_names();
                for(size_t i = 0; i < pokemon_box_list.size(); ++i) {
                    const pkmn::pokemon_list_t& pokemon_list = pokemon_box_list.at(i)->as_vector();
                    EXPECT_EQ(pokemon_list.at(0)->get_database_entry().get_pokemon_index(), int(native_pc->boxes[i].entries[0].blocks.growth.species));
                    EXPECT_EQ(pokemon_list.at(1)->get_database_entry().get_pokemon_index(), int(native_pc->boxes[i].entries[1].blocks.growth.species));
                    EXPECT_EQ(pokemon_list.at(0)->get_database_entry().get_pokemon_index(), int(native_pc->boxes[i].entries[0].blocks.growth.species));
                    EXPECT_EQ(pokemon_list.at(2)->get_database_entry().get_pokemon_index(), int(native_pc->boxes[i].entries[2].blocks.growth.species));
                    EXPECT_EQ(pokemon_box_list.at(i)->get_native(), &native_pc->boxes[i]);

                    char box_name[10] = {0};
                    PKSAV_CALL(
                        pksav_gba_import_text(
                            native_pc->box_names[i],
                            box_name,
                            9
                        );
                    )
                    EXPECT_EQ(std::string(box_name), box_names.at(i));
                }
            }
            break;
        }

        default:
            break;
    }
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

namespace pkmntest
{

TEST_P(pokemon_box_test, pokemon_box_test)
{
    pokemon_box_test_common(get_pokemon_box(), test_params);
}

INSTANTIATE_TEST_CASE_P(
    cpp_pokemon_box_test,
    pokemon_box_test,
    ::testing::ValuesIn(PARAMS)
);

TEST_P(pokemon_pc_test, pokemon_pc_test)
{
    pokemon_pc_test_common(get_pokemon_pc(), test_params);
}

INSTANTIATE_TEST_CASE_P(
    cpp_pokemon_pc_test,
    pokemon_pc_test,
    ::testing::ValuesIn(PARAMS)
);

}

/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/pokemon_pc_test.hpp> 

#include "pksav/pksav_call.hpp"

#include <pksav/gen1.h>
#include <pksav/gen2.h>
#include <pksav/gba.h>

#include <pkmn/exception.hpp>

#include <boost/assign.hpp>
#include <boost/format.hpp>

// No database access here
static const std::map<std::string, int> game_generations = boost::assign::map_list_of
    ("Red", 1)
    ("Blue", 1)
    ("Yellow", 1)
    ("Gold", 2)
    ("Silver", 2)
    ("Crystal", 2)
    ("Ruby", 3)
    ("Sapphire", 3)
    ("Emerald", 3)
    ("FireRed", 3)
    ("LeafGreen", 3)
    ("Colosseum", 3)
    ("XD", 3)
    ("Diamond", 4)
    ("Pearl", 4)
    ("Platinum", 4)
    ("HeartGold", 4)
    ("SoulSilver", 4)
    ("Black", 5)
    ("White", 5)
    ("Black 2", 5)
    ("White 2", 5)
    ("X", 6)
    ("Y", 6)
    ("Omega Ruby", 6)
    ("Alpha Sapphire", 6)
;

namespace pkmntest {

    void pokemon_box_test_common(
        pkmn::pokemon_box::sptr box
    ) {
        std::string game = box->get_game();
        int generation = game_generations.at(game);

        if(generation >= 2) {
            EXPECT_THROW(
                box->set_name("ABCDEFGHI");
            , std::invalid_argument);

            box->set_name("ABCDEFGH");
            EXPECT_EQ("ABCDEFGH", box->get_name());
        } else {
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
            box->set_pokemon(-1, bulbasaur);
        , pkmn::range_error);
        EXPECT_THROW(
            box->set_pokemon(box->get_capacity(), bulbasaur);
        , pkmn::range_error);

        box->set_pokemon(0, bulbasaur);
        EXPECT_EQ(1, box->get_num_pokemon());
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
        EXPECT_EQ("None", box->get_pokemon(2)->get_species());

        // Put it back.
        box->set_pokemon(2, box->get_pokemon(1));
        EXPECT_EQ(3, box->get_num_pokemon());

        // Check that Pokémon can be placed non-contiguously in the correct games.
        if(generation <= 2) {
            EXPECT_THROW(
                box->set_pokemon(1, original_first);
            , std::invalid_argument);
            EXPECT_EQ(3, box->get_num_pokemon());
            EXPECT_EQ("Charmander", box->get_pokemon(1)->get_species());

            EXPECT_THROW(
                box->set_pokemon(4, bulbasaur);
            , pkmn::range_error);
            EXPECT_EQ(3, box->get_num_pokemon());
            EXPECT_EQ("None", box->get_pokemon(4)->get_species());
        } else {
            box->set_pokemon(1, original_first);
            EXPECT_EQ(2, box->get_num_pokemon());
            EXPECT_EQ("None", box->get_pokemon(1)->get_species());

            box->set_pokemon(4, bulbasaur);
            EXPECT_EQ(3, box->get_num_pokemon());
            EXPECT_EQ("Bulbasaur", box->get_pokemon(4)->get_species());

            // Restore it to how it was.
            box->set_pokemon(1, charmander);
            box->set_pokemon(4, original_first);
            EXPECT_EQ(3, box->get_num_pokemon());
            EXPECT_EQ("Charmander", box->get_pokemon(1)->get_species());
            EXPECT_EQ("None", box->get_pokemon(4)->get_species());
        }

        /*
         * Now check everything we've created. Each variable should have the
         * same Pokémon underneath, even if the pointer has changed.
         */
        EXPECT_EQ("Squirtle", box->get_pokemon(0)->get_species());
        EXPECT_EQ("Charmander", box->get_pokemon(1)->get_species());
        EXPECT_EQ("Charmander", box->get_pokemon(2)->get_species());
        EXPECT_EQ("None", original_first->get_species());
        EXPECT_EQ("None", original_second->get_species());
        EXPECT_EQ("Bulbasaur", bulbasaur->get_species());
        EXPECT_EQ("Charmander", charmander->get_species());
        EXPECT_EQ("Squirtle", squirtle->get_species());

        // On the C++ level, make sure the expected equal pointers are equal.
        EXPECT_NE(original_first->get_native_pc_data(), box->get_pokemon(0)->get_native_pc_data());
        EXPECT_NE(original_second->get_native_pc_data(), box->get_pokemon(1)->get_native_pc_data());

        // On the C++ level, check the underlying PKSav struct.
        switch(generation) {
            case 1: {
                const pksav_gen1_pokemon_box_t* native_box = reinterpret_cast<const pksav_gen1_pokemon_box_t*>(box->get_native());
                const pkmn::pokemon_list_t& pokemon_list = box->as_vector();
                for(size_t i = 0; i < pokemon_list.size(); ++i) {
                    EXPECT_EQ(pokemon_list.at(i)->get_database_entry().get_pokemon_index(), int(native_box->species[i]));
                    EXPECT_EQ(pokemon_list.at(i)->get_database_entry().get_pokemon_index(), int(native_box->entries[i].species));
                    EXPECT_EQ(pokemon_list.at(i)->get_native_pc_data(), &native_box->entries[i]);
                }
                EXPECT_EQ(3, native_box->count);
                break;
            }

            case 2: {
                const pksav_gen2_pokemon_box_t* native_box = reinterpret_cast<const pksav_gen2_pokemon_box_t*>(box->get_native());
                const pkmn::pokemon_list_t& pokemon_list = box->as_vector();
                for(size_t i = 0; i < pokemon_list.size(); ++i) {
                    EXPECT_EQ(pokemon_list.at(i)->get_database_entry().get_pokemon_index(), int(native_box->species[i]));
                    EXPECT_EQ(pokemon_list.at(i)->get_database_entry().get_pokemon_index(), int(native_box->entries[i].species));
                    EXPECT_EQ(pokemon_list.at(i)->get_native_pc_data(), &native_box->entries[i]);

                    char nickname[11] = {0};
                    PKSAV_CALL(
                        pksav_text_from_gen2(
                            native_box->nicknames[i],
                            nickname,
                            10
                        );
                    )
                    EXPECT_EQ(std::string(nickname), pokemon_list.at(i)->get_nickname());

                    char otname[8] = {0};
                    PKSAV_CALL(
                        pksav_text_from_gen2(
                            native_box->otnames[i],
                            otname,
                            7
                        );
                    )
                    EXPECT_EQ(std::string(otname), pokemon_list.at(i)->get_trainer_name());
                }
                EXPECT_EQ(3, native_box->count);
                break;
            }

            case 3: {
                if(game == "Colosseum" or game == "XD") {
                } else {
                    const pksav_gba_pokemon_box_t* native_box = reinterpret_cast<const pksav_gba_pokemon_box_t*>(box->get_native());
                    const pkmn::pokemon_list_t& pokemon_list = box->as_vector();
                    for(size_t i = 0; i < pokemon_list.size(); ++i) {
                        EXPECT_EQ(pokemon_list.at(i)->get_native_pc_data(), &native_box->entries[i]);
                    }
                }
                break;
            }

            default:
                break;
        }
    }

    // See pokemon_pc_gen2impl.hpp
    typedef struct {
        pksav_gen2_pokemon_pc_t* pc;
        pksav_gen2_pokemon_box_names_t* box_names;
    } gen2_pokemon_full_pc_t;

    void pokemon_pc_test_common(
        pkmn::pokemon_pc::sptr pc
    ) {
        std::string game = pc->get_game();
        int generation = game_generations.at(pc->get_game());

        const pkmn::pokemon_box_list_t& box_list = pc->as_vector();
        ASSERT_EQ(box_list.size(), size_t(pc->get_num_boxes()));
        for(auto box_iter = box_list.begin(); box_iter != box_list.end(); ++box_iter) {
            ASSERT_EQ(game, (*box_iter)->get_game());
            pokemon_box_test_common(*box_iter);
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
            EXPECT_EQ("Squirtle", pc->get_box(i)->get_pokemon(0)->get_species());
            EXPECT_EQ("Charmander", pc->get_box(i)->get_pokemon(1)->get_species());
            EXPECT_EQ("Charmander", pc->get_box(i)->get_pokemon(2)->get_species());
        }

        // On the C++ level, just check the values we've set to confirm the pointers worked.
        switch(generation) {
            case 1: {
                const pksav_gen1_pokemon_box_t** native_boxes = reinterpret_cast<const pksav_gen1_pokemon_box_t**>(pc->get_native());
                const pkmn::pokemon_box_list_t& pokemon_box_list = pc->as_vector();
                for(size_t i = 0; i < pokemon_box_list.size(); ++i) {
                    const pkmn::pokemon_list_t& pokemon_list = pokemon_box_list.at(i)->as_vector();
                    EXPECT_EQ(pokemon_list.at(0)->get_database_entry().get_pokemon_index(), int(native_boxes[i]->species[0]));
                    EXPECT_EQ(pokemon_list.at(0)->get_database_entry().get_pokemon_index(), int(native_boxes[i]->entries[0].species));
                    EXPECT_EQ(pokemon_list.at(1)->get_database_entry().get_pokemon_index(), int(native_boxes[i]->species[1]));
                    EXPECT_EQ(pokemon_list.at(1)->get_database_entry().get_pokemon_index(), int(native_boxes[i]->entries[1].species));
                    EXPECT_EQ(pokemon_list.at(2)->get_database_entry().get_pokemon_index(), int(native_boxes[i]->species[2]));
                    EXPECT_EQ(pokemon_list.at(2)->get_database_entry().get_pokemon_index(), int(native_boxes[i]->entries[2].species));
                    EXPECT_EQ(pokemon_box_list.at(i)->get_native(), native_boxes[i]);
                }
                break;
            }

            case 2: {
                const gen2_pokemon_full_pc_t* native_pc = reinterpret_cast<const gen2_pokemon_full_pc_t*>(pc->get_native());
                const pkmn::pokemon_box_list_t& pokemon_box_list = pc->as_vector();
                const std::vector<std::string>& box_names = pc->get_box_names();
                for(size_t i = 0; i < pokemon_box_list.size(); ++i) {
                    const pkmn::pokemon_list_t& pokemon_list = pokemon_box_list.at(i)->as_vector();
                    EXPECT_EQ(pokemon_list.at(0)->get_database_entry().get_pokemon_index(), int(native_pc->pc->boxes[i].species[0]));
                    EXPECT_EQ(pokemon_list.at(0)->get_database_entry().get_pokemon_index(), int(native_pc->pc->boxes[i].entries[0].species));
                    EXPECT_EQ(pokemon_list.at(1)->get_database_entry().get_pokemon_index(), int(native_pc->pc->boxes[i].species[1]));
                    EXPECT_EQ(pokemon_list.at(1)->get_database_entry().get_pokemon_index(), int(native_pc->pc->boxes[i].entries[1].species));
                    EXPECT_EQ(pokemon_list.at(2)->get_database_entry().get_pokemon_index(), int(native_pc->pc->boxes[i].species[2]));
                    EXPECT_EQ(pokemon_list.at(2)->get_database_entry().get_pokemon_index(), int(native_pc->pc->boxes[i].entries[2].species));
                    EXPECT_EQ(pokemon_box_list.at(i)->get_native(), &native_pc->pc->boxes[i]);

                    char box_name[10] = {0};
                    PKSAV_CALL(
                        pksav_text_from_gen2(
                            native_pc->box_names->names[i],
                            box_name,
                            9
                        );
                    )
                    EXPECT_EQ(std::string(box_name), box_names.at(i));
                }
                break;
            }

            case 3: {
                if(game == "Colosseum" or game == "XD") {
                } else {
                    const pksav_gba_pokemon_pc_t* native_pc = reinterpret_cast<const pksav_gba_pokemon_pc_t*>(pc->get_native());
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
                            pksav_text_from_gba(
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

}

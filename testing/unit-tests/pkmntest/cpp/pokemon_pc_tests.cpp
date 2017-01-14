/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/pokemon_pc_tests.hpp> 

// Don't create the main in a library
#undef BOOST_TEST_MAIN

#include "pkmn_boost_unit_test.hpp"

#include <boost/assign.hpp>
#include <boost/format.hpp>

#include "pksav/pksav_call.hpp"

#include <pksav/gen1.h>
#include <pksav/gen2.h>
#include <pksav/gba.h>

#include <pkmn/exception.hpp>

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

    void test_empty_pokemon_box(
        pkmn::pokemon_box::sptr box,
        const std::string &game
    ) {
        BOOST_CHECK_EQUAL(box->get_game(), game);

        if(game == "Red" or game == "Blue" or game == "Yellow") {
            BOOST_CHECK_THROW(
                (void)box->get_name();
            , pkmn::feature_not_in_game_error);
        } else {
            BOOST_CHECK_EQUAL(
                box->get_name(),
                ""
            );
        }

        // Make sure trying to get a Pokémon out of range fails.
        BOOST_CHECK_THROW(
            (void)box->get_pokemon(-1);
        , pkmn::range_error);
        BOOST_CHECK_THROW(
            (void)box->get_pokemon(box->get_capacity());
        , pkmn::range_error);

        const pkmn::pokemon_list_t& pokemon_list = box->as_vector();
        BOOST_CHECK_EQUAL(box->get_capacity(), pokemon_list.size());

        for(int i = 0; i < box->get_capacity(); ++i) {
            pkmn::pokemon::sptr entry = pokemon_list.at(i);
            BOOST_CHECK_EQUAL(entry->get_species(), "None");
            BOOST_CHECK_EQUAL(entry->get_game(), game);

            const pkmn::move_slots_t& move_slots = entry->get_moves();
            BOOST_REQUIRE_EQUAL(move_slots.size(), 4);
            for(int j = 0; j < 4; ++j) {
                BOOST_CHECK_EQUAL(move_slots.at(j).move.get_name(), "None");
                BOOST_CHECK_EQUAL(move_slots.at(j).move.get_game(), game);
                BOOST_CHECK_EQUAL(move_slots.at(j).pp, 0);
            }
        }
    }

    void test_box_name(
        pkmn::pokemon_box::sptr box
    ) {
        std::string game = box->get_game();
        if(game == "Red" or game == "Blue" or game == "Yellow") {
            BOOST_CHECK_THROW(
                box->set_name("ABCDEFGH");
            , pkmn::feature_not_in_game_error);
            BOOST_CHECK_THROW(
                (void)box->get_name();
            , pkmn::feature_not_in_game_error);
        } else {
            BOOST_CHECK_THROW(
                box->set_name("ABCDEFGHI");
            , std::invalid_argument);

            box->set_name("ABCDEFGH");
            BOOST_CHECK_EQUAL(
                box->get_name(),
                "ABCDEFGH"
            );
        }
    }

    void test_setting_pokemon(
        pkmn::pokemon_box::sptr box
    ) {
        std::string game = box->get_game();

        pkmn::pokemon::sptr original_first = box->get_pokemon(0);
        pkmn::pokemon::sptr original_second = box->get_pokemon(1);

        // Make sure we can't move these
        BOOST_CHECK_THROW(
            box->set_pokemon(2, original_first);
        , std::runtime_error);
        BOOST_CHECK_THROW(
            box->set_pokemon(3, original_second);
        , std::runtime_error);

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
        BOOST_CHECK_THROW(
            box->set_pokemon(-1, bulbasaur);
        , pkmn::range_error);
        BOOST_CHECK_THROW(
            box->set_pokemon(box->get_capacity(), bulbasaur);
        , pkmn::range_error);

        box->set_pokemon(0, bulbasaur);
        box->set_pokemon(1, charmander);

        // Make sure we can't do that again.
        BOOST_CHECK_THROW(
            box->set_pokemon(2, bulbasaur);
        , std::runtime_error);
        BOOST_CHECK_THROW(
            box->set_pokemon(3, charmander);
        , std::runtime_error);

        // Replace one of the new ones.
        box->set_pokemon(0, squirtle);

        /*
         * Now check everything we've created. Each variable should have the
         * same Pokémon underneath, even if the pointer has changed.
         */
        BOOST_CHECK_EQUAL(box->get_pokemon(0)->get_species(), "Squirtle");
        BOOST_CHECK_EQUAL(box->get_pokemon(1)->get_species(), "Charmander");
        BOOST_CHECK_EQUAL(original_first->get_species(), "None");
        BOOST_CHECK_EQUAL(original_second->get_species(), "None");
        BOOST_CHECK_EQUAL(bulbasaur->get_species(), "Bulbasaur");
        BOOST_CHECK_EQUAL(charmander->get_species(), "Charmander");
        BOOST_CHECK_EQUAL(squirtle->get_species(), "Squirtle");

        // On the C++ level, make sure the expected equal pointers are equal.
        BOOST_CHECK_EQUAL(box->get_pokemon(0)->get_native_pc_data(), squirtle->get_native_pc_data());
        BOOST_CHECK_EQUAL(box->get_pokemon(1)->get_native_pc_data(), charmander->get_native_pc_data());
        BOOST_CHECK_NE(box->get_pokemon(0)->get_native_pc_data(), original_first->get_native_pc_data());
        BOOST_CHECK_NE(box->get_pokemon(1)->get_native_pc_data(), original_second->get_native_pc_data());

        // On the C++ level, check the underlying PKSav struct.
        int generation = game_generations.at(box->get_game());
        switch(generation) {
            case 1: {
                const pksav_gen1_pokemon_box_t* native_box = reinterpret_cast<const pksav_gen1_pokemon_box_t*>(box->get_native());
                const pkmn::pokemon_list_t& pokemon_list = box->as_vector();
                for(size_t i = 0; i < pokemon_list.size(); ++i) {
                    BOOST_CHECK_EQUAL(
                        int(native_box->species[i]),
                        pokemon_list.at(i)->get_database_entry().get_pokemon_index()
                    );
                    BOOST_CHECK_EQUAL(
                        int(native_box->entries[i].species),
                        pokemon_list.at(i)->get_database_entry().get_pokemon_index()
                    );
                    BOOST_CHECK_EQUAL(
                        &native_box->entries[i],
                        pokemon_list.at(i)->get_native_pc_data()
                    );
                }
                break;
            }

            case 2: {
                const pksav_gen2_pokemon_box_t* native_box = reinterpret_cast<const pksav_gen2_pokemon_box_t*>(box->get_native());
                const pkmn::pokemon_list_t& pokemon_list = box->as_vector();
                for(size_t i = 0; i < pokemon_list.size(); ++i) {
                    BOOST_CHECK_EQUAL(
                        int(native_box->species[i]),
                        pokemon_list.at(i)->get_database_entry().get_pokemon_index()
                    );
                    BOOST_CHECK_EQUAL(
                        int(native_box->entries[i].species),
                        pokemon_list.at(i)->get_database_entry().get_pokemon_index()
                    );
                    BOOST_CHECK_EQUAL(
                        &native_box->entries[i],
                        pokemon_list.at(i)->get_native_pc_data()
                    );

                    char nickname[11] = {0};
                    PKSAV_CALL(
                        pksav_text_from_gen2(
                            native_box->nicknames[i],
                            nickname,
                            10
                        );
                    )
                    BOOST_CHECK_EQUAL(
                        pokemon_list.at(i)->get_nickname(),
                        std::string(nickname)
                    );

                    char otname[8] = {0};
                    PKSAV_CALL(
                        pksav_text_from_gen2(
                            native_box->otnames[i],
                            otname,
                            7
                        );
                    )
                    BOOST_CHECK_EQUAL(
                        pokemon_list.at(i)->get_trainer_name(),
                        std::string(otname)
                    );
                }
                break;
            }

            case 3: {
                if(game == "Colosseum" or game == "XD") {
                } else {
                    const pksav_gba_pokemon_box_t* native_box = reinterpret_cast<const pksav_gba_pokemon_box_t*>(box->get_native());
                    const pkmn::pokemon_list_t& pokemon_list = box->as_vector();
                    for(size_t i = 0; i < pokemon_list.size(); ++i) {
                        BOOST_CHECK_EQUAL(
                            &native_box->entries[i],
                            pokemon_list.at(i)->get_native_pc_data()
                        );
                    }
                }
                break;
            }

            default:
                break;
        }
    }

    void test_empty_pokemon_pc(
        pkmn::pokemon_pc::sptr pc,
        const std::string &game
    ) {
        BOOST_CHECK_EQUAL(pc->get_game(), game);

        // Make sure trying to get a box at an invalid index fails.
        BOOST_CHECK_THROW(
            (void)pc->get_box(-1);
        , pkmn::range_error);
        BOOST_CHECK_THROW(
            (void)pc->get_box(pc->get_num_boxes());
        , pkmn::range_error);

        const pkmn::pokemon_box_list_t& pokemon_box_list = pc->as_vector();
        BOOST_CHECK_EQUAL(pc->get_num_boxes(), pokemon_box_list.size());

        for(int i = 0; i < pc->get_num_boxes(); ++i) {
            test_empty_pokemon_box(
                pokemon_box_list.at(i),
                game
            );
        }
    }

    void test_pc_box_names(
        pkmn::pokemon_pc::sptr pc
    ) {
        std::string game = pc->get_game();
        if(game == "Red" or game == "Blue" or game == "Yellow") {
            BOOST_CHECK_THROW(
                pc->get_box(0)->set_name("ABCDEFGH");
            , pkmn::feature_not_in_game_error);
            BOOST_CHECK_THROW(
                (void)pc->get_box_names();
            , pkmn::feature_not_in_game_error);
        } else {
            for(int i = 0; i < pc->get_num_boxes(); ++i) {
                std::string box_name = str(boost::format("BOX%d") % (i+1));
                pc->get_box(i)->set_name(box_name);
            }

            const std::vector<std::string>& box_names = pc->get_box_names();
            BOOST_REQUIRE_EQUAL(box_names.size(), pc->get_num_boxes());
        }
    }

    // See pokemon_pc_gen2impl.hpp
    typedef struct {
        pksav_gen2_pokemon_pc_t* pc;
        pksav_gen2_pokemon_box_names_t* box_names;
    } gen2_pokemon_full_pc_t;

    void test_setting_pokemon_in_boxes(
        pkmn::pokemon_pc::sptr pc
    ) {
        std::string game = pc->get_game();

        for(int i = 0; i < pc->get_num_boxes(); ++i) {
            test_setting_pokemon(
                pc->get_box(i)
            );
        }
        for(int i = 0; i < pc->get_num_boxes(); ++i) {
            BOOST_CHECK_EQUAL(pc->get_box(i)->get_pokemon(0)->get_species(), "Squirtle");
            BOOST_CHECK_EQUAL(pc->get_box(i)->get_pokemon(1)->get_species(), "Charmander");
        }

        // On the C++ level, just check the values we've set to confirm the pointers worked.
        int generation = game_generations.at(game);
        switch(generation) {
            case 1: {
                const pksav_gen1_pokemon_box_t** native_boxes = reinterpret_cast<const pksav_gen1_pokemon_box_t**>(pc->get_native());
                const pkmn::pokemon_box_list_t& pokemon_box_list = pc->as_vector();
                for(size_t i = 0; i < pokemon_box_list.size(); ++i) {
                    const pkmn::pokemon_list_t& pokemon_list = pokemon_box_list.at(i)->as_vector();
                    BOOST_CHECK_EQUAL(
                        int(native_boxes[i]->species[0]),
                        pokemon_list.at(0)->get_database_entry().get_pokemon_index()
                    );
                    BOOST_CHECK_EQUAL(
                        int(native_boxes[i]->entries[0].species),
                        pokemon_list.at(0)->get_database_entry().get_pokemon_index()
                    );
                    BOOST_CHECK_EQUAL(
                        int(native_boxes[i]->species[1]),
                        pokemon_list.at(1)->get_database_entry().get_pokemon_index()
                    );
                    BOOST_CHECK_EQUAL(
                        int(native_boxes[i]->entries[1].species),
                        pokemon_list.at(1)->get_database_entry().get_pokemon_index()
                    );
                    BOOST_CHECK_EQUAL(
                        native_boxes[i],
                        pokemon_box_list.at(i)->get_native()
                    );
                }
                break;
            }

            case 2: {
                const gen2_pokemon_full_pc_t* native_pc = reinterpret_cast<const gen2_pokemon_full_pc_t*>(pc->get_native());
                const pkmn::pokemon_box_list_t& pokemon_box_list = pc->as_vector();
                const std::vector<std::string>& box_names = pc->get_box_names();
                for(size_t i = 0; i < pokemon_box_list.size(); ++i) {
                    const pkmn::pokemon_list_t& pokemon_list = pokemon_box_list.at(i)->as_vector();
                    BOOST_CHECK_EQUAL(
                        int(native_pc->pc->boxes[i].species[0]),
                        pokemon_list.at(0)->get_database_entry().get_pokemon_index()
                    );
                    BOOST_CHECK_EQUAL(
                        int(native_pc->pc->boxes[i].entries[0].species),
                        pokemon_list.at(0)->get_database_entry().get_pokemon_index()
                    );
                    BOOST_CHECK_EQUAL(
                        int(native_pc->pc->boxes[i].species[1]),
                        pokemon_list.at(1)->get_database_entry().get_pokemon_index()
                    );
                    BOOST_CHECK_EQUAL(
                        int(native_pc->pc->boxes[i].entries[1].species),
                        pokemon_list.at(1)->get_database_entry().get_pokemon_index()
                    );
                    BOOST_CHECK_EQUAL(
                        &native_pc->pc->boxes[i],
                        pokemon_box_list.at(i)->get_native()
                    );

                    char box_name[10] = {0};
                    PKSAV_CALL(
                        pksav_text_from_gen2(
                            native_pc->box_names->names[i],
                            box_name,
                            9
                        );
                    )
                    BOOST_CHECK_EQUAL(
                        box_names.at(i),
                        std::string(box_name)
                    );
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
                        BOOST_CHECK_EQUAL(
                            int(native_pc->boxes[i].entries[0].blocks.growth.species),
                            pokemon_list.at(0)->get_database_entry().get_pokemon_index()
                        );
                        BOOST_CHECK_EQUAL(
                            int(native_pc->boxes[i].entries[1].blocks.growth.species),
                            pokemon_list.at(1)->get_database_entry().get_pokemon_index()
                        );
                        BOOST_CHECK_EQUAL(
                            &native_pc->boxes[i],
                            pokemon_box_list.at(i)->get_native()
                        );

                        char box_name[10] = {0};
                        PKSAV_CALL(
                            pksav_text_from_gba(
                                native_pc->box_names[i],
                                box_name,
                                9
                            );
                        )
                        BOOST_CHECK_EQUAL(
                            box_names.at(i),
                            std::string(box_name)
                        );
                    }
                }
                break;
            }

            default:
                break;
        }
    }
}

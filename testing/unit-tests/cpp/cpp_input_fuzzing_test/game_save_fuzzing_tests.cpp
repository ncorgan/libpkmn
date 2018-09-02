/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pkmntest/util.hpp"

#include "utils.hpp"

#include "env.hpp"
#include "pksav/pksav_call.hpp"
#include "types/rng.hpp"

#include <pkmn/game_save.hpp>

#include <pksav/gen1/save.h>
#include <pksav/gen2/save.h>
#include <pksav/gba/save.h>

#include <pksav/math/endian.h>

// PKSav includes
#include "gen1/save_internal.h"
#include "gen2/save_internal.h"
#include "gba/checksum.h"
#include "gba/save_internal.h"

// LibPkmGC includes, inside warning suppressions
#include "pkmgc/includes.hpp"

#include <boost/filesystem.hpp>

#include <gtest/gtest.h>

namespace fs = boost::filesystem;

static const std::string SAV_EXTENSION = ".sav";
static const std::string GCI_EXTENSION = ".gci";

TEST(input_fuzzing_test, test_fuzzing_game_save_static_fcns)
{
    static const std::string SAVE_EXTENSION = ".sav";

    static const size_t MIN_SAVE_SIZE = 0;
    static const size_t MAX_SAVE_SIZE = 1e6;

    pkmn::rng<size_t> size_rng;

    for(size_t iteration = 0; iteration < NUM_ITERATIONS; ++iteration)
    {
        std::string tmp_filepath = write_random_tmp_file(
                                       size_rng.rand(MIN_SAVE_SIZE, MAX_SAVE_SIZE),
                                       SAVE_EXTENSION
                                   );

        // We don't expect these calls to succeed, but we want to make sure
        // it doesn't crash.
        try
        {
            (void)pkmn::game_save::detect_type(tmp_filepath);
        }
        catch(...) {}
        try
        {
            (void)pkmn::game_save::from_file(tmp_filepath);
        }
        catch(...) {}

        fs::remove(tmp_filepath.c_str());
    }
}

// With nonsense under the hood, see if parsing anything crashes.
static void read_all_save_fields(
    const pkmn::game_save::sptr& save
)
{
    std::string game = save->get_game();

    int generation = game_generations.at(game);

    (void)save->get_trainer_name();
    (void)save->get_trainer_id();
    (void)save->get_trainer_public_id();

    if(generation >= 2)
    {
        (void)save->get_trainer_gender();
    }
    if(generation >= 3)
    {
        (void)save->get_trainer_secret_id();
    }

    if((game != "Colosseum") and (game != "XD"))
    {
        (void)save->get_rival_name();
    }

    for(const std::string& attribute: save->get_numeric_attribute_names())
    {
        (void)save->get_numeric_attribute(attribute);
    }
    for(const std::string& attribute: save->get_string_attribute_names())
    {
        (void)save->get_string_attribute(attribute);
    }
    for(const std::string& attribute: save->get_boolean_attribute_names())
    {
        (void)save->get_boolean_attribute(attribute);
    }

    // Item Bag
    const pkmn::item_pockets_t& item_pockets = save->get_item_bag()->get_pockets();
    for(const auto& pocket_pair: item_pockets)
    {
        const pkmn::item_list::sptr& pocket = pocket_pair.second;

        (void)pocket->get_valid_items();
    }

    // Pokémon Party
    save->get_pokemon_party()->get_num_pokemon();
    for(const auto& pokemon: save->get_pokemon_party()->as_vector())
    {
        read_all_pokemon_fields(pokemon);
    }

    // Pokémon PC
    if(generation >= 2)
    {
        (void)save->get_pokemon_pc()->get_box_names();
    }
    for(const auto& box: save->get_pokemon_pc()->as_vector())
    {
        (void)box->get_num_pokemon();

        for(const auto& pokemon: box->as_vector())
        {
            read_all_pokemon_fields(pokemon);
        }

        if(generation >= 2)
        {
            (void)box->get_name();
        }
    }
}

// Generation I fuzzing

typedef std::tuple<enum pksav_gen1_save_type, std::string> gen1_save_fuzzing_test_params_t;

class gen1_save_fuzzing_test: public ::testing::TestWithParam<gen1_save_fuzzing_test_params_t>
{};

// Create a random vector the size of a Generation I save, set its checksum
// so PKSav believes it's authentic, and see how LibPKMN+PKsav deals with it.
TEST_P(gen1_save_fuzzing_test, test_fuzzing_gen1_save)
{
    auto test_params = GetParam();

    enum pksav_gen1_save_type expected_pksav_save_type = std::get<0>(test_params);
    std::string expected_detected_type = std::get<1>(test_params);

    for(size_t iteration = 0; iteration < NUM_ITERATIONS; ++iteration)
    {
        std::vector<uint8_t> gen1_save_vector;
        randomize_vector(PKSAV_GEN1_SAVE_SIZE, &gen1_save_vector);

        // Set the Pikachu friendship depending on what version this is supposed
        // to be, as this is the only method PKSav has to differentiate between
        // Red/Blue and Yellow.
        if(expected_pksav_save_type == PKSAV_GEN1_SAVE_TYPE_RED_BLUE)
        {
            gen1_save_vector[PKSAV_GEN1_PIKACHU_FRIENDSHIP] = 0;
        }
        else
        {
            gen1_save_vector[PKSAV_GEN1_PIKACHU_FRIENDSHIP] = 255;
        }

        // Set the checksum.
        gen1_save_vector[PKSAV_GEN1_CHECKSUM] = pksav_gen1_get_save_checksum(
                                                    gen1_save_vector.data()
                                                );

        // Make sure PKSav thinks the buffer is valid.
        enum pksav_gen1_save_type gen1_save_type = PKSAV_GEN1_SAVE_TYPE_NONE;
        PKSAV_CALL(
            pksav_gen1_get_buffer_save_type(
                gen1_save_vector.data(),
                gen1_save_vector.size(),
                &gen1_save_type
            );
        )
        ASSERT_EQ(expected_pksav_save_type, gen1_save_type);

        std::string random_filepath1 = get_random_filepath(SAV_EXTENSION);
        write_vector_to_file(gen1_save_vector, random_filepath1);

        ASSERT_EQ(expected_detected_type, pkmn::game_save::detect_type(random_filepath1));

        // This should pass, as LibPKMN should label invalid inputs as invalid
        // rather than erroring out.
        pkmn::game_save::sptr save1 = pkmn::game_save::from_file(random_filepath1);
        read_all_save_fields(save1);

        std::string random_filepath2 = get_random_filepath(SAV_EXTENSION);
        save1->save_as(random_filepath2);

        pkmn::game_save::sptr save2 = pkmn::game_save::from_file(random_filepath2);
        read_all_save_fields(save2);

        fs::remove(random_filepath2);
        fs::remove(random_filepath1);
    }
}

static const std::vector<gen1_save_fuzzing_test_params_t> GEN1_TEST_PARAMS =
{
    std::make_tuple(PKSAV_GEN1_SAVE_TYPE_RED_BLUE, "Red/Blue"),
    std::make_tuple(PKSAV_GEN1_SAVE_TYPE_YELLOW,   "Yellow")
};

INSTANTIATE_TEST_CASE_P(
    gen1_save_fuzzing_test,
    gen1_save_fuzzing_test,
    ::testing::ValuesIn(GEN1_TEST_PARAMS)
);

// Generation II fuzzing

typedef std::tuple<enum pksav_gen2_save_type, std::string> gen2_save_fuzzing_test_params_t;

class gen2_save_fuzzing_test: public ::testing::TestWithParam<gen2_save_fuzzing_test_params_t>
{};

// Create a random vector the size of a Generation II save, set its checksums
// so PKSav believes it's authentic, and see how LibPKMN+PKsav deals with it.
TEST_P(gen2_save_fuzzing_test, test_fuzzing_gen2_save)
{
    auto test_params = GetParam();

    enum pksav_gen2_save_type expected_pksav_save_type = std::get<0>(test_params);
    std::string expected_detected_type = std::get<1>(test_params);

    for(size_t iteration = 0; iteration < NUM_ITERATIONS; ++iteration)
    {
        std::vector<uint8_t> gen2_save_vector;
        randomize_vector(PKSAV_GEN2_SAVE_SIZE, &gen2_save_vector);

        // Set the checksum, depending on what save type we're testing.
        size_t checksum1_index = 0;
        size_t checksum2_index = 0;
        if(expected_pksav_save_type == PKSAV_GEN2_SAVE_TYPE_GS)
        {
            checksum1_index = PKSAV_GS_CHECKSUM1;
            checksum2_index = PKSAV_GS_CHECKSUM2;
        }
        else
        {
            checksum1_index = PKSAV_CRYSTAL_CHECKSUM1;
            checksum2_index = PKSAV_CRYSTAL_CHECKSUM2;
        }

        uint16_t* checksum1_ptr = reinterpret_cast<uint16_t*>(
                                      &gen2_save_vector[checksum1_index]
                                  );
        uint16_t* checksum2_ptr = reinterpret_cast<uint16_t*>(
                                      &gen2_save_vector[checksum2_index]
                                  );

        pksav_gen2_get_save_checksums(
            expected_pksav_save_type,
            gen2_save_vector.data(),
            checksum1_ptr,
            checksum2_ptr
        );

        // Make sure PKSav thinks the buffer is valid.
        enum pksav_gen2_save_type gen2_save_type = PKSAV_GEN2_SAVE_TYPE_NONE;
        PKSAV_CALL(
            pksav_gen2_get_buffer_save_type(
                gen2_save_vector.data(),
                gen2_save_vector.size(),
                &gen2_save_type
            );
        )
        ASSERT_EQ(expected_pksav_save_type, gen2_save_type);

        std::string random_filepath1 = get_random_filepath(SAV_EXTENSION);
        write_vector_to_file(gen2_save_vector, random_filepath1);

        ASSERT_EQ(expected_detected_type, pkmn::game_save::detect_type(random_filepath1));

        // This should pass, as LibPKMN should label invalid inputs as invalid
        // rather than erroring out.
        pkmn::game_save::sptr save1 = pkmn::game_save::from_file(random_filepath1);
        read_all_save_fields(save1);

        std::string random_filepath2 = get_random_filepath(SAV_EXTENSION);
        save1->save_as(random_filepath2);

        pkmn::game_save::sptr save2 = pkmn::game_save::from_file(random_filepath2);
        read_all_save_fields(save2);

        fs::remove(random_filepath2);
        fs::remove(random_filepath1);
    }
}

static const std::vector<gen2_save_fuzzing_test_params_t> GEN2_TEST_PARAMS =
{
    std::make_tuple(PKSAV_GEN2_SAVE_TYPE_GS,      "Gold/Silver"),
    std::make_tuple(PKSAV_GEN2_SAVE_TYPE_CRYSTAL, "Crystal")
};

INSTANTIATE_TEST_CASE_P(
    gen2_save_fuzzing_test,
    gen2_save_fuzzing_test,
    ::testing::ValuesIn(GEN2_TEST_PARAMS)
);

// Generation II fuzzing

typedef std::tuple<enum pksav_gba_save_type, std::string> gba_save_fuzzing_test_params_t;

class gba_save_fuzzing_test: public ::testing::TestWithParam<gba_save_fuzzing_test_params_t>
{};

// Create a random vector the size of a GBA save, set its checksums, etc,
// so PKSav believes it's authentic, and see how LibPKMN+PKsav deals with it.
TEST_P(gba_save_fuzzing_test, test_fuzzing_gba_save)
{
    auto test_params = GetParam();

    enum pksav_gba_save_type expected_pksav_save_type = std::get<0>(test_params);
    std::string expected_detected_type = std::get<1>(test_params);

    for(size_t iteration = 0; iteration < NUM_ITERATIONS; ++iteration)
    {
        std::vector<uint8_t> gba_save_vector;
        randomize_vector(
            PKSAV_GBA_SAVE_SLOT_SIZE * 2,
            &gba_save_vector
        );
        union pksav_gba_save_slot* save_slots_ptr =
            reinterpret_cast<union pksav_gba_save_slot*>(
                gba_save_vector.data()
            );

        for(uint32_t slot_index = 0; slot_index < 2; ++slot_index)
        {
            union pksav_gba_save_slot* save_slot_ptr = &save_slots_ptr[slot_index];

            for(uint8_t section_index = 0;
                section_index < PKSAV_GBA_NUM_SAVE_SECTIONS;
                ++section_index)
            {
                struct pksav_gba_section_footer* footer_ptr =
                    &save_slot_ptr->sections_arr[section_index].footer;

                footer_ptr->section_id = section_index;
                footer_ptr->validation = pksav_littleendian32(
                                             PKSAV_GBA_VALIDATION_MAGIC
                                         );
                footer_ptr->save_index = pksav_littleendian32(slot_index);
            }

            pksav_gba_set_section_checksums(&save_slots_ptr[slot_index]);

            static PKMN_CONSTEXPR_OR_CONST uint32_t rs_game_code = 0;
            static PKMN_CONSTEXPR_OR_CONST uint32_t frlg_game_code = 1;

            const size_t security_key1_offset =
                PKSAV_GBA_SAVE_SECTION0_OFFSETS[expected_pksav_save_type-1][PKSAV_GBA_SECURITY_KEY1];
            const size_t security_key2_offset =
                PKSAV_GBA_SAVE_SECTION0_OFFSETS[expected_pksav_save_type-1][PKSAV_GBA_SECURITY_KEY2];

            static PKMN_CONSTEXPR_OR_CONST uint32_t dummy_security_key = 0x13510418;

            save_slot_ptr->section0.data32[security_key1_offset/4] = dummy_security_key;
            save_slot_ptr->section0.data32[security_key2_offset/4] = dummy_security_key;

            if(expected_pksav_save_type != PKSAV_GBA_SAVE_TYPE_EMERALD)
            {
                const size_t game_code_offset =
                    PKSAV_GBA_SAVE_SECTION0_OFFSETS[expected_pksav_save_type-1][PKSAV_GBA_GAME_CODE];

                save_slot_ptr->section0.data32[game_code_offset/4] =
                    pksav_littleendian32(
                        (expected_pksav_save_type == PKSAV_GBA_SAVE_TYPE_RS)
                            ? rs_game_code : frlg_game_code
                    );
            }
        }

        // Make sure PKSav thinks the buffer is valid.
        enum pksav_gba_save_type gba_save_type = PKSAV_GBA_SAVE_TYPE_NONE;
        PKSAV_CALL(
            pksav_gba_get_buffer_save_type(
                gba_save_vector.data(),
                gba_save_vector.size(),
                &gba_save_type
            );
        )
        ASSERT_EQ(expected_pksav_save_type, gba_save_type);

        std::string random_filepath1 = get_random_filepath(SAV_EXTENSION);
        write_vector_to_file(gba_save_vector, random_filepath1);

        ASSERT_EQ(expected_detected_type, pkmn::game_save::detect_type(random_filepath1));

        // This should pass, as LibPKMN should label invalid inputs as invalid
        // rather than erroring out.
        pkmn::game_save::sptr save1 = pkmn::game_save::from_file(random_filepath1);
        read_all_save_fields(save1);

        std::string random_filepath2 = get_random_filepath(SAV_EXTENSION);
        save1->save_as(random_filepath2);

        pkmn::game_save::sptr save2 = pkmn::game_save::from_file(random_filepath2);
        read_all_save_fields(save2);

        fs::remove(random_filepath2);
        fs::remove(random_filepath1);
    }
}

static const std::vector<gba_save_fuzzing_test_params_t> GBA_TEST_PARAMS =
{
    std::make_tuple(PKSAV_GBA_SAVE_TYPE_RS,      "Ruby/Sapphire"),
    std::make_tuple(PKSAV_GBA_SAVE_TYPE_EMERALD, "Emerald"),
    std::make_tuple(PKSAV_GBA_SAVE_TYPE_FRLG,    "FireRed/LeafGreen")
};

INSTANTIATE_TEST_CASE_P(
    gba_save_fuzzing_test,
    gba_save_fuzzing_test,
    ::testing::ValuesIn(GBA_TEST_PARAMS)
);

static void randomize_libpkmgc_data_struct(
    LibPkmGC::Base::DataStruct* p_data_struct
)
{
    ASSERT_NE(nullptr, p_data_struct);

    randomize_buffer(
        p_data_struct->data,
        p_data_struct->getSize()
    );
}

static void randomize_libpkmgc_save(
    LibPkmGC::GC::SaveEditing::Save* p_libpkmgc_save
)
{
    ASSERT_NE(nullptr, p_libpkmgc_save);

    for(size_t save_slot_index = 0;
        save_slot_index < p_libpkmgc_save->nbSlots;
        ++save_slot_index)
    {
        LibPkmGC::GC::SaveEditing::SaveSlot* p_save_slot = p_libpkmgc_save->saveSlots[save_slot_index];

        randomize_libpkmgc_data_struct(p_save_slot->gameConfig);

        for(size_t party_index = 0; party_index < 6; ++party_index)
        {
            randomize_libpkmgc_data_struct(
                p_save_slot->player->trainer->party[party_index]
            );
        }
        randomize_libpkmgc_data_struct(p_save_slot->player->trainer);
        randomize_libpkmgc_data_struct(p_save_slot->player->bag);
        randomize_libpkmgc_data_struct(p_save_slot->player);

        for(size_t pc_index = 0; pc_index < p_save_slot->PC->nbBoxes; ++pc_index)
        {
            LibPkmGC::GC::PokemonBox* p_pokemon_box = p_save_slot->PC->boxes[pc_index];

            for(size_t box_index = 0; box_index < 30; ++box_index)
            {
                randomize_libpkmgc_data_struct(p_pokemon_box->pkm[box_index]);
            }

            randomize_libpkmgc_data_struct(p_pokemon_box);
        }
        randomize_libpkmgc_data_struct(p_save_slot->PC);

        randomize_libpkmgc_data_struct(p_save_slot->mailbox);

        randomize_libpkmgc_data_struct(p_save_slot->daycare->pkm);
        randomize_libpkmgc_data_struct(p_save_slot->daycare);

        for(size_t strategy_memo_index = 0;
            strategy_memo_index < 500;
            ++strategy_memo_index)
        {
            randomize_libpkmgc_data_struct(
                p_save_slot->strategyMemo->entries[strategy_memo_index]
            );
        }
        randomize_libpkmgc_data_struct(p_save_slot->strategyMemo);

        for(size_t rule_index = 0; rule_index < 6; ++rule_index)
        {
            randomize_libpkmgc_data_struct(
                p_save_slot->battleMode->rules[rule_index]
            );
        }
        randomize_libpkmgc_data_struct(
            p_save_slot->battleMode
        );
        randomize_libpkmgc_data_struct(
            p_save_slot->ribbonDescriptions
        );

        if(LIBPKMGC_IS_XD(SaveEditing::SaveSlot, p_save_slot))
        {
            LibPkmGC::XD::SaveEditing::SaveSlot* p_xd_save_slot =
                dynamic_cast<LibPkmGC::XD::SaveEditing::SaveSlot*>(
                    p_save_slot
                );
            ASSERT_NE(nullptr, p_xd_save_slot);

            randomize_libpkmgc_data_struct(p_xd_save_slot->purifier);
        }
    }
}

// It's easier to randomize an existing save than to manually create one
// like with PKSav's early-gen saves.
template <typename libpkmgc_save_type>
static void generate_random_libpkmgc_save(
    const std::string& save_filename,
    std::string* p_filepath
)
{
    ASSERT_NE(nullptr, p_filepath);

    static const fs::path LIBPKMN_TEST_FILES(pkmn_getenv("LIBPKMN_TEST_FILES"));

    fs::path save_path = LIBPKMN_TEST_FILES / "gamecube_saves" / save_filename;

    std::vector<uint8_t> save_contents;
    read_file_to_vector(save_path.string(), &save_contents);
    ASSERT_FALSE(save_contents.empty());

    libpkmgc_save_type save(save_contents.data());
    ASSERT_NO_FATAL_FAILURE(randomize_libpkmgc_save(&save));
    save.save();

    *p_filepath = get_random_filepath(GCI_EXTENSION);
    ASSERT_NO_FATAL_FAILURE(write_vector_to_file(
        save_contents,
        *p_filepath
    ));
}

TEST(colosseum_save_fuzzing_test, test_fuzzing_colosseum_save)
{
    for(size_t iteration = 0; iteration < NUM_ITERATIONS; ++iteration)
    {
        std::string random_filepath1;
        ASSERT_NO_FATAL_FAILURE(
            generate_random_libpkmgc_save<LibPkmGC::Colosseum::SaveEditing::Save>(
                "pokemon_colosseum.gci",
                &random_filepath1
            )
        );
        ASSERT_TRUE(fs::exists(random_filepath1));

        // This should pass, as LibPKMN should label invalid inputs as invalid
        // rather than erroring out.
        ASSERT_EQ("Colosseum/XD", pkmn::game_save::detect_type(random_filepath1));

        pkmn::game_save::sptr save1 = pkmn::game_save::from_file(random_filepath1);
        read_all_save_fields(save1);

        std::string random_filepath2 = get_random_filepath(GCI_EXTENSION);
        save1->save_as(random_filepath2);

        pkmn::game_save::sptr save2 = pkmn::game_save::from_file(random_filepath2);
        read_all_save_fields(save2);

        fs::remove(random_filepath2);
        fs::remove(random_filepath1);
    }
}

TEST(xd_save_fuzzing_test, DISABLED_test_fuzzing_xd_save)
{
    for(size_t iteration = 0; iteration < NUM_ITERATIONS; ++iteration)
    {
        std::string random_filepath1;
        ASSERT_NO_FATAL_FAILURE(
            generate_random_libpkmgc_save<LibPkmGC::XD::SaveEditing::Save>(
                "pokemon_xd.gci",
                &random_filepath1
            )
        );
        ASSERT_TRUE(fs::exists(random_filepath1));

        // This should pass, as LibPKMN should label invalid inputs as invalid
        // rather than erroring out.
        ASSERT_EQ("Colosseum/XD", pkmn::game_save::detect_type(random_filepath1));

        pkmn::game_save::sptr save1 = pkmn::game_save::from_file(random_filepath1);
        read_all_save_fields(save1);

        std::string random_filepath2 = get_random_filepath(GCI_EXTENSION);
        save1->save_as(random_filepath2);

        pkmn::game_save::sptr save2 = pkmn::game_save::from_file(random_filepath2);
        read_all_save_fields(save2);

        fs::remove(random_filepath2);
        fs::remove(random_filepath1);
    }
}

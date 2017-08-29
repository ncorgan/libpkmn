#!/usr/bin/ruby
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

require_relative "PokemonTest"

require "pathname"

class FormTest < PokemonTest

    GEN1_POKEMON_WITH_XY_MEGA_FORMS = [
        "Venusaur", "Blastoise", "Alakazam", "Gengar", "Kangaskhan", "Pinsir", "Gyarados", "Aerodactyl"
    ]
    GEN1_POKEMON_WITH_ORAS_MEGA_FORMS = [
        "Beedrill", "Pidgeot", "Slowbro"
    ]

    GEN2_POKEMON_WITH_XY_MEGA_FORMS = [
        "Ampharos", "Scizor", "Heracross", "Houndoom", "Tyranitar"
    ]

    GEN3_POKEMON_WITH_XY_MEGA_FORMS = [
        "Blaziken", "Gardevoir", "Mawile", "Aggron", "Medicham",
        "Manectric", "Banette", "Absol", "Latias", "Latios", "Garchomp"
    ]
    GEN3_POKEMON_WITH_ORAS_MEGA_FORMS = [
        "Sceptile", "Swampert", "Sableye", "Sharpedo", "Camerupt",
        "Altaria", "Glalie", "Salamence", "Metagross", "Rayquaza"
    ]

    def _test_gen1_forms(game)
        generation = @@GAME_GENERATIONS[game]

        GEN1_POKEMON_WITH_XY_MEGA_FORMS.each do |species|
            if generation >= 6
                PKMN::Pokemon.new(species, game, "Mega", 100)
            else
                assert_raises ArgumentError do
                    PKMN::Pokemon.new(species, game, "Mega", 100)
                end
            end
        end

        GEN1_POKEMON_WITH_ORAS_MEGA_FORMS.each do |species|
            if game == "Omega Ruby" or game == "Alpha Sapphire"
                PKMN::Pokemon.new(species, game, "Mega", 100)
            else
                assert_raises ArgumentError do
                    PKMN::Pokemon.new(species, game, "Mega", 100)
                end
            end
        end

        # Cosplay Pikachu should only work for OR/AS.
        oras_pikachu_forms = PKMN::Database::PokemonEntry.new("Pikachu", "Omega Ruby", "").forms
        oras_pikachu_forms.each do |form|
            if form != "Standard"
                if game == "Omega Ruby" or game == "Alpha Sapphire"
                    PKMN::Pokemon.new("Pikachu", game, form, 100)
                else
                    assert_raises ArgumentError do
                        PKMN::Pokemon.new("Pikachu", game, form, 100)
                    end
                end
            end
        end

        # Hardcode Mega X/Y cases.
        ["Charizard", "Mewtwo"].each do |species|
            if generation >= 6
                PKMN::Pokemon.new(species, game, "Mega X", 100)
                PKMN::Pokemon.new(species, game, "Mega Y", 100)
            else
                assert_raises ArgumentError do
                    PKMN::Pokemon.new(species, game, "Mega X", 100)
                end
                assert_raises ArgumentError do
                    PKMN::Pokemon.new(species, game, "Mega Y", 100)
                end
            end
        end
    end

    def _test_gen2_forms(game)
        generation = @@GAME_GENERATIONS[game]

        GEN2_POKEMON_WITH_XY_MEGA_FORMS.each do |species|
            if generation >= 6
                PKMN::Pokemon.new(species, game, "Mega", 100)
            else
                assert_raises ArgumentError do
                    PKMN::Pokemon.new(species, game, "Mega", 100)
                end
            end
        end

        if generation >= 6
            PKMN::Pokemon.new("Steelix", game, "Mega", 100)
        else
            assert_raises ArgumentError do
                PKMN::Pokemon.new("Steelix", game, "Mega", 100)
            end
        end

        # Spiky-eared Pichu should only work in HG/SS.
        if game == "HeartGold" or game == "SoulSilver"
            PKMN::Pokemon.new("Pichu", game, "Spiky-eared", 100)
        else
            assert_raises ArgumentError do
                PKMN::Pokemon.new("Pichu", game, "Spiky-eared", 100)
            end
        end

        # Unown's "!" and "?" forms aren't in Generation II.
        unown_forms = PKMN::Database::PokemonEntry.new("Unown", "Omega Ruby", "").forms
        (0..unown_forms.length-1).each do |i|
            if i < 26
                unown = PKMN::Pokemon.new("Unown", game, unown_forms[i], 10)
                assert(Pathname.new(unown.icon_filepath).exist?)
                assert(Pathname.new(unown.sprite_filepath).exist?)
            end
        end
        ["?", "!"].each do |form|
            if generation >= 3
                unown = PKMN::Pokemon.new("Unown", game, form, 10)
                assert(Pathname.new(unown.icon_filepath).exist?)
                assert(Pathname.new(unown.sprite_filepath).exist?)
            else
                assert_raises ArgumentError do
                    PKMN::Pokemon.new("Unown", game, form, 10)
                end
            end
        end
    end

    def _test_gba_forms(game)
        generation = @@GAME_GENERATIONS[game]

        GEN3_POKEMON_WITH_XY_MEGA_FORMS.each do |species|
            if generation >= 6
                PKMN::Pokemon.new(species, game, "Mega", 100)
            else
                assert_raises ArgumentError do
                    PKMN::Pokemon.new(species, game, "Mega", 100)
                end
            end
        end

        GEN3_POKEMON_WITH_ORAS_MEGA_FORMS.each do |species|
            if game == "Omega Ruby" or game == "Alpha Sapphire"
                PKMN::Pokemon.new(species, game, "Mega", 100)
            else
                assert_raises ArgumentError do
                    PKMN::Pokemon.new(species, game, "Mega", 100)
                end
            end
        end

        # Castform should always work.
        castform_forms = PKMN::Database::PokemonEntry.new("Castform", "Omega Ruby", "").forms
        castform_forms.each do |form|
            castform = PKMN::Pokemon.new("Castform", game, form, 30)
            assert(Pathname.new(castform.icon_filepath).exist?)
            assert(Pathname.new(castform.sprite_filepath).exist?)
        end

        # Primal Reversion should only work in OR/AS.
        ["Groudon", "Kyogre"].each do |species|
            PKMN::Pokemon.new(species, game, "", 70)
            if game == "Omega Ruby" or game == "Alpha Sapphire"
                PKMN::Pokemon.new(species, game, "Primal Reversion", 70)
            else
                assert_raises ArgumentError do
                    PKMN::Pokemon.new(species, game, "Primal Reversion", 70)
                end
            end
        end

        # In Generation III, Deoxys's form is game-specific.
        if generation == 3
            if game == "Ruby" or game == "Sapphire" or game == "Colosseum" or game == "XD"
                deoxys = PKMN::Pokemon.new("Deoxys", game, "Normal", 70)
                assert(Pathname.new(deoxys.icon_filepath).exist?)
                assert(Pathname.new(deoxys.sprite_filepath).exist?)
            else
                assert_raises ArgumentError do
                    PKMN::Pokemon.new("Deoxys", game, "Normal", 70)
                end
            end

            if game == "FireRed"
                deoxys = PKMN::Pokemon.new("Deoxys", game, "Attack", 70)
                assert(Pathname.new(deoxys.icon_filepath).exist?)
                assert(Pathname.new(deoxys.sprite_filepath).exist?)
            else
                assert_raises ArgumentError do
                    PKMN::Pokemon.new("Deoxys", game, "Attack", 70)
                end
            end

            if game == "LeafGreen"
                deoxys = PKMN::Pokemon.new("Deoxys", game, "Defense", 70)
                assert(Pathname.new(deoxys.icon_filepath).exist?)
                assert(Pathname.new(deoxys.sprite_filepath).exist?)
            else
                assert_raises ArgumentError do
                    PKMN::Pokemon.new("Deoxys", game, "Defense", 70)
                end
            end

            if game == "Emerald"
                deoxys = PKMN::Pokemon.new("Deoxys", game, "Speed", 70)
                assert(Pathname.new(deoxys.icon_filepath).exist?)
                assert(Pathname.new(deoxys.sprite_filepath).exist?)
            else
                assert_raises ArgumentError do
                    PKMN::Pokemon.new("Deoxys", game, "Speed", 70)
                end
            end
        else
            deoxys_forms = PKMN::Database::PokemonEntry.new("Deoxys", "Omega Ruby", "").forms
            deoxys_forms.each do |form|
                deoxys = PKMN::Pokemon.new("Deoxys", game, form, 70)
                assert(Pathname.new(deoxys.icon_filepath).exist?)
                assert(Pathname.new(deoxys.sprite_filepath).exist?)
            end
        end
    end

    GEN1_GAMES = ["Red", "Blue", "Yellow"]
    GEN2_GAMES = ["Gold", "Silver", "Crystal"]
    GBA_GAMES  = ["Ruby", "Sapphire", "Emerald", "FireRed", "LeafGreen"]

    GEN1_PARAMS = GEN1_GAMES + GEN2_GAMES + GBA_GAMES
    GEN2_PARAMS = GEN2_GAMES + GBA_GAMES
    GBA_PARAMS  = GBA_GAMES

    GEN1_PARAMS.each do |game|
        define_method("test_gen1_forms_#{game}") do
            _test_gen1_forms(game)
        end
    end
    GEN2_PARAMS.each do |game|
        define_method("test_gen2_forms_#{game}") do
            _test_gen2_forms(game)
        end
    end
    GBA_PARAMS.each do |game|
        define_method("test_gba_forms_#{game}") do
            _test_gba_forms(game)
        end
    end
end
